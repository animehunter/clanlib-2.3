
#include "precomp.h"
#include "model_assimp.h"

#if defined(_MSC_VER)
	#if !defined(_DEBUG)
		#if defined(_DLL)
			#pragma comment(lib, "assimp-static-mtdll.lib")
		#else
			#pragma comment(lib, "assimp-static-mt.lib")
		#endif
	#else
		#if defined(_DLL)
			#pragma comment(lib, "assimp-static-mtdll-debug.lib")
		#else
			#pragma comment(lib, "assimp-static-mt-debug.lib")
		#endif
	#endif
#endif


/////////////////////////////////////////////////////////////////////////////
// ModelAssimp Construction:

ModelAssimp::ModelAssimp(CL_GraphicContext &gc, const CL_StringRef &filename) : scene(NULL)
{
	CL_String resource_dir = "Resources/";

	CL_VirtualDirectory vd;
	program_object_texture = CL_ProgramObject::load(gc, resource_dir + "model_3ds_vertex.glsl", resource_dir + "model_3ds_fragment.glsl", vd);
	program_object_texture.bind_attribute_location(0, "in_position");
	program_object_texture.bind_attribute_location(1, "in_normal");
	program_object_texture.bind_attribute_location(2, "in_texcoord");
	if (!program_object_texture.link())
		throw CL_Exception("Unable to link 3ds model program object (texture)");
	program_object_no_texture = CL_ProgramObject::load(gc, resource_dir + "model_3ds_vertex.glsl", resource_dir + "model_3ds_fragment_no_texture.glsl", vd);
	program_object_no_texture.bind_attribute_location(0, "in_position");
	program_object_no_texture.bind_attribute_location(1, "in_normal");
	if (!program_object_no_texture.link())
		throw CL_Exception("Unable to link 3ds model program object (no texture)");

	load(gc, filename);
	load_textures(gc);
}

ModelAssimp::~ModelAssimp()
{
	if (scene)
		aiReleaseImport(scene);
}

/////////////////////////////////////////////////////////////////////////////
// ModelAssimp Attributes:

/////////////////////////////////////////////////////////////////////////////
// ModelAssimp Operations:

void ModelAssimp::render(CL_GraphicContext &gc, const LightModel &light_model, const Position &position, const Orientation &orientation)
{
	CL_Mat4f modelmatrix = CL_Mat4f::identity();
	modelmatrix.multiply(position.to_matrix());
	modelmatrix.multiply(orientation.to_matrix());
	float scale = 1.5f;

	modelmatrix.multiply(CL_Mat4f::rotate(CL_Angle(-90.0f, cl_degrees), 1.0f, 0.0f, 0.0f, false));
	modelmatrix.multiply(CL_Mat4f::scale(scale, scale, scale));
	gc.push_modelview();
	gc.mult_modelview(modelmatrix);

	CL_PolygonRasterizer polygon_rasterizer;
	polygon_rasterizer.set_culled(true);
	polygon_rasterizer.set_face_cull_mode(cl_cull_back);
	polygon_rasterizer.set_front_face(cl_face_side_clockwise);
	gc.set_polygon_rasterizer(polygon_rasterizer);

	Material material;
	setup_light(program_object_texture, material, light_model);

	program_object_texture.set_uniform1i("texture1",0 );

	int current_position_offset = 0;
	int current_texcoord_offset = 0;
	render_vbo(gc, scene, scene->mRootNode, current_position_offset, current_texcoord_offset);

	gc.reset_program_object();
	gc.reset_polygon_rasterizer();

	gc.pop_modelview();

}

/////////////////////////////////////////////////////////////////////////////
// ModelAssimp Implementation:

