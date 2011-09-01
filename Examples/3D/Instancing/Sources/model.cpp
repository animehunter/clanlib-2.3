/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Mark Page
*/

#include "precomp.h"
#include "model.h"
#include "graphic_store.h"

class Model_Impl
{
public:
	Model_Impl();

	void Load(CL_GraphicContext &gc, const char *filename);

	void Draw(CL_GraphicContext &gc, GraphicStore *gs, const CL_Mat4f &modelview_matrix, int instance_count);
	void SetMaterial(float new_material_shininess, const CL_Vec4f &new_material_emission, const CL_Vec4f &new_material_ambient, const CL_Vec4f &new_material_specular);

private:
	int count_vertices(const struct aiScene* sc, const struct aiNode* nd);
	void insert_vbo(int vertex_count, const struct aiScene* sc, const struct aiNode* nd);

	CL_VertexArrayBuffer vbo_positions;
	CL_VertexArrayBuffer vbo_normals;
	CL_VertexArrayBuffer vbo_texcoords;
	int vbo_size;

	bool generate_texture_coords;

	float material_shininess;
	CL_Vec4f material_emission;
	CL_Vec4f material_ambient;
	CL_Vec4f material_specular;
};


Model::Model()
{
}

Model::Model(CL_GraphicContext &gc, const char *filename): impl(new Model_Impl())
{
	impl->Load(gc, filename);
}

bool Model::is_null()
{
	return !impl;
}

void Model::Draw(CL_GraphicContext &gc, GraphicStore *gs, const CL_Mat4f &modelview_matrix)
{
	impl->Draw(gc, gs, modelview_matrix, 1);
}

void Model::Draw(CL_GraphicContext &gc, GraphicStore *gs, const CL_Mat4f &modelview_matrix, int instance_count)
{
	impl->Draw(gc, gs, modelview_matrix, instance_count);
}

Model_Impl::Model_Impl()
{
	vbo_size = 0;
	generate_texture_coords = true;
	material_shininess = 64.0f;
	material_emission = CL_Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	material_ambient =  CL_Vec4f(0.9f, 0.2f, 0.2f, 1.0f);
	material_specular = CL_Vec4f(0.0f, 0.0f, 0.0f, 1.0f);

}

void Model::SetMaterial(float new_material_shininess, const CL_Vec4f &new_material_emission, const CL_Vec4f &new_material_ambient, const CL_Vec4f &new_material_specular)
{
	impl->SetMaterial(new_material_shininess, new_material_emission, new_material_ambient, new_material_specular);
}

void Model_Impl::SetMaterial(float new_material_shininess, const CL_Vec4f &new_material_emission, const CL_Vec4f &new_material_ambient, const CL_Vec4f &new_material_specular)
{
	material_shininess = new_material_shininess;
	material_emission = new_material_emission;
	material_ambient = new_material_ambient;
	material_specular = new_material_specular;
}

void Model_Impl::Load(CL_GraphicContext &gc, const char *filename)
{
	generate_texture_coords = true;

	const struct aiScene* scene = aiImportFile(filename,aiProcessPreset_TargetRealtime_MaxQuality);
	if (!scene)
		throw CL_Exception("Cannot load a model");

	try
	{
		vbo_size = count_vertices(scene, scene->mRootNode);
		if (!vbo_size)
			throw CL_Exception("No vertices found in the model");
	
		vbo_positions = CL_VertexArrayBuffer(gc, vbo_size * sizeof(CL_Vec3f), cl_usage_static_draw);
		vbo_normals = CL_VertexArrayBuffer(gc, vbo_size * sizeof(CL_Vec3f), cl_usage_static_draw);
		if (generate_texture_coords)
			vbo_texcoords = CL_VertexArrayBuffer(gc, vbo_size * sizeof(CL_Vec2f), cl_usage_static_draw);
		insert_vbo(0, scene, scene->mRootNode);
	}catch(...)
	{
		aiReleaseImport(scene);
		throw;
	}

	aiReleaseImport(scene);
}

