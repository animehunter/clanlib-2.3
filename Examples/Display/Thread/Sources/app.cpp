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

	// Create the initial pixelbuffers
#ifdef USE_OPENGL_2
	// Only clanGL supports OpenGL Pixel Buffer Objects -- The #define is in app.h
	pixel_buffers[0] = CL_PixelBuffer(gc, texture_size, texture_size, cl_data_to_gpu, cl_rgba8);
	pixel_buffers[1] = CL_PixelBuffer(gc, texture_size, texture_size, cl_data_to_gpu, cl_rgba8);
#else
	// app.h contains the display target selection #define's
	pixel_buffers[0] = CL_PixelBuffer(texture_size, texture_size, cl_rgba8);
	pixel_buffers[1] = CL_PixelBuffer(texture_size, texture_size, cl_rgba8);
#endif

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

	texture_buffers_offset = 0;
	pixel_buffers_offset = 0;
	worker_thread_complete = false;
	
	texture_write = &texture_buffers[0];
	texture_completed = &texture_buffers[1];
	pixelbuffer_write = &pixel_buffers[0];
	pixelbuffer_completed = &pixel_buffers[1];

	dest_pixels = NULL;
	quit = false;
	crashed_flag = false;

	CL_MutexSection worker_thread_mutex_section(&worker_thread_mutex, false);

	// We require a try block, so the worker thread exits correctly
	CL_Thread thread;
	try
	{
		thread.start(this, &App::worker_thread);

		// Main loop
		FramerateCounter framerate_counter;
		FramerateCounter worker_thread_framerate_counter;
		unsigned int last_time = CL_System::get_time();
		unsigned int last_mandelbrot_time = CL_System::get_time();

		float angle = 0.0f;
		bool worker_thread_started = false;
		bool texture_write_active = false;

		while (!quit)
		{
			framerate_counter.frame_shown();

			// Calculate timings
			unsigned int current_time = CL_System::get_time();
			float time_delta_ms = (float) (current_time - last_time);
			last_time = current_time;

			angle += time_delta_ms / 50.0f;
			while(angle > 360.0f)
				angle-=360.0f;

			gc.clear();
			
			// If the pixel buffer was uploaded on the last frame, double buffer it
			if (texture_write_active)
			{
				texture_write_active = false;
				if (texture_buffers_offset == 0)
				{
					texture_buffers_offset = 1;
					texture_write = &texture_buffers[1];
					texture_completed = &texture_buffers[0];
				}
				else
				{
					texture_buffers_offset = 0;
					texture_write = &texture_buffers[0];
					texture_completed = &texture_buffers[1];
				}
			}

			// Wait for pixel buffer completion
			if ((worker_thread_started == true) && (worker_thread_complete==true))
			{
				worker_thread_mutex_section.lock();

				worker_thread_started = false;
				worker_thread_complete = false;
				pixelbuffer_write->unlock();

				worker_thread_mutex_section.unlock();

				texture_write->set_subimage(0, 0, *pixelbuffer_write, pixelbuffer_write->get_size());
				texture_write_active = true;
				// Note the worker thread will start on the other pixelbuffer straight away, in the next "if" statement
			}

			// Start a new transfer when required
			if ((worker_thread_started == false) && (worker_thread_complete==false))
			{
				worker_thread_framerate_counter.frame_shown();

				worker_thread_mutex_section.lock();
				// Swap the pixelbuffer's
				if (pixel_buffers_offset == 0)
				{
					pixel_buffers_offset = 1;
					pixelbuffer_write = &pixel_buffers[1];
					pixelbuffer_completed = &pixel_buffers[0];
				}
				else
				{
					pixel_buffers_offset = 0;
					pixelbuffer_write = &pixel_buffers[0];
					pixelbuffer_completed = &pixel_buffers[1];
				}

				pixelbuffer_write->lock(cl_access_write_only);
				dest_pixels = (unsigned char *) pixelbuffer_write->get_data();
				worker_thread_started = true;
				worker_thread_complete = false;

				// Adjust the mandelbrot scale
				float mandelbrot_time_delta_ms = (float) (current_time - last_mandelbrot_time);
				last_mandelbrot_time = current_time;
				scale -= scale * mandelbrot_time_delta_ms / 1000.0f;
				if (scale <= 0.001f)
					scale = 4.0f;

				worker_thread_mutex_section.unlock();
				worker_thread_activate_event.set();
			}

			// Draw rotating mandelbrot
			gc.push_modelview();
			gc.mult_translate(gc.get_width()/2, gc.get_height()/2);
			gc.mult_rotate(CL_Angle(angle, cl_degrees));
			//gc.mult_scale(2.0f, 2.0f);
			gc.set_texture(0, *texture_completed);
			CL_Rectf dest_position(-texture_size/2, -texture_size/2, CL_Sizef(texture_size, texture_size));
			CL_Draw::texture(gc,dest_position, CL_Colorf::white, CL_Rectf(0.0f, 0.0f, 1.0f, 1.0f));
			gc.reset_texture(0);

			gc.pop_modelview();
		
			// Draw FPS
			CL_String fps;
			fps = CL_String(cl_format("Main Loop %1 fps", framerate_counter.get_framerate()));
			font.draw_text(gc, 16, gc.get_height()-16-2, fps, CL_Colorf(1.0f, 1.0f, 0.0f, 1.0f));
			fps = CL_String(cl_format("Worker Thread %1 fps", worker_thread_framerate_counter.get_framerate()));
			font.draw_text(gc, 16, gc.get_height()-64-2, fps, CL_Colorf(1.0f, 1.0f, 0.0f, 1.0f));

			// Draw worker thread crashed message
			if (crashed_flag)
				font.draw_text(gc, 16, 32, "WORKER THREAD CRASHED");
	
			window.flip(0);

			CL_KeepAlive::process();
		}
		gc.clear();
		font.draw_text(gc, 32, 32, "Waiting for worker thread to exit");
		window.flip(0);
		worker_thread_stop_event.set();
		thread.join();
	}
	catch(CL_Exception &exception)
	{
		worker_thread_stop_event.set();
		thread.join();
		throw exception;
	}

	return 0;
}


void App::window_close()
{
	quit = true;
}

void App::render_mandelbrot(float mandelbrot_scale, unsigned char *dest)
{
	unsigned char *pptr = dest;

	const int max_iteration = 254;

	for (int ycnt =0; ycnt < texture_size; ycnt++)
	{
		for (int xcnt =0; xcnt < texture_size; xcnt++)
		{
			double x0 = ((double) (xcnt - texture_size/2)) / (double) texture_size;
			double y0 = ((double) (ycnt - texture_size/2)) / (double) texture_size;

			x0 *= mandelbrot_scale;
			y0 *= mandelbrot_scale;

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

void App::worker_thread()
{
	try
	{
		
		CL_MutexSection worker_thread_mutex_section(&worker_thread_mutex, false);

		while(true)
		{
		
			int wakeup_reason = CL_Event::wait(worker_thread_activate_event, worker_thread_stop_event);
			if (wakeup_reason != 0)	// Stop event called
				break;
			worker_thread_activate_event.reset();

			// Write to texture
			worker_thread_mutex_section.lock();
			if (dest_pixels)
				render_mandelbrot(scale, dest_pixels);
			worker_thread_mutex_section.unlock();

			//throw CL_Exception("Bang!");	// <--- Use this to test the application handles exceptions in threads

			worker_thread_complete = true;
		}
	}
	catch(CL_Exception &exception)
	{
		crashed_flag = true;
	}


}

