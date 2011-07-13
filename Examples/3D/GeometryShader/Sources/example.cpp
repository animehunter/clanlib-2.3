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
#include "example.h"
#include "scene_object.h"
#include "particle_object.h"
#include "graphic_store.h"
#include "framerate_counter.h"

// The start of the Application
int App::start(const std::vector<CL_String> &args)
{
	quit = false;

	CL_OpenGLWindowDescription desc;
	desc.set_title("ClanLib Geometry Shader Example");
	desc.set_size(CL_Size(900, 700), true);
	desc.set_multisampling(0);
	desc.set_allow_resize(false);
	desc.set_depth_size(16);
	desc.set_version(3, 2, false);

	CL_DisplayWindow window(desc);

	// Connect the Window close event
	CL_Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);

	// Connect a keyboard handler to on_key_up()
	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

	// Get the graphic context
	CL_GraphicContext gc = window.get_gc();

	// Setup graphic store
	GraphicStore graphic_store(gc);
	scene.gs = &graphic_store;

	// Prepare the display
	gc.set_map_mode(cl_user_projection);

	CL_PolygonRasterizer polygon_rasterizer;
	polygon_rasterizer.set_culled(false);
	polygon_rasterizer.set_face_cull_mode(cl_cull_back);
	polygon_rasterizer.set_front_face(cl_face_side_clockwise);
	gc.set_polygon_rasterizer(polygon_rasterizer);

	create_scene(gc);

	camera_angle = 0.0f;

	CL_Font font(gc, "tahoma", 24);

	FramerateCounter framerate_counter;

	unsigned int time_last = CL_System::get_time();
	// Run until someone presses escape
	while (!quit)
	{
		framerate_counter.frame_shown();

		unsigned int time_now = CL_System::get_time();
		time_delta = time_now - time_last;
		time_last = time_now;

		control_camera();
		calculate_matricies(gc);

		gc.set_polygon_rasterizer(polygon_rasterizer);

		gc.clear_depth(1.0f);
		// ** If enabling below, change the graphic from alpha_ball2.png to alpha_ball.png in graphic_store.cpp
		//render_depth_buffer(gc);	// Render to depth buffer first, to fake sorting the particles
		render(gc);	// Render scene

		gc.set_modelview(CL_Mat4f::identity());
		gc.set_map_mode(cl_map_2d_upper_left);

		CL_String fps(cl_format("fps = %1", framerate_counter.get_framerate()));
		font.draw_text(gc, 16-2, gc.get_height()-16-2, fps, CL_Colorf(0.0f, 0.0f, 0.0f, 1.0f));
		font.draw_text(gc, 16, gc.get_height()-16-2, fps, CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f));

		CL_String info(cl_format("Drawing %1 particles", ParticleObject::num_points));
		font.draw_text(gc, 16, 30, info);

		// Use flip(1) to lock the fps
		window.flip(0);

		// This call processes user input and other events
		CL_KeepAlive::process();
	}

	return 0;
}

// A key was pressed
void App::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
	{
		quit = true;
	}
}

// The window was closed
void App::on_window_close()
{
	quit = true;
}

void App::render_depth_buffer(CL_GraphicContext &gc)
{
	gc.set_map_mode(cl_user_projection);
	gc.set_viewport(scene.gs->texture_depth.get_size());

	gc.set_projection(scene.gs->camera_projection);

	CL_BufferControl buffer_control;
	buffer_control.set_depth_compare_function(cl_comparefunc_lequal);
	buffer_control.enable_depth_write(true);
	buffer_control.enable_depth_test(true);
	buffer_control.enable_stencil_test(false);
	buffer_control.enable_color_write(false);
	gc.set_buffer_control(buffer_control);

	CL_Mat4f modelview_matrix = scene.gs->camera_modelview;
	scene.Draw(modelview_matrix, gc);
	gc.reset_program_object();
}

void App::render(CL_GraphicContext &gc)
{
	gc.set_map_mode(cl_user_projection);
	CL_Rect viewport_rect(0, 0, CL_Size(gc.get_width(), gc.get_height()));
	gc.set_viewport(viewport_rect);

	gc.set_projection(scene.gs->camera_projection);

	CL_BufferControl buffer_control;
	buffer_control.set_depth_compare_function(cl_comparefunc_lequal);
	buffer_control.enable_depth_write(true);
	buffer_control.enable_depth_test(true);
	buffer_control.enable_stencil_test(false);
	buffer_control.enable_color_write(true);
	gc.set_buffer_control(buffer_control);

	gc.clear(CL_Colorf(0.0f, 0.0f, 0.0f, 1.0f));

	CL_Mat4f modelview_matrix = scene.gs->camera_modelview;
	scene.Draw(modelview_matrix, gc);
	gc.reset_program_object();
}

void App::create_scene(CL_GraphicContext &gc)
{
	camera = new SceneObject(scene, scene.base);
	camera->position = CL_Vec3f(0.0f, 50.0f, -20.0f);
	camera->rotation_y = CL_Angle(0.0f, cl_degrees);

	object_particles = new ParticleObject(gc, scene, scene.base);
	object_particles->position = CL_Vec3f(-110.0f, 54.0f, -5.0f);
	object_particles->rotation_y = CL_Angle(0.0f, cl_degrees);
	object_particles->rotation_x = CL_Angle(0.0f, cl_degrees);
	object_particles->rotation_z = CL_Angle(0.0f, cl_degrees);
	object_particles->scale = CL_Vec3f(1.0f, 1.0f, 1.0f);
}

void App::calculate_matricies(CL_GraphicContext &gc)
{
	scene.gs->camera_projection = CL_Mat4f::perspective(45.0f, ((float) gc.get_width()) / ((float) gc.get_height()), 0.1f, 1000.0f);

	scene.gs->camera_modelview = CL_Mat4f::identity();
	camera->GetWorldMatrix(scene.gs->camera_modelview);
	scene.gs->camera_modelview.scale_self(1.0f, 1.0f, -1.0f);
	scene.gs->camera_modelview.inverse();

}

void App::control_camera()
{
	camera_angle += ((float) time_delta) / 20.0f;

	if (camera_angle > 360.0f)
	{
		camera_angle -= 360.0f;
	}

	float camera_angle_rad = (camera_angle * CL_PI) / 180.0f;

	float radius = 100.0f;
	float xpos = cos(camera_angle_rad) * radius;
	float zpos = sin(camera_angle_rad) * radius;

	xpos -= 100.0f;

	camera->position = CL_Vec3f(xpos, 100.0f, zpos);

	camera->rotation_x = CL_Angle(20.0f, cl_degrees);
	camera->rotation_y = CL_Angle(-(camera_angle+90.0f), cl_degrees);

}
