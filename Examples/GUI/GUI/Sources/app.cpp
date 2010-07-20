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
#include "GUI.h"

App::App() : quit(false)
{
}

// The start of the Application
int App::start(const std::vector<CL_String> &args)
{
	create_window();

	GUI gui(this);

	while(!quit)
	{
		if (!gui.run())
			break;
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

void App::create_window()
{
	CL_DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("GUI Example Application");
	win_desc.set_size(CL_Size( 1100, 900 ), false);

	CL_DisplayWindow new_window = CL_DisplayWindow(win_desc);	// Don't destroy the window first, so the shared gc data is not lost
	window = new_window;
	window_ptr = &window;
	slot_quit = window.sig_window_close().connect(this, &App::on_window_close);
	slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);
}
