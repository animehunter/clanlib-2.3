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

	// Load the font
	CL_Font font(gc, "tahoma", 32);

	// Create the initial textures
	texture_buffers[0] = CL_Texture(gc, texture_size, texture_size);
	texture_buffers[1] = CL_Texture(gc, texture_size, texture_size);

	// Initially clear the textures, so they are filled with a "Calculating..." message
	CL_FrameBuffer framebuffer(gc);
	framebuffer.attach_color_buffer(0, texture_buffers[0]);
	gc.set_frame_buffer(framebuffer);
	gc.clear();
	font.draw_text(gc, 32, 96, "Calculating...");
	gc.flush_batcher();
	framebuffer.attach_color_buffer(0, texture_buffers[1]);
	gc.set_frame_buffer(framebuffer);
	gc.clear();
	font.draw_text(gc, 32, 96, "Calculating...");
	gc.reset_frame_buffer();

	// Setup the initial texture double buffering variables
	texture_write_buffer_offset = 0;
	texture_write = &texture_buffers[0];
	texture_completed = &texture_buffers[1];

	quit = false;
	crashed_flag = false;

	CL_MutexSection texture_complete_mutex_section(&texture_completed_mutex, false);

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
			texture_complete_mutex_section.lock();
			gc.set_texture(0, *texture_completed);
			CL_Rectf dest_position(-texture_size/2, -texture_size/2, CL_Sizef(texture_size, texture_size));
			CL_Draw::texture(gc,dest_position, CL_Colorf::white, CL_Rectf(0.0f, 0.0f, 1.0f, 1.0f));
			gc.reset_texture(0);
			texture_complete_mutex_section.unlock();
			gc.pop_modelview();
		
			// Draw FPS
			CL_String fps(cl_format("%1 fps", framerate_counter.get_framerate()));
			font.draw_text(gc, 16, gc.get_height()-16-2, fps, CL_Colorf(1.0f, 0.0f, 0.0f, 1.0f));

			// Draw worker thread crashed message
			if (crashed_flag)
				font.draw_text(gc, 16, 32, "WORKER THREAD CRASHED");
	
			window.flip(0);

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

void App::render_mandelbrot(CL_GraphicContext &gc, CL_PixelBuffer &dest)
{
	unsigned char *pptr = (unsigned char *) dest.get_data();

	const int max_iteration = 254;

	for (int ycnt =0; ycnt < texture_size; ycnt++)
	{
		for (int xcnt =0; xcnt < texture_size; xcnt++)
		{
			double x0 = ((double) (xcnt - texture_size/2)) / (double) texture_size;
			double y0 = ((double) (ycnt - texture_size/2)) / (double) texture_size;

			x0 *= scale;
			y0 *= scale;

			x0 += -0.639;
			y0 += 0.41;

			double x = 0.0f;
			double y = 0.0f;

			int iteration = 0;
			while ( (x*x + y*y) <= 4.0 )
			{
				double xtemp = x*x - y*y + x0;
				y = 2*x*y + y0;
				x = xtemp;
				++iteration;
				if ((++iteration) == max_iteration)
					break;
			}

			*(pptr++) = 0xFF;
			*(pptr++) = iteration;
			*(pptr++) = iteration/2;
			*(pptr++) = iteration/2;
		}
	}

}

void App::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
	{
		quit = true;
	}

}

void App::worker_thread(CL_GraphicContext gc)
{
	CL_SharedGCData::add_ref();

	try
	{

		texture_pixels = CL_PixelBuffer(gc, texture_size, texture_size);

		CL_MutexSection texture_complete_mutex_section(&texture_completed_mutex, false);

		// Load the font
		CL_Font font(gc, "tahoma", 16);

		CL_FrameBuffer framebuffer(gc);

		unsigned int last_time = CL_System::get_time();

		while(!quit)
		{
			// Control the loop timing
			unsigned int current_time = CL_System::get_time();
			float time_delta_ms = (float) (current_time - last_time);
			last_time = current_time;

			scale -= scale * time_delta_ms / 1000.0f;
			if (scale <= 0.001f)
				scale = 4.0f;

			// Lock the pixel buffer object
			texture_pixels.lock(cl_access_write_only);
			
			// Double buffer the textures
			texture_complete_mutex_section.lock();
			if (texture_write_buffer_offset == 0)
			{
				texture_write_buffer_offset = 1;
				texture_write = &texture_buffers[1];
				texture_completed = &texture_buffers[0];
			}
			else
			{
				texture_write_buffer_offset = 0;
				texture_write = &texture_buffers[0];
				texture_completed = &texture_buffers[1];
			}
			texture_complete_mutex_section.unlock();

			// Write to texture
			render_mandelbrot(gc, texture_pixels);

			// Unlock the pixel buffer object
			texture_pixels.unlock();

			// Upload the pixel buffer
			texture_write->set_subimage(0, 0, texture_pixels, texture_pixels.get_size());

			// Draw text to the image, to show that we can. (This is optional)
			framebuffer.attach_color_buffer(0, *texture_write);
			gc.set_frame_buffer(framebuffer);
			font.draw_text(gc, 32, 96, CL_StringHelp::float_to_text(scale));
			gc.reset_frame_buffer();


			//throw CL_Exception("Bang!");	// <--- Use this to test the application handles exceptions in threads

			// Sleep to slow down this thread on fast pc's
			current_time = CL_System::get_time();
			const int main_loop_rate = 100;	// 100 ms (10 hz)
			int time_to_sleep_for = main_loop_rate - (current_time - last_time);
			if (time_to_sleep_for > 0)
			{
				CL_System::sleep(time_to_sleep_for);
			}
		}
	}
	catch(CL_Exception &exception)
	{
		crashed_flag = true;
	}

	CL_SharedGCData::release_ref();

}

