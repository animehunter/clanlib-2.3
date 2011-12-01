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
**    Magnus Norddahl
**    Harry Storbacka
**    Mark Page
*/

#include "Display/precomp.h"
#include "API/Core/Math/rect.h"
#include "API/Core/Math/point.h"
#include "API/Core/Text/logger.h"
#include "API/Display/Window/display_window_description.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/display.h"
#include "API/Display/display_target.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Window/keys.h"
#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Core/System/event_provider.h"
#include "x11_window.h"
#include "input_device_provider_x11keyboard.h"
#include "input_device_provider_x11mouse.h"
#ifdef HAVE_LINUX_JOYSTICK_H
#include "input_device_provider_linuxjoystick.h"
#endif
#include "display_message_queue_x11.h"
#include <X11/Xatom.h>
#include <cstdio>
#include "../Window/input_context_impl.h"
#include <X11/XKBlib.h>
#include <dlfcn.h>

#ifndef MWM_HINTS_FUNCTIONS
/* bit definitions for MwmHints.flags */
#define MWM_HINTS_FUNCTIONS	(1L << 0)
#define MWM_HINTS_DECORATIONS	(1L << 1)
#define MWM_HINTS_INPUT_MODE	(1L << 2)
#define MWM_HINTS_STATUS	(1L << 3)
#endif

#ifndef MWM_FUNC_ALL
/* bit definitions for MwmHints.functions */
#define MWM_FUNC_ALL		(1L << 0)
#define MWM_FUNC_RESIZE		(1L << 1)
#define MWM_FUNC_MOVE		(1L << 2)
#define MWM_FUNC_MINIMIZE	(1L << 3)
#define MWM_FUNC_MAXIMIZE	(1L << 4)
#define MWM_FUNC_CLOSE		(1L << 5)
#endif

#ifndef MWM_DECOR_ALL
/* bit definitions for MwmHints.decorations */
#define MWM_DECOR_ALL		(1L << 0)
#define MWM_DECOR_BORDER	(1L << 1)
#define MWM_DECOR_RESIZEH	(1L << 2)
#define MWM_DECOR_TITLE		(1L << 3)
#define MWM_DECOR_MENU		(1L << 4)
#define MWM_DECOR_MINIMIZE	(1L << 5)
#define MWM_DECOR_MAXIMIZE	(1L << 6)
#endif

#ifndef _NET_WM_STATE_REMOVE
#define _NET_WM_STATE_REMOVE  0
#define _NET_WM_STATE_ADD     1
#define _NET_WM_STATE_TOGGLE  2
#endif

#ifndef KDE_noDecoration
#define KDE_noDecoration              0
#define KDE_normalDecoration          1
#define KDE_tinyDecoration            2
#endif

// From Xm/MwmUtil.h
typedef struct
{
    // These were "int", but only work at "long". Is this okay on a 32bit platform?
    long         flags;
    long	 functions;
    long	 decorations;
    long	 input_mode;
    long	 status;
} ClanLib_MotifWmHints;

CL_X11Window::CL_X11Window()
: window(0), color_map(0), display(0), system_cursor(0), hidden_cursor(0), cursor_bitmap(0), size_hints(NULL),
  minimized(false), maximized(false), restore_to_maximized(false), fullscreen(false),
  wm_protocols(None), wm_delete_window(None), wm_state(None), motif_wm_hints(None), net_wm_state(None), net_wm_state_maximized_vert(None), 
  net_wm_state_maximized_horz(None), net_wm_state_hidden(None), net_wm_state_fullscreen(None), kwm_win_decoration(None), win_hints(None),
  net_wm_ping(None), net_frame_extents(None),
  is_window_mapped(false),
  site(0), clipboard(this), dlopen_lib_handle(NULL),
  always_send_window_position_changed_event(false), always_send_window_size_changed_event(false)

{
	last_repaint_rect.reserve(32);
	keyboard = CL_InputDevice(new CL_InputDeviceProvider_X11Keyboard(this));
	mouse = CL_InputDevice(new CL_InputDeviceProvider_X11Mouse(this));

	CL_DisplayMessageQueue_X11::message_queue.add_client(this);
}

CL_X11Window::~CL_X11Window()
{
	CL_DisplayMessageQueue_X11::message_queue.remove_client(this);
	CL_DisplayMessageQueue_X11::message_queue.set_mouse_capture(this, false);

	ic.dispose();

	get_keyboard()->dispose();
	get_mouse()->dispose();

	for (size_t i = 0; i < joysticks.size(); i++)
		joysticks[i].get_provider()->dispose();

	close_window();

	if (display)
	{
		XCloseDisplay(display);
		display = 0;
	}

	// This MUST be called after XCloseDisplay - It is used for http://www.xfree86.org/4.8.0/DRI11.html
	if (dlopen_lib_handle)
	{
		dlclose(dlopen_lib_handle);
	}

}

void CL_X11Window::open_screen()
{
	if (!display)
	{
		display = XOpenDisplay(NULL);
		if (!display)
			throw CL_Exception("Could not open X11 display!");
	}
}

