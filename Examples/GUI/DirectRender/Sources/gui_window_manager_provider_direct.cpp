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

#include "gui_window_manager_provider_direct.h"
#include "gui_window_manager_direct_window.h"

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerProvider_Direct Construction:

GUIWindowManagerProvider_Direct::GUIWindowManagerProvider_Direct(CL_DisplayWindow &display_window)
: site(0), activated_window(0), capture_mouse_window(NULL), display_window(display_window), 
  setup_painting_called(false), painting_set(false)
{
//	slots.connect(display_window.sig_paint(), this, &GUIWindowManagerProvider_Direct::on_displaywindow_paint);
	slots.connect(display_window.sig_window_close(), this, &GUIWindowManagerProvider_Direct::on_displaywindow_window_close);

	CL_InputContext& ic = display_window.get_ic();
	slots.connect(ic.get_mouse().sig_key_up(), this, &GUIWindowManagerProvider_Direct::on_input_mouse_up);
	slots.connect(ic.get_mouse().sig_key_down(), this, &GUIWindowManagerProvider_Direct::on_input_mouse_down);
	slots.connect(ic.get_mouse().sig_pointer_move(), this, &GUIWindowManagerProvider_Direct::on_input_mouse_move);

	slots.connect(ic.get_keyboard().sig_key_up(), this, &GUIWindowManagerProvider_Direct::on_input);
	slots.connect(ic.get_keyboard().sig_key_down(), this, &GUIWindowManagerProvider_Direct::on_input);

	for (int tc = 0; tc < ic.get_tablet_count(); ++tc)
	{
		slots.connect(ic.get_tablet(tc).sig_axis_move(), this, &GUIWindowManagerProvider_Direct::on_input_mouse_move);
		slots.connect(ic.get_tablet(tc).sig_key_down(), this, &GUIWindowManagerProvider_Direct::on_input_mouse_down);
		slots.connect(ic.get_tablet(tc).sig_key_up(), this, &GUIWindowManagerProvider_Direct::on_input);
	}

}

GUIWindowManagerProvider_Direct::~GUIWindowManagerProvider_Direct()
{
}

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerProvider_Direct Attributes:

CL_GUIWindowManager::CL_WindowManagerType GUIWindowManagerProvider_Direct::get_window_manager_type() const
{
	return CL_GUIWindowManager::cl_wm_type_texture;
}

std::vector<GUIWindowManagerDirectWindow> GUIWindowManagerProvider_Direct::get_windows(bool only_visible) const
{
	std::vector<GUIWindowManagerDirectWindow> windows;

	get_all_windows_zorder(only_visible, windows, root_window_z_order);
	return windows;
}

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerProvider_Direct Operations:

void GUIWindowManagerProvider_Direct::destroy()
{
	delete this;
}

void GUIWindowManagerProvider_Direct::update_paint()
{
	std::map<CL_GUITopLevelWindow *, GUITopLevelWindowDirect *>::iterator it;
	for (it = window_map.begin(); it != window_map.end(); ++it)
	{
		if (it->second->dirty)
		{
			it->second->dirty = false;
			std::vector<CL_Rect>::size_type size = it->second->update_region_list.size();
			for (unsigned int i = 0; i < size; i++)
			{
				site->func_paint->invoke(it->first, it->second->update_region_list[i]);
			}
		}
	}
}

void GUIWindowManagerProvider_Direct::on_displaywindow_window_close()
{
	if (activated_window == 0)
		return;

	// this isn't really right, but it will allow us to close our test GUI applications.
	site->func_close->invoke(activated_window);
}

void GUIWindowManagerProvider_Direct::on_input(const CL_InputEvent &input_event, const CL_InputState &input_state)
{
	if (activated_window == 0)
		return;

	CL_InputEvent new_input_event = input_event;
	CL_InputState new_input_state = input_state;

	if (!func_input_intercept.is_null())
		func_input_intercept.invoke(new_input_event, new_input_state);

	invoke_input_received(activated_window, new_input_event, new_input_state);
}