int Model_Impl::count_vertices(const struct aiScene* sc, const struct aiNode* nd)
{
	int vertex_count = 0;
	unsigned int n = 0, t;

	// All meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n)
	{
		const struct aiMesh* mesh = sc->mMeshes[nd->mMeshes[n]];
		int num_vertex = mesh->mNumFaces * 3;
		if (!num_vertex)
			continue;

		if (mesh->mTextureCoords[0] == NULL || mesh->mTextureCoords[0][0] == NULL)
			generate_texture_coords = false;

		vertex_count += num_vertex;
		for (t = 0; t < mesh->mNumFaces; ++t)
		{
			if (mesh->mFaces[t].mNumIndices != 3)
					throw CL_Exception("This example only supports triangles");
		}
	}

	// All children
	for (n = 0; n < nd->mNumChildren; ++n)
	{
		vertex_count += count_vertices(sc, nd->mChildren[n]);
	}

	return vertex_count;
}

void Model_Impl::insert_vbo(int vertex_count, const struct aiScene* sc, const struct aiNode* nd)
{
	int i;
	unsigned int n = 0, t;

	bool use_texcoords = !vbo_texcoords.is_null();

	// All meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n)
	{
		const struct aiMesh* mesh = sc->mMeshes[nd->mMeshes[n]];
		int num_vertex = mesh->mNumFaces * 3;
		if (!num_vertex)
			continue;

		std::vector<CL_Vec3f> normals;
		std::vector<CL_Vec3f> vertices;
		std::vector<CL_Vec2f> tex_coords;

		normals.reserve(num_vertex);
		vertices.reserve(num_vertex);

		if (use_texcoords)
		{
			if (mesh->mTextureCoords[0] == NULL || mesh->mTextureCoords[0][0] == NULL)
				throw CL_Exception("This example expects texcoords to be set for this object");
			tex_coords.reserve(num_vertex);
		}

		for (t = 0; t < mesh->mNumFaces; ++t)
		{
			const struct aiFace* face = &mesh->mFaces[t];
			if (face->mNumIndices != 3)
					throw CL_Exception("This example only supports triangles");

			for(i = 0; i < face->mNumIndices; i++)
			{
				int index = face->mIndices[i];
				normals.push_back(&mesh->mNormals[index].x);
				vertices.push_back( &mesh->mVertices[index].x);
				if (use_texcoords)
					tex_coords.push_back( &mesh->mTextureCoords[0][index].x);
			}
		}

		vbo_positions.upload_data(vertex_count * sizeof(CL_Vec3f), &vertices[0], num_vertex * sizeof(CL_Vec3f));
		vbo_normals.upload_data(vertex_count * sizeof(CL_Vec3f), &normals[0], num_vertex * sizeof(CL_Vec3f));
		if (use_texcoords)
			vbo_texcoords.upload_data(vertex_count * sizeof(CL_Vec2f), &tex_coords[0], num_vertex * sizeof(CL_Vec2f));

		vertex_count += num_vertex;
	}

	// All children
	for (n = 0; n < nd->mNumChildren; ++n)
	{
		insert_vbo(vertex_count, sc, nd->mChildren[n]);
	}

}

void Model_Impl::Draw(CL_GraphicContext &gc, GraphicStore *gs, const CL_Mat4f &modelview_matrix, int instance_count)
{
	if (!vbo_size)
		return;

	gc.set_modelview(modelview_matrix);

	CL_PrimitivesArray prim_array(gc);

	prim_array.set_attributes(0, vbo_positions, 3, cl_type_float, (void *) 0);
	prim_array.set_attributes(1, vbo_normals, 3, cl_type_float, (void *) 0);

	if (instance_count <= 1)
	{
		if (!vbo_texcoords.is_null())
		{
			prim_array.set_attributes(2, vbo_texcoords, 2, cl_type_float, (void *) 0);
			gc.set_texture(0, gs->texture_brick);
			gs->shader_texture.SetMaterial(material_shininess, material_emission, material_ambient, material_specular);
			gs->shader_texture.Use(gc);
		}
		else
		{
			gs->shader_color.SetMaterial(material_shininess, material_emission, material_ambient, material_specular);
			gs->shader_color.Use(gc);
		}

		gc.draw_primitives(cl_triangles, vbo_size, prim_array);
	}
	else
	{
		if (vbo_texcoords.is_null())
			throw CL_Exception("Shader is not available");

		gs->shader_color_instanced.SetMaterial(material_shininess, material_emission);
		gs->shader_color_instanced.Use(gc);

		gc.set_primitives_array(prim_array);
		gc.draw_primitives_array_instanced(cl_triangles, 0, vbo_size, instance_count);
		gc.reset_primitives_array();
	}

	gc.reset_texture(0);
}
