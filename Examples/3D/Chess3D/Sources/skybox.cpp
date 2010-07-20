/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
**    Magnus Norddahl
**    Mark Page
*/
#include "precomp.h"
#include "skybox.h"

Skybox::Skybox(CL_GraphicContext &gc)
{
	CL_PixelBuffer pb_positive_x = CL_ImageProviderFactory::load("Resources/skybox_x_positive.png");
	CL_PixelBuffer pb_negative_x = CL_ImageProviderFactory::load("Resources/skybox_x_negative.png");
	CL_PixelBuffer pb_positive_y = CL_ImageProviderFactory::load("Resources/skybox_y_positive.png");
	CL_PixelBuffer pb_negative_y = CL_ImageProviderFactory::load("Resources/skybox_y_negative.png");
	CL_PixelBuffer pb_positive_z = CL_ImageProviderFactory::load("Resources/skybox_z_positive.png");
	CL_PixelBuffer pb_negative_z = CL_ImageProviderFactory::load("Resources/skybox_z_negative.png");

	CL_VirtualDirectory vd;
	program_object = CL_ProgramObject::load_and_link(gc, "Resources/skybox_vertex.glsl", "Resources/skybox_fragment.glsl", vd);

	skybox_texture = CL_Texture(gc, cl_texture_cube_map);
	skybox_texture.set_cube_map(
		pb_positive_x,
		pb_negative_x,
		pb_positive_y,
		pb_negative_y,
		pb_positive_z,
		pb_negative_z);

	skybox_texture.set_min_filter(cl_filter_linear);
	skybox_texture.set_mag_filter(cl_filter_linear);
	//skybox_texture.generate_mipmap(true);
	//skybox_texture.set_min_filter(cl_filter_linear_mipmap_linear);
}

Skybox::~Skybox()
{
}

void Skybox::render(CL_GraphicContext &gc, const Camera &camera)
{
	Camera cam = camera;
	cam.get_position().set_position(0,0,0);
	cam.setup_gc(gc, 0.1f, 10.0f);

	gc.set_texture(0, skybox_texture);
	gc.set_program_object(program_object);
	program_object.set_uniform1i("texture1", 0);

	CL_PrimitivesArray prim_array(gc);
	prim_array.set_attributes(0, positions);
	gc.draw_primitives(cl_triangles, 6*6, prim_array);

	gc.reset_program_object();
	gc.reset_texture(0);

}

CL_Vec3f Skybox::positions[6*6] =
{
	CL_Vec3f(-0.5f,  0.5f,  0.5f),
	CL_Vec3f( 0.5f,  0.5f,  0.5f),
	CL_Vec3f(-0.5f, -0.5f,  0.5f),
	CL_Vec3f( 0.5f,  0.5f,  0.5f),
	CL_Vec3f( 0.5f, -0.5f,  0.5f),
	CL_Vec3f(-0.5f, -0.5f,  0.5f),

	CL_Vec3f( 0.5f, -0.5f, -0.5f),
	CL_Vec3f( 0.5f,  0.5f, -0.5f),
	CL_Vec3f(-0.5f, -0.5f, -0.5f),
	CL_Vec3f( 0.5f,  0.5f, -0.5f),
	CL_Vec3f(-0.5f,  0.5f, -0.5f),
	CL_Vec3f(-0.5f, -0.5f, -0.5f),

	CL_Vec3f( 0.5f,  0.5f, -0.5f),
	CL_Vec3f( 0.5f,  0.5f,  0.5f),
	CL_Vec3f(-0.5f,  0.5f, -0.5f),
	CL_Vec3f( 0.5f,  0.5f,  0.5f),
	CL_Vec3f(-0.5f,  0.5f,  0.5f),
	CL_Vec3f(-0.5f,  0.5f, -0.5f),

	CL_Vec3f(-0.5f, -0.5f,  0.5f),
	CL_Vec3f( 0.5f, -0.5f,  0.5f),
	CL_Vec3f(-0.5f, -0.5f, -0.5f),
	CL_Vec3f( 0.5f, -0.5f,  0.5f),
	CL_Vec3f( 0.5f, -0.5f, -0.5f),
	CL_Vec3f(-0.5f, -0.5f, -0.5f),

	CL_Vec3f( 0.5f, -0.5f,  0.5f),
	CL_Vec3f( 0.5f,  0.5f,  0.5f),
	CL_Vec3f( 0.5f, -0.5f, -0.5f),
	CL_Vec3f( 0.5f,  0.5f,  0.5f),
	CL_Vec3f( 0.5f,  0.5f, -0.5f),
	CL_Vec3f( 0.5f, -0.5f, -0.5f),

	CL_Vec3f(-0.5f,  0.5f, -0.5f),
	CL_Vec3f(-0.5f,  0.5f,  0.5f),
	CL_Vec3f(-0.5f, -0.5f, -0.5f),
	CL_Vec3f(-0.5f,  0.5f,  0.5f),
	CL_Vec3f(-0.5f, -0.5f,  0.5f),
	CL_Vec3f(-0.5f, -0.5f, -0.5f)
};