void GUIWindowManagerProvider_Direct::on_input_mouse_move(const CL_InputEvent &input_event, const CL_InputState &input_state)
{
	CL_InputEvent new_input_event = input_event;
	CL_InputState new_input_state = input_state;

	if (!func_input_intercept.is_null())
				func_input_intercept.invoke(new_input_event, new_input_state);

	bool capture_mouse_flag = false;
	if (capture_mouse_window)
	{
		// Only capture when left mouse is pressed 
		//(see win32 mouse capture for behaviour http://msdn.microsoft.com/en-us/library/ms646262.aspx )
// ** Disabled - It seems multiple windows in the same app act differently to microsoft docs **
//		if (get_display_window(capture_mouse_window).get_ic().get_mouse().get_keycode(CL_MOUSE_LEFT))
		{
			capture_mouse_flag = true;
		}
	}


	CL_GUITopLevelWindow *toplevel_window;
	if (capture_mouse_flag)
	{
		// From MSDN:
		//   "Only the foreground window can capture the mouse. 
		//    When a background window attempts to do so, the window receives messages
		//    only for mouse events that occur when the cursor hot spot is within
		//    the visible portion of the window."
// ** Disabled - It seems multiple windows in the same app act differently to microsoft docs **
//		toplevel_window = get_captured_window_at_point(new_input_event.mouse_pos, capture_mouse_window);
		toplevel_window = capture_mouse_window;
	}
	else
	{
		toplevel_window = get_window_at_point(new_input_event.mouse_pos);
	}

	if (toplevel_window)
		invoke_input_received(toplevel_window, new_input_event, new_input_state);
}

void GUIWindowManagerProvider_Direct::on_input_mouse_up(const CL_InputEvent &input_event, const CL_InputState &input_state)
{
	// It seems multiple windows in the same app act differently for window SetCapture()
	if (!capture_mouse_window)
	{
		// Process MouseUp as normal input event when capture mouse is not used
		on_input(input_event, input_state);
		return;
	}

	CL_InputEvent new_input_event = input_event;
	CL_InputState new_input_state = input_state;

	if (!func_input_intercept.is_null())
		func_input_intercept.invoke(new_input_event, new_input_state);

	invoke_input_received(capture_mouse_window, new_input_event, new_input_state);


}
void GUIWindowManagerProvider_Direct::on_input_mouse_down(const CL_InputEvent &input_event, const CL_InputState &input_state)
{
	CL_InputEvent new_input_event = input_event;
	CL_InputState new_input_state = input_state;

	if (!func_input_intercept.is_null())
		func_input_intercept.invoke(new_input_event, new_input_state);

	// It seems multiple windows in the same app act differently for window SetCapture()
	if (capture_mouse_window)
	{
		invoke_input_received(capture_mouse_window, new_input_event, new_input_state);
		return;
	}

	CL_GUITopLevelWindow *toplevel_window;
	toplevel_window = get_window_at_point(new_input_event.mouse_pos);

	if (toplevel_window)
		bring_to_front(toplevel_window);

	// Send focus lost events
	if (toplevel_window)
	{
		if (toplevel_window != activated_window)
		{
			// Change of focus required
			// From: http://msdn.microsoft.com/en-us/library/ms646274(VS.85).aspx - deactivation occurs before activation

			if (activated_window)
			{
				site->func_focus_lost->invoke(activated_window);
				toplevel_window = get_window_at_point(new_input_event.mouse_pos);
			}
		}
	}

	// Send focus gained events
	if (toplevel_window)
	{
		if (toplevel_window != activated_window)
		{
			activated_window = toplevel_window;
			site->func_focus_gained->invoke(activated_window);
			toplevel_window = get_window_at_point(new_input_event.mouse_pos);
		}
	}

	// Send mouse click event to toplevel window
	if (toplevel_window)
	{
		invoke_input_received(toplevel_window, new_input_event, new_input_state);
	}

}

void GUIWindowManagerProvider_Direct::bring_to_front(CL_GUITopLevelWindow *handle)
{
	GUITopLevelWindowDirect *texture = get_window_texture(handle);
	if(texture)
	{
		// Get root texture
		GUITopLevelWindowDirect *root_texture = texture;
		while (root_texture->owner_window)
		{
			root_texture = root_texture->owner_window;
		}

		// Check root window order
		if (root_window_z_order[0] != root_texture)
		{
			std::vector<GUITopLevelWindowDirect *>::iterator it;
			it = std::find(root_window_z_order.begin(), root_window_z_order.end(), root_texture);
			root_window_z_order.erase(it);
			root_window_z_order.insert(root_window_z_order.begin(), root_texture);
		}

		// Check owner window order
		if (texture->owner_window)
		{
			std::vector<GUITopLevelWindowDirect *> &z_order = texture->owner_window->child_windows_zorder;
			if (z_order[0] != texture)
			{
				std::vector<GUITopLevelWindowDirect *>::iterator it;
				it = std::find(z_order.begin(), z_order.end(), texture);
				z_order.erase(it);
				z_order.insert(z_order.begin(), texture);
			}
		}
	}
}