void CL_X11Window::create(XVisualInfo *visual, CL_DisplayWindowSite *new_site, const CL_DisplayWindowDescription &desc)
{
	site = new_site;

	open_screen();
	close_window();		// Reset all variables
	
	current_screen = visual->screen;
	color_map = XCreateColormap( display, RootWindow(display,  visual->screen), visual->visual, AllocNone);

	memset(&attributes, 0, sizeof(attributes));

	attributes.colormap = color_map;
	attributes.border_pixel = 0;
	attributes.override_redirect = False;

	attributes.event_mask =
		ExposureMask |
		KeyPressMask |
		KeyReleaseMask |
		ButtonPressMask |
		ButtonReleaseMask |
		StructureNotifyMask |
		PropertyChangeMask |
		PointerMotionMask |
		EnterWindowMask |
		LeaveWindowMask |
		KeymapStateMask |
		FocusChangeMask;

	// retrieve some useful atoms
	motif_wm_hints = XInternAtom(display, "_MOTIF_WM_HINTS", True);
	wm_protocols = XInternAtom(display, "WM_PROTOCOLS", True);
	wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", True);
	net_wm_ping = XInternAtom(display, "_NET_WM_PING", True);
	wm_state = XInternAtom(display, "WM_STATE", True);
	net_wm_state = XInternAtom(display, "_NET_WM_STATE", True);
	net_wm_state_maximized_vert = XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_VERT", True);
	net_wm_state_maximized_horz = XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_HORZ", True);
	net_wm_state_hidden = XInternAtom(display, "_NET_WM_STATE_HIDDEN", True);
	net_wm_state_fullscreen = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", True);
	net_frame_extents = XInternAtom(display, "_NET_FRAME_EXTENTS", True);
	kwm_win_decoration = XInternAtom(display, "KWM_WIN_DECORATION", True);
	win_hints = XInternAtom(display, "_WIN_HINTS", True);
	Atom net_wm_window_type = XInternAtom(display, "_NET_WM_WINDOW_TYPE", True);
	Atom net_wm_window_type_desktop = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DESKTOP", True);
	Atom net_wm_window_type_dock = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DOCK", True);
	Atom net_wm_window_type_toolbar = XInternAtom(display, "_NET_WM_WINDOW_TYPE_TOOLBAR", True);
	Atom net_wm_window_type_menu = XInternAtom(display, "_NET_WM_WINDOW_TYPE_MENU", True);
	Atom net_wm_window_type_utility = XInternAtom(display, "_NET_WM_WINDOW_TYPE_UTILITY", True);
	Atom net_wm_window_type_splash = XInternAtom(display, "_NET_WM_WINDOW_TYPE_SPLASH", True);
	Atom net_wm_window_type_dialog = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DIALOG", True);
	Atom net_wm_window_type_dropdown_menu = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DROPDOWN_MENU", True);
	Atom net_wm_window_type_popup_menu = XInternAtom(display, "_NET_WM_WINDOW_TYPE_POPUP_MENU", True);
	Atom net_wm_window_type_tooltip = XInternAtom(display, "_NET_WM_WINDOW_TYPE_TOOLTIP", True);
	Atom net_wm_window_type_notification = XInternAtom(display, "_NET_WM_WINDOW_TYPE_NOTIFICATION", True);
	Atom net_wm_window_type_combo = XInternAtom(display, "_NET_WM_WINDOW_TYPE_COMBO", True);
	Atom net_wm_window_type_dnd = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DND", True);
	Atom net_wm_window_type_normal = XInternAtom(display, "_NET_WM_WINDOW_TYPE_NORMAL", True);
	Atom kde_net_wm_window_type_override = XInternAtom(display, "_KDE_NET_WM_WINDOW_TYPE_OVERRIDE", True);

	size_hints = XAllocSizeHints();
	if (!size_hints)
		throw CL_Exception("Cannot allocate X11 XSizeHints structure");

	system_cursor = XCreateFontCursor(display, XC_left_ptr);	// This is allowed to fail

	CL_SocketMessage_X11 screen_connection;
	screen_connection.type = CL_EventProvider::type_fd_read;
	screen_connection.handle = ConnectionNumber(display);
	current_window_events.push_back(screen_connection);

	int win_x = desc.get_position().left;
	int win_y = desc.get_position().top;
	int win_width = desc.get_size().width;
	int win_height = desc.get_size().height;

	// Check for valid width and height
	if ( win_width <=0 ) 
		win_width = 128; 

	if ( win_height <=0 ) 
		win_height = 128; 

	if (desc.is_fullscreen())
	{
		win_x = 0;
		win_y = 0;
	}

	if (win_x == -1 && win_y == -1)
	{
		int disp_width = DisplayWidth(display, current_screen);
		int disp_height = DisplayHeight(display, current_screen);
		win_x = (disp_width - win_width)/2 - 1;
		win_y = (disp_height - win_height)/2 - 1;
	}

	window = XCreateWindow(display, RootWindow(display, current_screen),
		win_x, win_y, win_width, win_height, 0,  visual->depth,
		InputOutput,  visual->visual, CWBorderPixel | CWColormap | CWOverrideRedirect | CWEventMask, &attributes);

	if (!window)
		throw CL_Exception("Unable to create the X11 window");

	if (!desc.get_owner().is_null())
	{
		CL_DisplayWindow owner = desc.get_owner();
		XSetTransientForHint(display, window, owner.get_window());
	}

	// Setup the hidden cursor (Maybe this should be done only once when required)
	char data[64]; // 8x8
	memset(data, 0, 64);

	XColor black_color;
	memset(&black_color, 0, sizeof(black_color));

	cursor_bitmap = XCreateBitmapFromData(display, window, data, 8, 8);
	hidden_cursor = XCreatePixmapCursor(display, cursor_bitmap, cursor_bitmap, &black_color, &black_color, 0,0 );

	// set title of window:
	set_title(desc.get_title());

	// Inform the window manager who we are
	char hostname[256];
	if (gethostname(hostname, sizeof(hostname)) > -1)
	{
		hostname[sizeof(hostname)-1] = 0;
		long pid = getpid();
		if (pid > 0)
		{
			Atom atom = XInternAtom(display, "_NET_WM_PID", False);
			if (atom)
			{
				long pid = getpid();
				XChangeProperty(display, window, atom, XA_CARDINAL, 32, PropModeReplace, (unsigned char *) &pid, 1);
			}

			atom = XInternAtom(display, "WM_CLIENT_MACHINE", False);
			if (atom)
			{
				long pid = getpid();
				XChangeProperty(display, window, atom, XA_STRING, 8, PropModeReplace, (unsigned char *) hostname, strlen(hostname));
			}
		}
	}

	// Retrieve the frame size (emulating microsoft windows)
	bool window_is_frameless = false;
	bool window_has_thin_frame = false;
	if ((!desc.has_caption()) || (!desc.get_decorations()))
	{
		if (desc.is_tool_window())
		{
			window_is_frameless = true;
		}
		else
		{
			if (desc.get_allow_resize() || desc.is_dialog())
			{
				window_has_thin_frame = true;
			}
			else
			{	
				window_is_frameless = true;
			}
		}
	}

	if (net_wm_window_type != None)
	{
		Atom decor;
		if (desc.is_dialog())
		{
			decor = net_wm_window_type_dialog;
		}
		else if (desc.is_tool_window())
		{
			decor = net_wm_window_type_toolbar;
		}
		else if (window_is_frameless)
		{
			decor = net_wm_window_type_dropdown_menu;
		}
		else
		{
			decor = net_wm_window_type_normal;
		}

		if (decor)	// Ensure the atom exists
		{
			XChangeProperty(display, window, net_wm_window_type, XA_ATOM, 32, PropModeReplace, (unsigned char *)&decor, 1);
		}

		if (window_is_frameless)	// Attempt frameless windows for KDE 2 and 3, if dropdown menus are not supported
		{
			if (kde_net_wm_window_type_override)
				XChangeProperty(display, window, net_wm_window_type, XA_ATOM, 32, PropModeReplace, (unsigned char *)&kde_net_wm_window_type_override, 1);

		}

	}

	// Do not use an else clause here, because on the net_wm_window_type may not turn off the border
	if ( motif_wm_hints != None )
	{
		long decor = 0;
		if (!window_is_frameless)
		{
			if (window_has_thin_frame)
			{
				decor |= MWM_DECOR_BORDER;
			}
			else
			{
				if (desc.get_allow_resize())
					decor |= MWM_DECOR_RESIZEH;

				if (desc.has_caption() || desc.get_decorations())
					decor |= MWM_DECOR_TITLE | MWM_DECOR_MENU;
				
				if (desc.has_maximize_button())
					decor |= MWM_DECOR_MAXIMIZE;

				if (desc.has_minimize_button())
					decor |= MWM_DECOR_MINIMIZE;
			}
		}

		ClanLib_MotifWmHints hints = { MWM_HINTS_DECORATIONS, 0, decor, 0, 0 };
		XChangeProperty(display, window, motif_wm_hints, XA_CARDINAL, 32, PropModeReplace, (unsigned char *)&hints, 5);

	}
	else if (kwm_win_decoration != None)
	{
		long decor;
		if (window_is_frameless)
		{
			decor = KDE_noDecoration;
		}
		else
		{
			decor = window_has_thin_frame ? KDE_tinyDecoration : -1;
		}

		if (decor >= 0)
		{
			XChangeProperty(display, window, kwm_win_decoration, XA_CARDINAL, 32, PropModeReplace, (unsigned char *)&decor, 1);
		}
	}
	else if (win_hints)
	{
		long decor;
		if (window_is_frameless)
		{
			decor = 0;
		}
		else
		{
			decor = window_has_thin_frame ? 0 : -1;
		}

		if (decor >= 0)
		{
			XChangeProperty(display, window, win_hints, XA_CARDINAL, 32, PropModeReplace, (unsigned char *)&decor, 1);
		}
	}

	resize_enabled = desc.get_allow_resize() || desc.is_fullscreen(); // Fs needs resizable window

	if (resize_enabled)
	{
		minimum_size = CL_Size(32, 32);
		maximum_size = CL_Size(0, 0);	// No maximum size by default
	}
	else
	{
		minimum_size = CL_Size(win_width, win_height);
		maximum_size = CL_Size(win_width, win_height);
	}

	size_hints->x = win_x;
	size_hints->y = win_y;
	size_hints->width       = win_width;
	size_hints->height      = win_height;
	size_hints->base_width  = win_width;
	size_hints->base_height = win_height;
	size_hints->min_width   = minimum_size.width;
	size_hints->min_height  = minimum_size.height;
	size_hints->max_width   = maximum_size.width;
	size_hints->max_height  = maximum_size.height;
	size_hints->width_inc = 1;
	size_hints->height_inc = 1;
	size_hints->win_gravity = NorthWestGravity;
	size_hints->flags       = PSize|PBaseSize|PPosition|PMinSize|PResizeInc|PWinGravity;

	if (!resize_enabled)
	{
		size_hints->flags |= PMaxSize;
	}
	XSetWMNormalHints(display, window, size_hints);

	// subscribe to window manager events
	XSetWMProtocols(display, window, &wm_delete_window, 1);

	// Make auto repeat keys detectable
	Bool supports_detectable_autorepeat;
	XkbSetDetectableAutoRepeat(display,1,&supports_detectable_autorepeat);

	fullscreen = desc.is_fullscreen() && net_wm_state && net_wm_state_fullscreen;
	if (fullscreen)
	{
		Atom atom1 = net_wm_state_fullscreen;
		XChangeProperty(display, window, net_wm_state, XA_ATOM, 32, PropModeReplace, (unsigned char *)&atom1, 1);
	}
	
	// Guess the initial position (until the window is mapped)
	requested_size_contains_frame = !desc.get_position_client_area();
	requested_current_window_client_area = CL_Rect(win_x, win_y, CL_Size(win_width, win_height));
	current_window_client_area = requested_current_window_client_area;

	// Set window visibility
	if (desc.is_visible())
	{
		show(false);
	}
	minimized = is_minimized();
	maximized = is_maximized();
	restore_to_maximized = maximized;

	XSync(display, True);

	// Setup the clipboard
	clipboard.setup();

	// Create input devices for window:
	ic.clear();
	ic.add_keyboard(keyboard);
	ic.add_mouse(mouse);

	// Go looking for joysticks:
	setup_joysticks();
}

