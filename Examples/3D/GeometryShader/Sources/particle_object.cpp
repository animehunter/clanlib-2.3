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
#include "particle_object.h"
#include "scene.h"
#include "shader_color_geometry.h"
#include "graphic_store.h"

ParticleObject::ParticleObject(CL_GraphicContext &gc, Scene &scene_owner, SceneObject *parent_object) : SceneObject(scene_owner, parent_object)
{
	std::vector<CL_Vec3f> object_positions;
	std::vector<CL_Vec4f> object_colours;

	object_positions.resize(num_points);
	object_colours.resize(num_points);

	const float scale_x = 0.0015f;
	const float scale_y = 0.0015f;
	const float scale_z = 0.0015f;
	for (int cnt=0; cnt < num_points; cnt++)
	{
		object_positions[cnt] = CL_Vec3f(
			((rand() & 0xFFFFF) - (0xFFFFF/128.0f)) * scale_x,
			((rand() & 0xFFFFF) - (0xFFFFF/128.0f)) * scale_y,
			((rand() & 0xFFFFF) - (0xFFFFF/128.0f)) * scale_z );

		object_colours[cnt] = CL_Vec4f(
			(rand() & 0xFF) / 256.0f,
			(rand() & 0xFF) / 256.0f,
			(rand() & 0xFF) / 256.0f,
			1.0f);
	}

	object_positions_vbo = CL_VertexArrayBuffer(gc, &object_positions[0], sizeof(CL_Vec3f) * object_positions.size());
	object_colours_vbo = CL_VertexArrayBuffer(gc, &object_colours[0], sizeof(CL_Vec4f) * object_colours.size());

}

ParticleObject::~ParticleObject()
{
}

void ParticleObject::Draw(CL_GraphicContext &gc, const CL_Mat4f &current_modelview)
{
	UpdateOrientationMatrix();
	UpdateModelViewMatrix(current_modelview);

	Draw(gc, scene->gs, modelview_matrix);

	std::vector<SceneObject *>::size_type size, cnt;
	size = child_objects.size();
	for (cnt=0; cnt< size; cnt++)
	{
		child_objects[cnt]->Draw(gc, modelview_matrix);
	}
}

void ParticleObject::Draw(CL_GraphicContext &gc, GraphicStore *gs, const CL_Mat4f &modelview_matrix)
{
	gc.set_modelview(modelview_matrix);

	CL_PrimitivesArray prim_array(gc);

	prim_array.set_attributes(0, object_positions_vbo, 3, cl_type_float, (void *) 0);
	prim_array.set_attributes(1, object_colours_vbo, 4, cl_type_float, (void *) 0);

	gs->shader_color_geometry.Use(gc);

	gc.set_texture(0, gs->texture_alpha_ball);
	gc.draw_primitives(cl_points, num_points, prim_array);
	gc.reset_texture(0);
}
