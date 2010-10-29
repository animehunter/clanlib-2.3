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
#include "timing.h"

// The start of the Application
int Timing::start(const std::vector<CL_String> &args)
{
	quit = false;

	// Set the window
	CL_DisplayWindowDescription desc;
	desc.set_title("ClanLib Timing Example");
	desc.set_size(CL_Size(800, 600), true);
	desc.set_allow_resize(true);

	CL_DisplayWindow window(desc);

	// Connect the Window close event
	CL_Slot slot_quit = window.sig_window_close().connect(this, &Timing::on_window_close);

	// Connect a keyboard handler to on_key_up()
	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &Timing::on_input_up);

	// Get the graphic context
	CL_GraphicContext gc = window.get_gc();

	set_stars(gc, 100);

	unsigned int last_time = CL_System::get_time();

	// Run until someone presses escape
	while (!quit)
	{
		// Get the time delta to control movement
		unsigned int current_time = CL_System::get_time();
		float time_delta_ms = (float) (current_time - last_time);
		last_time = current_time;

		gc.clear(CL_Colorf(0.0f,0.0f,0.0f));

		draw_graphics(gc, time_delta_ms);

		window.flip(1);

		CL_KeepAlive::process(0);

		// Sleep to give other processes more time
		current_time = CL_System::get_time();
		const int main_loop_rate = 10;	// 10 ms (100 hz)
		int time_to_sleep_for = main_loop_rate - (current_time - last_time);
		if (time_to_sleep_for > 0)
			CL_System::sleep(time_to_sleep_for);

	}

	return 0;
}

// A key was pressed
void Timing::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
	{
		quit = true;
	}
}

// The window was closed
void Timing::on_window_close()
{
	quit = true;
}

void Timing::draw_graphics(CL_GraphicContext &gc, float time_delta_ms)
{
	int gc_width = gc.get_width();
	int gc_height = gc.get_height();

	std::vector<Star>::size_type max, cnt;
	max = stars.size();
	for (cnt=0; cnt<max; cnt++)
	{
		float xpos = stars[cnt].xpos;
		xpos += time_delta_ms * stars[cnt].speed;
		if (xpos >= gc_width)
			xpos -= (gc_width + 8);
		stars[cnt].xpos = xpos;

		CL_Draw::circle(gc, xpos, stars[cnt].ypos, 6.0f, stars[cnt].color);
	}
}

void Timing::set_stars(CL_GraphicContext &gc, int star_cnt)
{
	stars.clear();
	stars.resize(star_cnt);
	unsigned int random = 1231;
	int gc_width = gc.get_width();
	int gc_height = gc.get_height();

	for (int cnt=0; cnt < star_cnt; cnt++)
	{
		stars[cnt].xpos = (float) (random % gc_width);
		random+= 143222321;
		stars[cnt].ypos = (float) (random % gc_height);
		random+= 89079086;
		stars[cnt].speed = (((float) (random % 256)) / 1024.0f) + 0.01f;
		random*= 595443965;
		stars[cnt].color.r = (((float) (random % 256)) / 256.0f);
		random*= 196243625;
		stars[cnt].color.b = (((float) (random % 256)) / 256.0f);
		random*= 14365;
		stars[cnt].color.g = (((float) (random % 256)) / 256.0f);
		stars[cnt].color.a = 1.0f;

	}

}