void CL_X11Window::calculate_window_frame_size()
{
	if (!net_frame_extents)
		return;

	unsigned long number_items;
	int actual_format;
	Atom actual_type;

	// _NET_FRAME_EXTENTS, left, right, top, bottom, CARDINAL[4]/32
	unsigned char *data = get_property(window, net_frame_extents, &number_items, &actual_format, &actual_type);
	if (data != NULL)
	{
		if (number_items >= 4)
		{
			frame_size_left = ((long *)data)[0];
			frame_size_right = ((long *)data)[1];
			frame_size_top = ((long *)data)[2];
			frame_size_bottom = ((long *)data)[3];
		}
		XFree(data);
	}
}

void CL_X11Window::close_window()
{
	current_window_events.clear();

	if (window)
	{
		XDestroyWindow(display, window);
		window = 0;
	}

	if (system_cursor)
	{
		XFreeCursor(display, system_cursor);
		system_cursor = 0;
	}

	if (hidden_cursor)
	{
		XFreeCursor(display, hidden_cursor);
		hidden_cursor = 0;
	}

	if (cursor_bitmap)
	{
		XFreePixmap(display, cursor_bitmap);
		cursor_bitmap = 0;
	}

	if (color_map)
	{
		XFreeColormap(display, color_map);
		color_map = 0;
	}

	if (size_hints)
	{
		XFree(size_hints);
		size_hints = NULL;
	}

	wm_protocols = None;
	wm_delete_window = None;
	wm_state = None;
	net_wm_ping = None;
	motif_wm_hints = None;
	net_wm_state = None;
	net_wm_state_maximized_vert = None;
	net_wm_state_maximized_horz = None;
	net_wm_state_hidden = None;
	net_wm_state_fullscreen = None;
	net_frame_extents = None;
	kwm_win_decoration = None;
	win_hints = None;

	frame_size_left = 0;
	frame_size_right = 0;
	frame_size_top = 0;
	frame_size_bottom = 0;
	frame_size_calculated = false;

}

