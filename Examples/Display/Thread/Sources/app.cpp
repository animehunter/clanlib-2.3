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
*/

#include "precomp.h"
#include "app.h"
#include "framerate_counter.h"

int App::start(const std::vector<CL_String> &args)
{
	CL_OpenGLWindowDescription description;
	description.set_title("Thread Example");
	description.set_size(CL_Size(1024, 768), true);

	CL_DisplayWindow window(description);
	CL_InputDevice keyboard = window.get_ic().get_keyboard();
	CL_GraphicContext gc = window.get_gc();

	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

	CL_Slot slot_window_close = window.sig_window_close().connect(this, &App::window_close);

	// Create texture
	texture_1 = CL_Texture(gc, texture_size, texture_size);
	texture_2 = CL_Texture(gc, texture_size, texture_size);

	// Load the font
	CL_Font font(gc, "tahoma", 32);

	// Load and link shaders
	shader = CL_ProgramObject::load(gc, "Resources/vertex_shader.glsl", "Resources/fragment_shader.glsl");
	shader.bind_attribute_location(0, "Position");
	shader.bind_attribute_location(2, "TexCoord0");
	if (!shader.link())
		throw CL_Exception("Unable to link shader program: Error:" + shader.get_info_log());

	quit = false;
	crashed_flag = false;

	// Prepare thread
	working_framebuffer = 1;
	CL_MutexSection mutex_section(&working_framebuffer_mutex, false);

	// We require a try block, so the worker thread exits correctly
	CL_Thread thread;
	try
	{
		CL_GraphicContext worker_gc = gc.create_worker_gc();
		thread.start(this, &App::worker_thread, worker_gc);

		// Main loop
		FramerateCounter framerate_counter;
		unsigned int last_time = CL_System::get_time();

		float angle = 0.0f;

		while (!quit)
		{
			framerate_counter.frame_shown();

			// Calculate timings
			unsigned int current_time = CL_System::get_time();
			float time_delta_ms = (float) (current_time - last_time);
			last_time = current_time;

			angle += time_delta_ms / 100.0f;
			while(angle > 360.0f)
				angle-=360.0f;

			gc.clear();
			
			// Draw rotating mandelbrot
			gc.push_modelview();
			gc.mult_translate(gc.get_width()/2, gc.get_height()/2);
			gc.mult_rotate(CL_Angle(angle, cl_degrees));
			mutex_section.lock();
			if (working_framebuffer == 1)
			{
				gc.set_texture(0, texture_2);
			}
			else
			{
				gc.set_texture(0, texture_1);
			}

			CL_Rectf dest_position(-texture_size/2, -texture_size/2, CL_Sizef(texture_size, texture_size));
			CL_Draw::texture(gc,dest_position, CL_Colorf::white, CL_Rectf(0.0f, 0.0f, 1.0f, 1.0f));
			gc.reset_texture(0);
			mutex_section.unlock();
			gc.pop_modelview();
		
			// Draw FPS
			CL_String fps(cl_format("%1 fps", framerate_counter.get_framerate()));
			font.draw_text(gc, 16, gc.get_height()-16-2, fps, CL_Colorf(1.0f, 0.0f, 0.0f, 1.0f));

			// Draw worker thread crashed message
			if (crashed_flag)
				font.draw_text(gc, 16, 32, "WORKER THREAD CRASHED");
	
			window.flip(1);

			CL_KeepAlive::process();
		}
		gc.clear();
		font.draw_text(gc, 32, 32, "Waiting for worker thread to exit");
		window.flip(0);

		thread.join();
	}
	catch(CL_Exception &exception)
	{
		quit = true;
		thread.join();
		throw exception;
	}

	return 0;
}


void App::window_close()
{
	quit = true;
}

void App::render_mandelbrot(CL_GraphicContext &gc, CL_ProgramObject &program_object)
{

	gc.set_program_object(program_object, cl_program_matrix_modelview_projection);

	float x0 = -0.639;
	float y0 = 0.41;
	float size = 1.0f;

	//x0 *= scale/2.0f;
	//y0 *= scale/2.0f;
	size *= scale;


	draw_texture(gc, CL_Rectf(0,0,gc.get_width(),gc.get_height()), CL_Colorf::white, CL_Rectf(x0, y0, CL_Sizef(size, size)));
	gc.reset_program_object();

}

void App::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
	{
		quit = true;
	}

}

void App::draw_texture(CL_GraphicContext &gc, const CL_Rectf &rect, const CL_Colorf &color, const CL_Rectf &texture_unit1_coords)
{
	CL_Vec2f positions[6] =
	{
		CL_Vec2f(rect.left, rect.top),
		CL_Vec2f(rect.right, rect.top),
		CL_Vec2f(rect.left, rect.bottom),
		CL_Vec2f(rect.right, rect.top),
		CL_Vec2f(rect.left, rect.bottom),
		CL_Vec2f(rect.right, rect.bottom)
	};

	CL_Vec2f tex1_coords[6] =
	{
		CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.top),
		CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.top),
		CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.bottom),
		CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.top),
		CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.bottom),
		CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.bottom)
	};

	CL_PrimitivesArray prim_array(gc);
	prim_array.set_attributes(0, positions);
	prim_array.set_attribute(1, color);
	prim_array.set_attributes(2, tex1_coords);
	gc.draw_primitives(cl_triangles, 6, prim_array);
}

void App::worker_thread(CL_GraphicContext gc)
{
	CL_SharedGCData::add_ref();

	try
	{
		CL_MutexSection mutex_section(&working_framebuffer_mutex, false);


		// Create framebuffer
		CL_FrameBuffer framebuffer_1(gc);
		framebuffer_1.attach_color_buffer(0, texture_1);
	
		CL_FrameBuffer framebuffer_2(gc);
		framebuffer_2.attach_color_buffer(0, texture_2);

		// Load the font
		CL_Font font(gc, "tahoma", 64);

		// Clear the framebuffers
		gc.set_frame_buffer(framebuffer_1);
		gc.clear();
		font.draw_text(gc, 32, 96, "Calculating...");

		gc.set_frame_buffer(framebuffer_2);
		gc.clear();
		font.draw_text(gc, 32, 96, "Calculating...");
		gc.reset_frame_buffer();

		unsigned int last_time = CL_System::get_time();

		while(!quit)
		{
			// Render mandelbrot
			if (working_framebuffer == 1)
			{
				gc.set_frame_buffer(framebuffer_1);
			}
			else
			{
				gc.set_frame_buffer(framebuffer_2);
			}

			unsigned int current_time = CL_System::get_time();
			float time_delta_ms = (float) (current_time - last_time);
			last_time = current_time;

			scale -= time_delta_ms / 1000.0f;
			if (scale <= 0.1f)
				scale = 2.0f;

			render_mandelbrot(gc, shader);
			gc.reset_frame_buffer();

			// Swap the working framebuffers
			mutex_section.lock();
			if (working_framebuffer == 1)
			{
				working_framebuffer = 2;
			}
			else
			{
				working_framebuffer = 1;
			}
			mutex_section.unlock();

			//throw CL_Exception("Bang!");	// <--- Use this to test the application handles exceptions in threads
		}
	}
	catch(CL_Exception &exception)
	{
		crashed_flag = true;
	}

	CL_SharedGCData::release_ref();

}

