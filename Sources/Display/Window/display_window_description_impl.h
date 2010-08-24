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
**    Harry Storbacka
**    Kenneth Gangstoe
*/

#pragma once

#include "API/Core/Text/string_types.h"
#include "API/Display/Window/display_window.h"
#include <map>
#include "API/Core/Math/rect.h"

class CL_DisplayWindowDescription_Impl
{
/// \name Construction
/// \{
public:
	CL_DisplayWindowDescription_Impl()
	{
		title = "Untitled Clanlib Window";
		position = CL_Rect(-1, -1, 639, 479);
		position_client_area = false;
		decorations = true;
		caption = true;
		topmost = false;
		visible = true;
		minimize_button = true;
		maximize_button = true;
		border = false;
		sysmenu = true;
		drop_shadow = false;
		tool_window = false;
		fullscreen = false;
		fullscreen_monitor = 0;
		flipping_buffers = 2;
		swap_interval = -1;
		allow_resize = false;
		refresh_rate = 0;
		bpp = 0;
		create_tablet_context = false;
		layered = false;
		#ifdef WIN32
		handle = 0;
		#endif
		depth_size = 0;
		stencil_size = 0;
		use_cached_window = false;
	}

	virtual ~CL_DisplayWindowDescription_Impl() { return; }

/// \}
/// \name Attributes
/// \{
public:
	CL_String title;
	CL_Rect position;
	bool position_client_area;
	bool decorations;
	bool caption;
	bool minimize_button;
	bool maximize_button;
	bool sysmenu;
	bool border;
	bool topmost;
	bool visible;
	bool drop_shadow;
	bool tool_window;
	bool fullscreen;
	int fullscreen_monitor;
	bool allow_resize;
	int flipping_buffers;
	int swap_interval;
	int bpp;
	int refresh_rate;
	CL_DisplayWindow owner;
	bool create_tablet_context;
	bool use_cached_window;
	bool layered;
#ifdef WIN32
	HWND handle;
#endif
	std::map<CL_String, CL_UnknownSharedPtr> data_objects;
	int depth_size;
	int stencil_size;
/// \}
};