void *CL_X11Window::dlopen(const char *filename, int flag)
{
	if (dlopen_lib_handle)
		throw CL_Exception("CL_X11Window::dlopen called twice - This is currently not supported, and is probably a bug!");
	dlopen_lib_handle = ::dlopen(filename, flag);
	return dlopen_lib_handle;
}

CL_Rect CL_X11Window::get_geometry() const
{
	CL_Rect rect = requested_current_window_client_area;
	rect.left -=frame_size_left;
	rect.top -=frame_size_top;
	rect.right += frame_size_right;
	rect.bottom += frame_size_bottom;
	return rect;
}

CL_Rect CL_X11Window::get_viewport() const
{
	return CL_Rect(0, 0, requested_current_window_client_area.get_size());
}

bool CL_X11Window::has_focus() const
{
	Window focus_return;
	int revert_to_return;
	XGetInputFocus(display, &focus_return, &revert_to_return);
	if (focus_return == window) return true;
	return false;
}

bool CL_X11Window::is_minimized() const
{
	unsigned long number_items;
	int actual_format;
	Atom actual_type;

	// first check _NET_WM_STATE property if supported
	if (net_wm_state != None)
		return check_net_wm_state(net_wm_state_hidden);

	// now check WM_STATE property
	if (wm_state != None)
	{
		unsigned char *data = get_property(window, wm_state, &number_items, &actual_format, &actual_type);
		if (data != NULL)
		{
			long state = *(long *)data;
			XFree(data);
			return state == IconicState;
		}
	}

	return false;
}

bool CL_X11Window::is_maximized() const
{
	return check_net_wm_state(net_wm_state_maximized_vert, net_wm_state_maximized_horz);
}

bool CL_X11Window::is_visible() const
{
	XWindowAttributes attr;

	XGetWindowAttributes(display, window, &attr);
	if (attr.map_state == IsViewable) return false;
	return true;
}

CL_Size CL_X11Window::get_minimum_size(bool client_area) const
{
	if (!client_area)
		return minimum_size;
	else
		throw CL_Exception("CL_X11Window::get_minimum_size not implemented for client_area");
}

CL_Size CL_X11Window::get_maximum_size(bool client_area) const
{
	if (!client_area)
		return maximum_size;
	else
		throw CL_Exception("CL_X11Window::get_maximum_size not implemented for client_area");
}

CL_String CL_X11Window::get_title() const
{
	return window_title;
}

CL_Point CL_X11Window::client_to_screen(const CL_Point &client)
{
	CL_Rect view = requested_current_window_client_area;
	return CL_Point(view.left + client.x, view.top + client.y);
}

CL_Point CL_X11Window::screen_to_client(const CL_Point &screen)
{
	CL_Rect view = requested_current_window_client_area;
	return CL_Point(screen.x - view.left, screen.y - view.top);
}

void CL_X11Window::show_system_cursor()
{
	XDefineCursor(display, window, system_cursor);
}

void CL_X11Window::set_cursor(CL_StandardCursor type)
{
	if (system_cursor)
	{
		XFreeCursor(display, system_cursor);
		system_cursor = 0;
	}
	unsigned int index = XC_left_ptr;
	switch (type)
	{
		case cl_cursor_arrow:
			index = XC_left_ptr;
			break;
		case cl_cursor_appstarting:
			index = XC_watch;
			break;
		case cl_cursor_cross:
			index = XC_cross;
			break;
		case cl_cursor_hand:	
			index = XC_hand2;
			break;
		case cl_cursor_ibeam:
			index = XC_xterm;
			break;
		case cl_cursor_size_all:	
			index = XC_fleur;
			break;
		case cl_cursor_size_ns:
			index = XC_double_arrow;
			break;
		case cl_cursor_size_we:
			index = XC_sb_h_double_arrow;
			break;
		case cl_cursor_uparrow:
			index = XC_sb_up_arrow;
			break;
		case cl_cursor_wait:
			index = XC_watch;
			break;
		case cl_cursor_no:
			index = XC_X_cursor;
			break;
		case cl_cursor_size_nesw:
		case cl_cursor_size_nwse:
		default:
			break;
	}
	system_cursor = XCreateFontCursor(display, index);
	XDefineCursor(display, window, system_cursor);
}

void CL_X11Window::hide_system_cursor()
{
	XDefineCursor(display, window, hidden_cursor);
}

void CL_X11Window::set_title(const CL_StringRef &new_title)
{
	window_title = new_title;
	XSetStandardProperties(display, window, new_title.c_str(), new_title.c_str(), None, NULL, 0, NULL);
}

void CL_X11Window::set_position(const CL_Rect &pos, bool client_area)
{
	always_send_window_position_changed_event = true;
	always_send_window_size_changed_event = true;

	if (!frame_size_calculated)	// If the frame size has not yet been calculated, we delay setting the window position until later (when mapped)
	{
		requested_current_window_client_area = pos;
		requested_size_contains_frame = !client_area;
		return;
	}
	if (!resize_enabled)	// If resize has been disabled, we have to temporary enable it
	{
		long user_hints;
		XGetWMNormalHints(display, window, size_hints, &user_hints);
		size_hints->flags &= ~(PMinSize | PMaxSize);
		XSetWMNormalHints(display, window, size_hints);
	}

	int width = pos.get_width();
	int height = pos.get_height();

	if (client_area)
	{
		requested_current_window_client_area = pos;
		XMoveResizeWindow(display, window, pos.left - frame_size_left, pos.top - frame_size_top, width, height);
	}
	else
	{
		width = width - frame_size_left - frame_size_right;
		height = height - frame_size_top - frame_size_bottom;
		requested_current_window_client_area = CL_Rect(pos.left + frame_size_left, pos.top + frame_size_top, CL_Size(width, height));
		XMoveResizeWindow(display, window, pos.left, pos.top, width, height);
	}


	if (!resize_enabled)	// resize has been temporary enabled
	{
		size_hints->min_width   = width;
		size_hints->min_height  = height;
		size_hints->max_width   = width;
		size_hints->max_height  = height;
		size_hints->flags |= PMinSize | PMaxSize;
		XSetWMNormalHints(display, window, size_hints);
	}
}