void GUIWindowManagerProvider_Direct::set_site(CL_GUIWindowManagerSite *new_site)
{
	site = new_site;
}

void GUIWindowManagerProvider_Direct::create_window(
	CL_GUITopLevelWindow *handle,
	CL_GUITopLevelWindow *owner,
	CL_GUIComponent *component,
	CL_GUITopLevelDescription description)
{
	// Create the texture group, if it has not already been set
	CL_GraphicContext &gc = display_window.get_gc();

	//Note: destroy_window() deletes this window
	GUITopLevelWindowDirect *toplevel_window = new GUITopLevelWindowDirect(handle);
	CL_Rect geometry = description.get_position();
	toplevel_window->geometry = geometry;
	toplevel_window->enabled = true;
	toplevel_window->visible = true;

	window_map[handle] = toplevel_window;

	if (owner)
	{
		// Attach to owner
		toplevel_window->owner_window = get_window_texture(owner);
		toplevel_window->owner_window->child_windows_zorder.insert(toplevel_window->owner_window->child_windows_zorder.begin(), toplevel_window);
	}
	else
	{
		// Attach to root
		root_window_z_order.insert(root_window_z_order.begin(), toplevel_window);
	}

	activated_window = handle;

}

void GUIWindowManagerProvider_Direct::destroy_window(CL_GUITopLevelWindow *handle)
{
	capture_mouse(handle, false);	// Ensure the destroyed window has not captured the mouse

	GUITopLevelWindowDirect *toplevel_window = get_window_texture(handle);

	if (toplevel_window->owner_window)
	{
		// Detach from owner
		toplevel_window->owner_window->child_windows_zorder.erase(std::find(toplevel_window->owner_window->child_windows_zorder.begin(), toplevel_window->owner_window->child_windows_zorder.end(), toplevel_window));
	}
	else
	{
		// Detach from root
		root_window_z_order.erase(std::find(root_window_z_order.begin(), root_window_z_order.end(), toplevel_window));
	}

	if (handle == activated_window)	// Ensure that the destroyed window is not in focus
	{
		activated_window = NULL;
	}

	delete toplevel_window;
	window_map.erase(window_map.find(handle));
}

void GUIWindowManagerProvider_Direct::enable_window(CL_GUITopLevelWindow *handle, bool enable)
{
	GUITopLevelWindowDirect *window_texture = get_window_texture(handle);
	window_texture->enabled = enable;
}

bool GUIWindowManagerProvider_Direct::has_focus(CL_GUITopLevelWindow *handle) const
{
	return activated_window == handle;
}

void GUIWindowManagerProvider_Direct::set_visible(CL_GUITopLevelWindow *handle, bool visible, bool activate)
{
	GUITopLevelWindowDirect *toplevel_window = get_window_texture(handle);
	toplevel_window->visible = visible;
	if (activate)
		activated_window = handle;
}

void GUIWindowManagerProvider_Direct::set_geometry(CL_GUITopLevelWindow *handle, const CL_Rect &geometry, bool client_area)
{
	// to-do: convert client area rect to window area rect, if needed.

	CL_GraphicContext &gc = display_window.get_gc();
	GUITopLevelWindowDirect *toplevel_window = get_window_texture(handle);

	if ((toplevel_window->geometry.get_width() == geometry.get_width()) &&
		(toplevel_window->geometry.get_height() == geometry.get_height()) )
	{
		// Do not need to rebuild texture if the width and height has not changed
		toplevel_window->geometry = geometry;
		return;
	}

	toplevel_window->geometry = geometry;

}

CL_Rect GUIWindowManagerProvider_Direct::get_geometry(CL_GUITopLevelWindow *handle, bool client_area) const
{
	std::map<CL_GUITopLevelWindow *, GUITopLevelWindowDirect *>::const_iterator it = window_map.find(handle);
	if (it != window_map.end())
	{
		return it->second->geometry;
	}
	else
	{
		return CL_Rect();
	}
}

CL_Point GUIWindowManagerProvider_Direct::screen_to_window(CL_GUITopLevelWindow *handle, const CL_Point &screen_point, bool client_area) const
{
	CL_Rect geometry = get_geometry(handle, false);
	return CL_Point(screen_point.x - geometry.left, screen_point.y - geometry.top);
}

