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
*/

#include "Display/precomp.h"
#include "API/Display/Window/input_event.h"
#include "API/Core/Text/string_format.h"
#include "input_device_provider_win32mouse.h"
#include "win32_window.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_Win32Mouse construction:

CL_InputDeviceProvider_Win32Mouse::CL_InputDeviceProvider_Win32Mouse(CL_Win32Window *window)
: sig_provider_event(0), window(window)
{
	for (int i=0; i<32; i++) key_states[i] = false;
}

CL_InputDeviceProvider_Win32Mouse::~CL_InputDeviceProvider_Win32Mouse()
{
	dispose();
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_Win32Mouse attributes:

int CL_InputDeviceProvider_Win32Mouse::get_x() const
{
	throw_if_disposed();
	POINT cursor_pos;
	GetCursorPos(&cursor_pos);

	BOOL res = ScreenToClient(window->get_hwnd(), &cursor_pos);
	if (res == FALSE) return 0;

	return cursor_pos.x;
}

int CL_InputDeviceProvider_Win32Mouse::get_y() const
{
	throw_if_disposed();
	POINT cursor_pos;
	GetCursorPos(&cursor_pos);

	BOOL res = ScreenToClient(window->get_hwnd(), &cursor_pos);
	if (res == FALSE) return 0;

	return cursor_pos.y;
}

bool CL_InputDeviceProvider_Win32Mouse::get_keycode(int keycode) const
{
	throw_if_disposed();
	if (keycode < 0 || keycode >= 32) return false;
	return key_states[keycode];
}

CL_String CL_InputDeviceProvider_Win32Mouse::get_key_name(int id) const
{
	throw_if_disposed();
	switch (id)
	{
	case 0: return "Mouse left";
	case 1: return "Mouse right";
	case 2: return "Mouse middle";
	case 3: return "Mouse wheel up";
	case 4: return "Mouse wheel down";
	}

	return cl_format("Mouse button %1", id);
}

float CL_InputDeviceProvider_Win32Mouse::get_axis(int index) const
{
	throw_if_disposed();
	return 0.0f;
}

CL_String CL_InputDeviceProvider_Win32Mouse::get_name() const
{
	throw_if_disposed();
	return "System Mouse";
}

CL_String CL_InputDeviceProvider_Win32Mouse::get_device_name() const
{
	throw_if_disposed();
	return "System Mouse";
}

int CL_InputDeviceProvider_Win32Mouse::get_axis_count() const
{
	throw_if_disposed();
	return 0;
}

int CL_InputDeviceProvider_Win32Mouse::get_button_count() const
{
	throw_if_disposed();
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_Win32Mouse operations:

void CL_InputDeviceProvider_Win32Mouse::set_position(int x, int y)
{
	throw_if_disposed();
	POINT pt;
	pt.x = x;
	pt.y = y;
	
	ClientToScreen(window->get_hwnd(), &pt);
	SetCursorPos(pt.x, pt.y);
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_Win32Mouse implementation:

void CL_InputDeviceProvider_Win32Mouse::on_dispose()
{
}