void CL_X11Window::set_size(int width, int height, bool client_area)
{
	always_send_window_size_changed_event = true;

	if (!frame_size_calculated)	// If the frame size has not yet been calculated, we delay setting the window position until later (when mapped)
	{
		requested_current_window_client_area = CL_Rect(requested_current_window_client_area.left, requested_current_window_client_area.top, CL_Size(width, height));
		requested_size_contains_frame = !client_area;
		return;
	}

	if (!resize_enabled)	// If resize has been disabled, we have to temporary enable it
	{
		long user_hints;
		XGetWMNormalHints(display, window, size_hints, &user_hints);
		size_hints->flags &= ~(PMinSize | PMaxSize);
		XSetWMNormalHints(display, window, size_hints);
	}

	if (client_area)
	{
		requested_current_window_client_area = CL_Rect(requested_current_window_client_area.left, requested_current_window_client_area.top, CL_Size(width, height));  

		XResizeWindow(display, window, width, height);
	}
	else
	{
		width = width - frame_size_left - frame_size_right;
		height = height - frame_size_top - frame_size_bottom;
		if (width < 1)
			width = 1;
		if (height < 1)
			height = 1;

		requested_current_window_client_area = CL_Rect(requested_current_window_client_area.left, requested_current_window_client_area.top, CL_Size(width, height));  

		XResizeWindow(display, window, width, height);
	}

	if (!resize_enabled)	// resize has been temporary enabled
	{
		size_hints->min_width   = width;
		size_hints->min_height  = height;
		size_hints->max_width   = width;
		size_hints->max_height  = height;
		size_hints->flags |= PMinSize | PMaxSize;
		XSetWMNormalHints(display, window, size_hints);
	}

}

void CL_X11Window::set_enabled(bool enable)
{
	// Window's version of set_enabled() calls EnableWindow() which tells the windows API that the window can have input focus if desired. 
	// If you do require it for linux, changing the masks: KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask might work
}

void CL_X11Window::minimize()
{
	if (!is_minimized())
		restore_to_maximized = is_maximized();
	XIconifyWindow(display, window, current_screen);
}

void CL_X11Window::restore()
{
	if (is_minimized())
	{
		if (restore_to_maximized)
		{
			maximize();
		}
		else
		{
			map_window();
		}
	}
	else if (is_maximized())
	{
		modify_net_wm_state(false, net_wm_state_maximized_vert, net_wm_state_maximized_horz);
	}
}

void CL_X11Window::map_window()
{
	clear_structurenotify_events();

	if (!is_window_mapped)
	{
		int result = XMapWindow(display, window);
		if ( (result == BadValue) || (result == BadWindow) )
		{
			throw CL_Exception("Failed to map window");		
		}
		else
		{
			XEvent event;
			do {
				XMaskEvent(display, StructureNotifyMask, &event);
			}while ( (event.type != MapNotify) || (event.xmap.event != window) );

			is_window_mapped = true;
		}
		XSync(display, True);

		if (!frame_size_calculated)
		{
			frame_size_calculated = true;
			calculate_window_frame_size();

			if (!fullscreen)
			{
				// Now we know the frame size, nudge the window into the exact position
				CL_Rect frame_size = requested_current_window_client_area;	// Must copy as set_position() changes this variable, causing all sorts of problems
				set_position(frame_size, !requested_size_contains_frame);
			}
			current_window_client_area = get_screen_position();
			requested_current_window_client_area = current_window_client_area;
		}

		always_send_window_position_changed_event = true;
		always_send_window_size_changed_event = true;
	}
}

void CL_X11Window::unmap_window()
{
	clear_structurenotify_events();
	if (is_window_mapped)
	{
		int result = XUnmapWindow(display, window);
		if ( (result == BadValue) || (result == BadWindow) )
		{
			throw CL_Exception("Failed to unmap window");		
		}
		else
		{
			XEvent event;
			do {
				XMaskEvent(display, StructureNotifyMask, &event);
			}while ( (event.type != UnmapNotify) || (event.xmap.event != window) );

			is_window_mapped = false;
		}
	}
}

void CL_X11Window::maximize()
{
	modify_net_wm_state(true, net_wm_state_maximized_vert, net_wm_state_maximized_horz);

}

void CL_X11Window::show(bool activate)
{
	map_window();
	if (activate) set_enabled(true);

	// Force the window is updated
	//CL_Rect window_rect = get_viewport();
	//set_size(window_rect.get_width(), window_rect.get_height(), true);
}

void CL_X11Window::hide()
{
	set_enabled(false);
	unmap_window();
}

void CL_X11Window::bring_to_front()
{
	XRaiseWindow(display, window);
}

void CL_X11Window::capture_mouse(bool capture)
{
	CL_DisplayMessageQueue_X11::message_queue.set_mouse_capture(this, capture);
}

void CL_X11Window::clear_structurenotify_events()
{
	XEvent event;
	while( XCheckMaskEvent(display, StructureNotifyMask, &event));

}

bool CL_X11Window::check_net_wm_state(Atom atom1, Atom atom2) const
{
	unsigned long number_items;
	int actual_format;
	Atom actual_type;

	// search for atom1 or atom2 in _NET_WM_STATE array
	if (net_wm_state != None)
	{
		unsigned char *data = get_property(window, net_wm_state, &number_items, &actual_format, &actual_type);
		if (data != NULL)
		{
			bool find = false;
			for (unsigned i = 0; i < number_items; ++i)
			{
				long atom = ((long *)data)[i];
				if (atom != None && (atom == atom1 || atom == atom2))
				{
					find = true;
					break;
				}
			}
			XFree(data);
			return find;
		}
	}

	return false;
}