CL_Point GUIWindowManagerProvider_Direct::window_to_screen(CL_GUITopLevelWindow *handle, const CL_Point &window_point, bool client_area) const
{
	CL_Rect geometry = get_geometry(handle, false);
	return CL_Point(window_point.x + geometry.left, window_point.y + geometry.top);
}

CL_GraphicContext& GUIWindowManagerProvider_Direct::get_gc(CL_GUITopLevelWindow *handle) const
{
	return display_window.get_gc();
}

CL_InputContext& GUIWindowManagerProvider_Direct::get_ic(CL_GUITopLevelWindow *handle) const
{
	return display_window.get_ic();
}

CL_GraphicContext GUIWindowManagerProvider_Direct::begin_paint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region)
{
	CL_GraphicContext &gc = display_window.get_gc();

	GUITopLevelWindowDirect *toplevel_window = get_window_texture(handle);

	if ( (painting_set == false) || (setup_painting_called == false) )
	{
		painting_set = true;
	}

	gc.set_map_mode(cl_map_2d_upper_left);

	CL_Rect clip_rect = update_region;
	clip_rect.translate(toplevel_window->geometry.left, toplevel_window->geometry.top);
	clip_rect.overlap(toplevel_window->geometry);

	gc.set_cliprect(clip_rect);

	// Translate model view matrix to the texture position
	gc.push_modelview();
	gc.mult_translate(toplevel_window->geometry.left, toplevel_window->geometry.top, 0);

	return gc;
}

void GUIWindowManagerProvider_Direct::end_paint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region)
{
	CL_GraphicContext &gc = display_window.get_gc();
	gc.flush_batcher();

	// Restore the model view matrix
	gc.pop_modelview();

	// Only reset the frame buffer at this point when setup_painting() was not used
	if (!setup_painting_called)
	{
		gc.reset_cliprect();
	}
}

void GUIWindowManagerProvider_Direct::capture_mouse(CL_GUITopLevelWindow *handle, bool state)
{
	if (state)
	{
		display_window.capture_mouse(true);
		capture_mouse_window = handle;
	}
	else
	{
		// Don't remove capture to the window, if handle does not control the capture
		if (capture_mouse_window == handle)
		{
			display_window.capture_mouse(false);
			capture_mouse_window = NULL;
		}
	}
}

CL_DisplayWindow GUIWindowManagerProvider_Direct::get_display_window(CL_GUITopLevelWindow *handle) const
{
	return display_window;
}

void GUIWindowManagerProvider_Direct::set_cursor(CL_GUITopLevelWindow *handle, const CL_Cursor &cursor)
{
}

void GUIWindowManagerProvider_Direct::set_cursor(CL_GUITopLevelWindow *handle, CL_StandardCursor type)
{
}

bool GUIWindowManagerProvider_Direct::is_minimized(CL_GUITopLevelWindow *handle) const
{
	return false;
}

bool GUIWindowManagerProvider_Direct::is_maximized(CL_GUITopLevelWindow *handle) const
{
	return false;
}

void GUIWindowManagerProvider_Direct::request_repaint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region)
{
	// Only invalidate when a valid rect was given
	if ((update_region.left >= update_region.right) || (update_region.top >= update_region.bottom))
	{
		return;
	}

	GUITopLevelWindowDirect *wptr = get_window_texture(handle);
	if (wptr->dirty)
	{
		// If the update region is already set, then check to see if it is a seperate rect or if it
		// should be extended to include the new update region

		bool updated = false;
		std::vector<CL_Rect>::size_type size = wptr->update_region_list.size();
		for (unsigned int i = 0; i < size; i++)
		{
			if (wptr->update_region_list[i].is_overlapped(update_region))
			{
				// Found an overlapping section
				wptr->update_region_list[i].bounding_rect(update_region);
				updated = true;
				break;
			}
		}

		if (!updated)
		{
			wptr->update_region_list.push_back(update_region);
		}
		else
		{
			//TODO: Check for overlapping rects in the wptr->update_region_list
		}
	}
	else
	{
		wptr->dirty = true;
		wptr->update_region_list.clear();
		wptr->update_region_list.push_back(update_region);
	}
}

void GUIWindowManagerProvider_Direct::update()
{
	update_paint();
}

void GUIWindowManagerProvider_Direct::setup_painting()
{
	setup_painting_called = true;
	painting_set = false;
}

