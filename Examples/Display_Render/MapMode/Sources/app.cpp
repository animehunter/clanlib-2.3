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
**    Mark Page
*/

#include "precomp.h"
#include "app.h"
#include "options.h"
#include <cstdlib>

App::App() : quit(false)
{
}

// The start of the Application
int App::start(const std::vector<CL_String> &args)
{
	CL_DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("MapMode Example");
	win_desc.set_size(CL_Size( 800, 480 ), false);

	CL_DisplayWindow window(win_desc);
	CL_Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);
	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

	CL_String theme;
	if (CL_FileHelp::file_exists("../../../Resources/GUIThemeAero/theme.css"))
		theme = "../../../Resources/GUIThemeAero";
	else if (CL_FileHelp::file_exists("../../../Resources/GUIThemeBasic/theme.css"))
		theme = "../../../Resources/GUIThemeBasic";
	else
		throw CL_Exception("No themes found");

	CL_GUIWindowManagerTexture wm(window);
	CL_GUIManager gui(wm, theme);

	wm.func_repaint().set(this, &App::wm_repaint);

	CL_GraphicContext gc = window.get_gc();

	// Deleted automatically by the GUI
	Options *options = new Options(gui, CL_Rect(0, 0, gc.get_size()));

	CL_Image image_grid(gc, "../Blend/Resources/grid.png");
	CL_Image image_ball(gc, "../Blend/Resources/ball.png");
	float grid_width = (float) image_grid.get_width();
	float grid_height = (float) image_grid.get_height();

	grid_space = (float) (image_grid.get_width() - image_ball.get_width());

	setup_balls();

	options->request_repaint();

	unsigned int time_last = CL_System::get_time();

	while (!quit)
	{
		unsigned int time_now = CL_System::get_time();
		float time_diff = (float) (time_now - time_last);
		time_last = time_now;

		gui.exec(false);
		wm.draw_windows(gc);

		int num_balls = options->num_balls;
		if (num_balls > max_balls)
			num_balls = max_balls;

		if (options->is_moveballs_set)
			move_balls(time_diff, num_balls);

		gc.set_map_mode(options->current_mapmode);

		const float grid_xpos = 10.0f;
		const float grid_ypos = 10.0f;

		if (options->current_mapmode == cl_user_projection)
		{
			CL_Sizef area_size(grid_width + (grid_xpos * 2.0f), grid_height + (grid_ypos * 2.0f));
			set_user_projection(gc, area_size, options);
		}

		// Draw the grid
		image_grid.draw(gc, grid_xpos, grid_ypos);

		gc.flush_batcher();	// <--- Fix me, this should not be required for cl_user_projection

		for (int cnt=0; cnt<num_balls; cnt++)
		{
			image_ball.draw(gc, grid_xpos + balls[cnt].xpos, grid_ypos + balls[cnt].ypos);
		}

		gc.set_modelview(CL_Mat4f::identity());
		gc.set_projection(CL_Mat4f::identity());

		gc.set_map_mode(cl_map_2d_upper_left);

		window.flip(1);
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

void App::setup_balls()
{
	for (int cnt=0; cnt<max_balls; cnt++)
	{
		balls[cnt].xpos = (float) (rand() % (int) grid_space);
		balls[cnt].ypos = (float) (rand() % (int) grid_space);
		balls[cnt].xspeed = (float) ((rand() & 0xFF)+0x7F);
		balls[cnt].yspeed = (float) ((rand() & 0xFF)+0x7F);
		balls[cnt].xdir = ((rand() & 1) == 0);
		balls[cnt].ydir = ((rand() & 1) == 0);
	}
}

void App::move_balls(float time_diff, int num_balls)
{
	for (int cnt=0; cnt<num_balls; cnt++)
	{
		float xdisp = (balls[cnt].xspeed * time_diff) / 5000.0f;
		float ydisp = (balls[cnt].yspeed * time_diff) / 5000.0f;

		if (balls[cnt].xdir)
		{
			balls[cnt].xpos += xdisp;
			if (balls[cnt].xpos >= (float) grid_space)
			{
				balls[cnt].xpos = grid_space;
				balls[cnt].xdir = false;
			}
		}
		else
		{
			balls[cnt].xpos -= xdisp;
			if (balls[cnt].xpos <= 0.0f)
			{
				balls[cnt].xpos = 0.0f;
				balls[cnt].xdir = true;
			}
		}

		if (balls[cnt].ydir)
		{
			balls[cnt].ypos += ydisp;
			if (balls[cnt].ypos >= (float) grid_space)
			{
				balls[cnt].ypos = grid_space;
				balls[cnt].ydir = false;
			}
		}
		else
		{
			balls[cnt].ypos -= ydisp;
			if (balls[cnt].ypos <= 0.0f)
			{
				balls[cnt].ypos = 0.0f;
				balls[cnt].ydir = true;
			}
		}
	}
}

void App::wm_repaint()
{
}

void App::set_user_projection(CL_GraphicContext &gc, CL_Sizef &area_size, Options *options)
{
	gc.set_viewport(CL_Rectf(0, 0, area_size));

	float lens_zoom = 3.2f;
	float lens_near = 0.1f;
	float lens_far = 10000.0f;
	float lens_aspect = 1.0f;

	float fov = 2.0f * atan2(1.0f, lens_zoom);
	float aspect = 1.0f;

	aspect = ( area_size.width * lens_aspect) / area_size.height;

	fov = (fov * 180.0f) / CL_PI;
	CL_Mat4f projection_matrix = CL_Mat4f::perspective( fov, aspect, lens_near, lens_far);
	gc.set_projection(projection_matrix);

	CL_Mat4f modelview_matrix = CL_Mat4f::identity();

	modelview_matrix.scale_self(1.0f, 1.0f, -1.0f);	// So positive Z goes into the screen
	modelview_matrix.translate_self(-1.0f, 1.0, lens_zoom);
	modelview_matrix = modelview_matrix.multiply(CL_Mat4f::rotate(CL_Angle((float) -options->grid_angle, cl_degrees), 1.0f, 0.0f, 0.0f, false));
	modelview_matrix.scale_self(2.0f / area_size.width, -2.0f / area_size.height, 1.0f);
	modelview_matrix.translate_self(cl_pixelcenter_constant,cl_pixelcenter_constant, 0.0f);

	gc.set_modelview(modelview_matrix);
}