bool CL_X11Window::modify_net_wm_state(bool add, Atom atom1, Atom atom2)
{
	// change _NET_WM_STATE property, see: http://standards.freedesktop.org/wm-spec/wm-spec-latest.html
	if (net_wm_state != None)
	{
		XEvent xev;
		memset(&xev, 0, sizeof(xev));
		xev.xclient.type = ClientMessage;
		xev.xclient.window = window;
		xev.xclient.message_type = net_wm_state;
		xev.xclient.format = 32;
		xev.xclient.data.l[0] = add ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
		xev.xclient.data.l[1] = atom1;
		xev.xclient.data.l[2] = atom2;
		xev.xclient.data.l[3] = None;
		return XSendEvent(display, DefaultRootWindow(display), False, SubstructureRedirectMask | SubstructureNotifyMask, &xev) != 0;
	}

	return false;
}

CL_Rect CL_X11Window::get_screen_position() const
{

	int xpos;
	int ypos;
	unsigned int width;
	unsigned int height;
	Window *children_ptr;
	unsigned int num_child;
	Window temp_window;
	XWindowAttributes attr;

	XLockDisplay(display);

	XGetWindowAttributes(display, window, &attr);

	xpos = attr.x;
	ypos = attr.y;
	width = attr.width;
	height = attr.height;

	Window current_window = window;
	while(true)
	{
		children_ptr = NULL;
		XQueryTree(display, current_window, &temp_window, &current_window, &children_ptr, &num_child);
		if (children_ptr)
			XFree(children_ptr);

		if (!current_window) break;

		XGetWindowAttributes(display, current_window, &attr);
		xpos += attr.x;
		ypos += attr.y;
	}
	XUnlockDisplay(display);

	return (CL_Rect(xpos, ypos, width+xpos, height+ypos));

}

//CL_Rect CL_X11Window::get_client_position() const
//{
//	Window root_window;
//	int x, y;
//	unsigned int width, height;
//	unsigned int depth, border_width;
//	XGetGeometry(display, window, &root_window, &x, &y, &width, &height, &border_width, &depth);
//
//	Window child_return;
//	int newx, newy;
//	XTranslateCoordinates(display, window, root_window, x, y, &newx, &newy, &child_return);
//
//	return CL_Rect(newx, newy, CL_Size(width, height));
//}