void ModelAssimp::render_vbo(CL_GraphicContext &gc, const struct aiScene* sc, const struct aiNode* nd, int &current_position_offset, int &current_texcoord_offset)
{
	unsigned int n = 0;

	// All meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n)
	{
		const struct aiMesh* mesh = sc->mMeshes[nd->mMeshes[n]];
		int num_vertex = mesh->mNumFaces * 3;
		if (!num_vertex)
			continue;

		bool use_texcoords = true;
		if (mesh->mTextureCoords == NULL || mesh->mTextureCoords[0] == NULL)
			use_texcoords = false;

		CL_PrimitivesArray prim_array(gc);
		prim_array.set_attributes(0, vbo_positions, 3, cl_type_float, (void *) (current_position_offset * sizeof(CL_Vec3f)));
		prim_array.set_attributes(1, vbo_normals, 3, cl_type_float, (void *) (current_position_offset * sizeof(CL_Vec3f)));

		if (use_texcoords)
		{
			prim_array.set_attributes(2, vbo_texcoords, 2, cl_type_float, (void *) (current_texcoord_offset * sizeof(CL_Vec2f)));
			set_texture(gc, mesh);
			gc.set_program_object(program_object_texture);
		}
		else
		{
			gc.set_program_object(program_object_no_texture);
		}

		gc.draw_primitives(cl_triangles, num_vertex, prim_array);
	

		if (use_texcoords)
		{
				current_texcoord_offset += num_vertex;
				gc.reset_texture(0);
		}

		current_position_offset += num_vertex;
	}

	// All children
	for (n = 0; n < nd->mNumChildren; ++n)
	{
		render_vbo(gc, sc, nd->mChildren[n], current_position_offset, current_texcoord_offset);
	}

}

void ModelAssimp::setup_light(CL_ProgramObject &program, const Material &material, const LightModel &light_model)
{
	program.set_uniform4f("lightSourcePosition", light_model.light_sources[0].position);
	program.set_uniform4f("lightSourceHalfVector", light_model.light_sources[0].get_halfvector());
	program.set_uniform4f("lightSourceDiffuse", light_model.light_sources[0].diffuse);
	program.set_uniform4f("lightSourceAmbient", light_model.light_sources[0].ambient);
	program.set_uniform4f("lightSourceSpecular", light_model.light_sources[0].specular);
	program.set_uniform1f("lightSourceConstantAttenuation", light_model.light_sources[0].constant_attenuation);
	program.set_uniform1f("lightSourceLinearAttenuation", light_model.light_sources[0].constant_attenuation);
	program.set_uniform1f("lightSourceQuadraticAttenuation", light_model.light_sources[0].quadratic_attenuation);
	program.set_uniform4f("frontMaterialDiffuse", material.diffuse);
	program.set_uniform4f("frontMaterialAmbient", material.ambient);
	program.set_uniform4f("frontMaterialSpecular", material.specular);
	program.set_uniform1f("frontMaterialShininess", material.shininess);
	program.set_uniform4f("lightModelAmbient", light_model.scene_ambient);

}

void ModelAssimp::load_textures(CL_GraphicContext &gc)
{
	if (scene->HasTextures())
		throw CL_Exception("Support for meshes with embedded textures is not implemented");

	textures.clear();

	for (unsigned int m=0; m<scene->mNumMaterials; m++)
	{
		int texIndex = 0;
		aiString path;	// filename

		while (true)
		{
			aiReturn texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
			if (texFound != AI_SUCCESS)
				break;
			textures[path.data] = CL_Texture(); //fill map with textures
			texIndex++;
		}
	}

	for (std::map<CL_String, CL_Texture>::iterator itr = textures.begin(); itr != textures.end(); ++itr)
	{
		CL_String filename = cl_format("Resources/%1", CL_PathHelp::get_filename((*itr).first));
		CL_Texture texture(gc, filename);
		texture.set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat);
		texture.set_min_filter(cl_filter_linear);
		texture.set_mag_filter(cl_filter_linear);
		(*itr).second = texture;
	}
}