void GUIWindowManagerProvider_Direct::finalise_painting()
{
	setup_painting_called = false;

	if ( painting_set )
	{
		painting_set = false;
		CL_GraphicContext &gc = display_window.get_gc();
		gc.reset_blend_mode();
		gc.reset_cliprect();
		gc.reset_frame_buffer();
	}
}

void GUIWindowManagerProvider_Direct::complete_painting()
{
	finalise_painting();
}

bool GUIWindowManagerProvider_Direct::is_constant_repaint_enabled(CL_GUIComponent *component) const
{
	// Constant repaint is always required
	return true;
}

void GUIWindowManagerProvider_Direct::set_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc, const CL_Rect &rect)
{
	GUITopLevelWindowDirect *toplevel_window = get_window_texture(handle);
	CL_Rect subtexture_geometry = toplevel_window->geometry;
	CL_Rect cliprect = rect;
	cliprect.translate(subtexture_geometry.left, subtexture_geometry.top);
	cliprect.overlap(subtexture_geometry);
	gc.set_cliprect(cliprect);

}

void GUIWindowManagerProvider_Direct::reset_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc)
{
	gc.pop_cliprect();
}

void GUIWindowManagerProvider_Direct::push_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc, const CL_Rect &rect)
{
	GUITopLevelWindowDirect *toplevel_window = get_window_texture(handle);
	CL_Rect subtexture_geometry = toplevel_window->geometry;
	CL_Rect cliprect = rect;
	cliprect.translate(subtexture_geometry.left, subtexture_geometry.top);
	cliprect.overlap(subtexture_geometry);
	gc.push_cliprect(cliprect);
}

void GUIWindowManagerProvider_Direct::pop_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc)
{
	gc.pop_cliprect();
}


/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerProvider_Direct Implementation:

CL_GUITopLevelWindow *GUIWindowManagerProvider_Direct::get_window_at_point(const CL_Point &point)
{
	return get_window_at_point(point, root_window_z_order);
}

CL_GUITopLevelWindow *GUIWindowManagerProvider_Direct::get_window_at_point(const CL_Point &point, const std::vector<GUITopLevelWindowDirect *> &z_order)
{
	std::vector<GUITopLevelWindowDirect *>::size_type index, size;
	size = z_order.size();
	for (index = 0; index < size; index++)
	{
		GUITopLevelWindowDirect *toplevel_window_texture = z_order[index];
		if (toplevel_window_texture->visible)
		{
			// Check children first
			CL_GUITopLevelWindow *child_window = get_window_at_point(point, toplevel_window_texture->child_windows_zorder);
			if (child_window)
				return child_window;

			if (toplevel_window_texture->geometry.contains(point))
			{
				return toplevel_window_texture->window;
			}
		}
	}
	return 0;
}

void GUIWindowManagerProvider_Direct::invoke_input_received(CL_GUITopLevelWindow *window, const CL_InputEvent &input_event, const CL_InputState &input_state)
{
	GUITopLevelWindowDirect *texture_window = get_window_texture(window);
	CL_InputEvent inp_event = input_event;
	inp_event.mouse_pos.x -= texture_window->geometry.left;
	inp_event.mouse_pos.y -= texture_window->geometry.top;
	site->func_input_received->invoke(window, inp_event, input_state);
}

GUITopLevelWindowDirect *GUIWindowManagerProvider_Direct::get_window_texture(CL_GUITopLevelWindow *handle)
{
	std::map<CL_GUITopLevelWindow *, GUITopLevelWindowDirect *>::iterator it;
	it = window_map.find(handle);
	if (it == window_map.end())
		throw CL_Exception("Invalid GUI Top Level Window requested");
	return it->second;
}

// Used by get_windows() to get child windows z order
void GUIWindowManagerProvider_Direct::get_all_windows_zorder(bool only_visible, std::vector<GUIWindowManagerDirectWindow> &windows_dest_list, const std::vector<GUITopLevelWindowDirect *> &z_order) const
{
	std::vector<GUITopLevelWindowDirect *>::size_type index, size;
	size = z_order.size();
	for (index = size; index > 0; index--)
	{
		GUITopLevelWindowDirect *toplevel_window = z_order[index-1];
		if ( (only_visible == false) || (toplevel_window->visible) )
		{
			windows_dest_list.push_back(GUIWindowManagerDirectWindow(toplevel_window->window, toplevel_window->geometry));

			// Get all children
			get_all_windows_zorder(only_visible, windows_dest_list, toplevel_window->child_windows_zorder);
		}
	}
}