void CL_X11Window::get_message(CL_X11Window *mouse_capture_window)
{
	XEvent event;

	std::vector<CL_Rect> exposed_rects;
	CL_Rect largest_exposed_rect;
	CL_Rect rect;

	ic.poll(false);		// Check input devices
	// Dispatch all Xlib events
	while (get_xevent(event))
	{
		bool process_input_context = false;
		switch(event.type)
		{
			//Resize or Move
			case ConfigureNotify:
			{
				// From http://tronche.com/gui/x/icccm/sec-4.html
				// (A client will receive a synthetic ConfigureNotify event that describes the (unchanged) geometry of the window)
				// (The client will not receive a real ConfigureNotify event because no change has actually taken place.)

				rect = current_window_client_area;
				if (event.xany.send_event != 0)
				{
					int bw = event.xconfigure.border_width;
					current_window_client_area = CL_Rect( 
						event.xconfigure.x + bw,
						event.xconfigure.y + bw,
						event.xconfigure.x + bw + event.xconfigure.width,
						event.xconfigure.y + bw + event.xconfigure.height
					);
				}
				else
				{
					current_window_client_area = get_screen_position();
				}

				requested_current_window_client_area = current_window_client_area;

				if (site)
				{
					if ( (rect.left != current_window_client_area.left) || (rect.top != current_window_client_area.top) || always_send_window_position_changed_event )
					{
						always_send_window_position_changed_event = false;
						site->sig_window_moved->invoke();
					}

					if ( (rect.get_width() != current_window_client_area.get_width()) || (rect.get_height() != current_window_client_area.get_height()) || always_send_window_size_changed_event )
					{
						always_send_window_size_changed_event = false;
						if (!site->func_window_resize->is_null())
						{
							site->func_window_resize->invoke(rect);
							// TODO: If rect output is different, update this window rect. Maybe use a  XConfigureRequestEvent?
						}

						if (!callback_on_resized.is_null())
							callback_on_resized.invoke();

						site->sig_resize->invoke(rect.get_width(), rect.get_height());
					}
				}
	
				break;
			}
			case ClientMessage:
				// handle window manager messages
				if (wm_protocols)
				{
					if (event.xclient.message_type == wm_protocols)
					{
						if (wm_delete_window)
						{
							if (event.xclient.data.l[0] == wm_delete_window)
							{
								if (site)
									site->sig_window_close->invoke();
							}
						}
						if (net_wm_ping)
						{
							if (event.xclient.data.l[0] == net_wm_ping)
							{
								XSendEvent( display, RootWindow(display, current_screen), False, SubstructureNotifyMask | SubstructureRedirectMask, &event );
							}
						}
					}
				}
				break;
			case Expose:
				// Repaint notification
				if (!site)
					break;

				if (exposed_rects.empty())	// First call, reserve some additional memory as required
				{
					unsigned int num_exposed = event.xexpose.count;
					exposed_rects.reserve(max_allowable_expose_events);
				}

				if (exposed_rects.size() < max_allowable_expose_events)
				{
					rect = CL_Rect(event.xexpose.x, event.xexpose.y, 
						event.xexpose.x + event.xexpose.width, event.xexpose.y + event.xexpose.height);

					exposed_rects.push_back(rect);

					// For optimisation later on, calculate the largest exposed rect
					if ((largest_exposed_rect.get_width() * largest_exposed_rect.get_height()) < (rect.get_width() * rect.get_height()))
					{
						largest_exposed_rect = rect;
					}
				}
				break;
			case FocusIn:
				if (site)
					site->sig_got_focus->invoke();

				break;
			case FocusOut:
				if (site)
				{
					if (!has_focus())	// For an unknown reason, FocusOut is called when clicking on title bar of window
					{
						site->sig_lost_focus->invoke();
					}
				}
				break;
			case PropertyNotify:
				if (net_wm_state != None)
				{
					if (event.xproperty.atom == net_wm_state && event.xproperty.state == PropertyNewValue)
					{
						if (is_minimized())
						{
							if (!minimized && site != NULL)
								site->sig_window_minimized->invoke();
							minimized = true;
							maximized = false;
						}
						else if (is_maximized())
						{
							if (!maximized && site != NULL)
								site->sig_window_maximized->invoke();
							if (minimized && site != NULL)
							{
								// generate resize events for minimized -> maximized transition
								CL_Rect rect = get_geometry();
								site->sig_window_moved->invoke();
								if (!site->func_window_resize->is_null())
									site->func_window_resize->invoke(rect);
								if (!callback_on_resized.is_null())
									callback_on_resized.invoke();
								site->sig_resize->invoke(rect.get_width(), rect.get_height());
							}
							minimized = false;
							maximized = true;
						}
						else
						{
							if ((minimized || maximized) && site != NULL)
								site->sig_window_restored->invoke();
							minimized = false;
							maximized = false;
						}
					}
				}
				else
				{
					if (event.xproperty.atom == wm_state && event.xproperty.state == PropertyNewValue)
					{
						if (is_minimized())
						{
							if (!minimized && site != NULL)
								site->sig_window_minimized->invoke();
							minimized = true;
						}
						else
						{
							if (minimized && site != NULL)
								site->sig_window_restored->invoke();
							minimized = false;
						}
					}
				}
				break;
			case KeyRelease:
			case KeyPress:
				if (get_keyboard())
				{
					get_keyboard()->received_keyboard_input(event.xkey);
					process_input_context = true;
				}
				break;
			case ButtonPress:
			case ButtonRelease:
				if (mouse_capture_window->get_mouse() && event.xany.send_event==0)
				{
					if (!callback_on_clicked.is_null())
					{
						// This callback is required for GL layered windows
						if (!callback_on_clicked.invoke(event.xbutton))
							break;
					}

					// Adjust to what clanlib client expects
					event.xmotion.x = event.xmotion.x_root - requested_current_window_client_area.left;
					event.xmotion.y = event.xmotion.y_root - requested_current_window_client_area.top;

					if (this != mouse_capture_window)
					{
						CL_Rect this_scr = current_window_client_area;
						CL_Rect capture_scr = mouse_capture_window->current_window_client_area;
	
						event.xbutton.x += this_scr.left - capture_scr.left;
						event.xbutton.y += this_scr.top - capture_scr.top;
					}
					
					mouse_capture_window->get_mouse()->received_mouse_input(event.xbutton);
					process_input_context = true;
				}
				break;
			case MotionNotify:
				if (mouse_capture_window->get_mouse() && event.xany.send_event==0)
				{
					// Adjust to what clanlib client expects
					event.xmotion.x = event.xmotion.x_root - requested_current_window_client_area.left;
					event.xmotion.y = event.xmotion.y_root - requested_current_window_client_area.top;
					if (this != mouse_capture_window)
					{
						CL_Rect this_scr = current_window_client_area;
						CL_Rect capture_scr = mouse_capture_window->current_window_client_area;
	
						event.xmotion.x += this_scr.left - capture_scr.left;
						event.xmotion.y += this_scr.top - capture_scr.top;
					}

					mouse_capture_window->get_mouse()->received_mouse_move(event.xmotion);
					process_input_context = true;
				}

				break;

			case SelectionClear:	// New clipboard selection owner
				clipboard.event_selection_clear(event.xselectionclear);
				break;
			case SelectionNotify:
				clipboard.event_selection_notify();
				break;
			case SelectionRequest:	// Clipboard requests
				clipboard.event_selection_request(event.xselectionrequest);
				break;
		default:
			break;
		}

		if (process_input_context)
		{
			// Immediately dispatch any messages queued (to ensure any later event is adjusted for window geometry or cursor changes)
			ic.process_messages();
			if (ic.is_disposed())
				return;		// Disposed, thefore "this" is invalid, must exit now
		}
	}

	// Send any exposure events, unless they have already been sent
	unsigned int max = exposed_rects.size();

	if (max==1)	// Simple case, a single rect
	{
		site->sig_paint->invoke(largest_exposed_rect);
	}
	else if (max >= max_allowable_expose_events)
	{
		CL_Rect window_rect = get_viewport();
		site->sig_paint->invoke(window_rect);
	}
	else if (max > 1)
	{
		// Send the largest rect first
		site->sig_paint->invoke(largest_exposed_rect);
		for (unsigned int cnt=0; cnt < max; cnt++)
		{
			CL_Rect &rect = exposed_rects[cnt];

			// Rect is the larged rect or is inside the largest rect
			if (largest_exposed_rect.is_inside(rect))
			{
				continue;
			}

			// Search for later larger rects that contain this rect
			bool inner_flag = false;
			for (unsigned int inner_cnt=cnt+1; inner_cnt < max; inner_cnt++)
			{
				if (exposed_rects[inner_cnt].is_inside(rect))
				{
					inner_flag = true;
					break;
				}
			}

			if (!inner_flag)
			{
				site->sig_paint->invoke(rect);
			}
		}
	}
}

// This is called for each window by CL_DisplayMessageQueue_X11 to check for messages
bool CL_X11Window::has_messages()
{
	bool message_flag = false;

	if (XPending(display) > 0)
	{
		message_flag = true;
	}

	if (ic.poll(true))
	{
		message_flag = true;
	}

	return message_flag;
}

void CL_X11Window::setup_joysticks()
{
	for (size_t i = 0; i < joysticks.size(); i++)
		joysticks[i].get_provider()->dispose();
	joysticks.clear();

#ifdef HAVE_LINUX_JOYSTICK_H

	char pathname[128];
	char joydev[128];

	if (access("/dev/input/", R_OK | X_OK) == 0)
	{
		strcpy(joydev, "/dev/input/js%d");
	} 
	else
	{
		strcpy(joydev, "/dev/js%d");
	}

	const int max_joysticks = 16;
	for(int i = 0; i < max_joysticks; ++i)
	{
		snprintf(pathname, sizeof(pathname), joydev, i);
		
		if (access(pathname, R_OK) == 0)
		{
			try
			{
				CL_InputDeviceProvider_LinuxJoystick *joystick_provider = new CL_InputDeviceProvider_LinuxJoystick(this, pathname);
				CL_InputDevice device(joystick_provider);
				joysticks.push_back(device);
				ic.add_joystick(device);

				CL_SocketMessage_X11 joystick_connection;
				joystick_connection.type = CL_EventProvider::type_fd_read;
				joystick_connection.handle = joystick_provider->get_fd();
				current_window_events.push_back(joystick_connection);

			} 
			catch (CL_Exception error)
			{
				cl_log_event("debug", "Joystick Error: %1", error.message);
			}
		}
	}
#endif
}