void ModelAssimp::load(CL_GraphicContext &gc, const CL_StringRef &filename)
{
	if (scene)
		aiReleaseImport(scene);
	scene = aiImportFile(filename.c_str(),aiProcessPreset_TargetRealtime_MaxQuality);
	if (!scene)
		throw CL_Exception("Cannot load a model");

	total_positions_size = 0;
	total_texcoords_size = 0;
	count_vertices(scene, scene->mRootNode, total_positions_size, total_texcoords_size);
	if (!total_positions_size)
		throw CL_Exception("No vertices found in the model");

	vbo_positions = CL_VertexArrayBuffer(gc, total_positions_size * sizeof(CL_Vec3f), cl_usage_static_draw);
	vbo_normals = CL_VertexArrayBuffer(gc, total_positions_size * sizeof(CL_Vec3f), cl_usage_static_draw);
	if (total_texcoords_size)
	{
		vbo_texcoords = CL_VertexArrayBuffer(gc, total_texcoords_size * sizeof(CL_Vec2f), cl_usage_static_draw);
	}
	else
	{
		vbo_texcoords = CL_VertexArrayBuffer();
	}

	int current_position_offset = 0;
	int current_texcoord_offset = 0;
	insert_vbo(scene, scene->mRootNode, current_position_offset, current_texcoord_offset);

}

void ModelAssimp::count_vertices(const struct aiScene* sc, const struct aiNode* nd, int &total_positions_size, int &total_texcoords_size)
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

		if (! (mesh->mTextureCoords == NULL || mesh->mTextureCoords[0] == NULL) )
			total_texcoords_size += num_vertex;

		total_positions_size += num_vertex;
		for (t = 0; t < mesh->mNumFaces; ++t)
		{
			if (mesh->mFaces[t].mNumIndices != 3)
					throw CL_Exception("This example only supports triangles");
		}
	}

	// All children
	for (n = 0; n < nd->mNumChildren; ++n)
	{
		count_vertices(sc, nd->mChildren[n], total_positions_size, total_texcoords_size);
	}
}
void ModelAssimp::insert_vbo(const struct aiScene* sc, const struct aiNode* nd, int &current_position_offset, int &current_texcoord_offset)
{
	unsigned int i;
	unsigned int n = 0, t;

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

		bool use_texcoords = true;
		if (mesh->mTextureCoords == NULL || mesh->mTextureCoords[0] == NULL)
			use_texcoords = false;

		if (use_texcoords)
			tex_coords.reserve(num_vertex);


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

		vbo_positions.upload_data(current_position_offset * sizeof(CL_Vec3f), &vertices[0], num_vertex * sizeof(CL_Vec3f));
		vbo_normals.upload_data(current_position_offset * sizeof(CL_Vec3f), &normals[0], num_vertex * sizeof(CL_Vec3f));
		if (use_texcoords)
		{
			vbo_texcoords.upload_data(current_texcoord_offset * sizeof(CL_Vec2f), &tex_coords[0], num_vertex * sizeof(CL_Vec2f));
			current_texcoord_offset += num_vertex;
		}

		current_position_offset += num_vertex;
	}

	// All children
	for (n = 0; n < nd->mNumChildren; ++n)
	{
		insert_vbo(sc, nd->mChildren[n], current_position_offset, current_texcoord_offset);
	}

}

void ModelAssimp::set_texture(CL_GraphicContext &gc, const struct aiMesh* mesh)
{
	const struct aiMaterial *mtl = scene->mMaterials[mesh->mMaterialIndex];

	aiString path;	// filename
	aiReturn texFound = mtl->GetTexture(aiTextureType_DIFFUSE, 0, &path);
	if (texFound != AI_SUCCESS)
		throw CL_Exception("Cannot find texture filename");

	CL_Texture texture = textures[path.data];
	if (texture.is_null())
		throw CL_Exception("For an unknown reason, the texture was not loaded");

	gc.set_texture(0, texture);
}