//void CL_X11Window::set_fullscreen()
//{
//	modify_net_wm_state(true, net_wm_state_fullscreen);
//}

void CL_X11Window::set_clipboard_text(const CL_StringRef &text)
{
	clipboard.set_clipboard_text(text);
}

void CL_X11Window::set_clipboard_image(const CL_PixelBuffer &buf)
{
	throw CL_Exception("Todo: CL_X11Window::set_clipboard_image");
}

CL_String CL_X11Window::get_clipboard_text() const
{
	return clipboard.get_clipboard_text();
}

CL_PixelBuffer CL_X11Window::get_clipboard_image() const
{
	throw CL_Exception("Todo: CL_X11Window::get_clipboard_image");

	return CL_PixelBuffer();
}

bool CL_X11Window::is_clipboard_text_available() const
{
	return clipboard.is_clipboard_text_available();
}

bool CL_X11Window::is_clipboard_image_available() const
{
	return clipboard.is_clipboard_image_available();
}

void CL_X11Window::set_cursor(CL_CursorProvider_X11 *cursor)
{
	//TODO:
}

bool CL_X11Window::get_xevent( XEvent &event, int event_type) const
{
	if (XCheckTypedWindowEvent(display, window, event_type, &event))
	{
		return true;
	}else
	{
		return false;
	}

}

bool CL_X11Window::get_xevent( XEvent &event ) const
{
	if  (XPending(display) > 0)
	{
		XNextEvent(display, &event);
		return true;
	}
	return false;
}

void CL_X11Window::request_repaint( const CL_Rect &cl_rect )
{

	CL_Rect rect = cl_rect;
	CL_Rect window_rect = get_viewport();

	rect.clip(window_rect);

	// Validate rect size (if outside clipping region)
	if ((rect.get_width() <= 0) || (rect.get_height() <= 0) )
		return;

	// Search the repaint list
	unsigned int max = last_repaint_rect.size();
	for (unsigned int cnt=0; cnt < max; cnt++)
	{
		// Ensure request for exposure has not already been made
		if (last_repaint_rect[cnt].is_inside(rect))
		{
			return;
		}

		// Does this esposure completely contain one already been made
		if (rect.is_inside(last_repaint_rect[cnt]))
		{
			last_repaint_rect[cnt] = rect;

			// We can also flag all others as free space to optimise further, but get_messages() indirectly does this (and probably faster)
			return;
		}

	}
	// Message sent in process_queued_events() because expose events need to contain a counter
	last_repaint_rect.push_back(rect);

}

void CL_X11Window::process_queued_events()
{
	unsigned int max = last_repaint_rect.size();
	for (unsigned int cnt=0; cnt < max; cnt++)
	{
		CL_Rect &rect = last_repaint_rect[cnt];

		XEvent event;
		event.xexpose.type = Expose;
		event.xexpose.serial = 0;
		event.xexpose.send_event = True;
		event.xexpose.display = display;
		event.xexpose.window = window;
		event.xexpose.x = rect.left;
		event.xexpose.y = rect.top;	
		event.xexpose.width = rect.get_width();
		event.xexpose.height = rect.get_height();
		event.xexpose.count = (max -1) - cnt;
		XSendEvent( display, window, False, 0, &event );
	}
	last_repaint_rect.clear();
}

void CL_X11Window::set_minimum_size( int width, int height, bool client_area)
{
	minimum_size = CL_Size(width,height);

	long user_hints;
	XGetWMNormalHints(display, window, size_hints, &user_hints);
	size_hints->min_width   = minimum_size.width;
	size_hints->min_height  = minimum_size.height;
	size_hints->flags |= PMinSize;
	XSetWMNormalHints(display, window, size_hints);
}

void CL_X11Window::set_maximum_size( int width, int height, bool client_area)
{
	maximum_size = CL_Size(width,height);

	long user_hints;
	XGetWMNormalHints(display, window, size_hints, &user_hints);
	size_hints->max_width   = maximum_size.width;
	size_hints->max_height  = maximum_size.height;
	size_hints->flags |= PMaxSize;
	XSetWMNormalHints(display, window, size_hints);
}

void CL_X11Window::get_keyboard_modifiers(bool &key_shift, bool &key_alt, bool &key_ctrl) const
{
	if (!get_keyboard())
	{
		key_shift = false;
		key_alt = false;
		key_ctrl = false;
		return;
	}
	return get_keyboard()->get_keyboard_modifiers(key_shift, key_alt, key_ctrl);
}

CL_Point CL_X11Window::get_mouse_position() const
{
	if (!get_mouse())
	{
		return CL_Point();
	}
	return get_mouse()->get_position();
}

// Important: Use XFree() on the returned pointer (if not NULL)
unsigned char *CL_X11Window::get_property(Window use_window, Atom prop, unsigned long *number_items_ptr, int *actual_format_ptr, Atom *actual_type_ptr) const
{
	unsigned long bytes_after;
	int read_bytes = 1024;
	unsigned char *read_data = NULL;
	do
	{
		if(read_data != NULL)
		{
			XFree(read_data);
		}
		int result = XGetWindowProperty(display, use_window, prop, 0, read_bytes, False, AnyPropertyType, actual_type_ptr, actual_format_ptr, number_items_ptr, &bytes_after, &read_data);
		if (result != Success)
		{
			*number_items_ptr = 0;
			*actual_format_ptr = 0;
			*actual_type_ptr = None;
			return NULL;
		}
		read_bytes += bytes_after;
	}while(bytes_after != 0);
	return read_data;
}

const std::vector<CL_SocketMessage_X11> &CL_X11Window::get_window_socket_messages() const
{
	return current_window_events;
}

void CL_X11Window::set_large_icon(const CL_PixelBuffer &image)
{

}

void CL_X11Window::set_small_icon(const CL_PixelBuffer &image)
{

}

CL_InputDeviceProvider_X11Keyboard *CL_X11Window::get_keyboard() const
{
	return static_cast<CL_InputDeviceProvider_X11Keyboard *>(keyboard.get_provider());
}

CL_InputDeviceProvider_X11Mouse *CL_X11Window::get_mouse() const
{
	return static_cast<CL_InputDeviceProvider_X11Mouse *>(mouse.get_provider());
}

