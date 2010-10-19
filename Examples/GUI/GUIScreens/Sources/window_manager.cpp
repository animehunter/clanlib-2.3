
#include "precomp.h"
#include "window_manager.h"
#include "toplevel_window.h"

CL_GUIWindowManager WindowManager::create(CL_DisplayWindow &dispwindow)
{
	return CL_GUIWindowManager(new WindowManager(dispwindow));
}

WindowManager *WindowManager::cast(const CL_GUIWindowManager &wm)
{
	return dynamic_cast<WindowManager*>(wm.get_provider());
}

WindowManager::WindowManager(CL_DisplayWindow &dispwindow)
: dispwindow(dispwindow), site(0)
{
}

WindowManager::~WindowManager()
{
	for (std::map<CL_GUITopLevelWindow *, TopLevelWindow *>::iterator it = window_map.begin(); it != window_map.end(); ++it)
		delete it->second;
	window_map.clear();
}

void WindowManager::destroy()
{
	delete this;
}

void WindowManager::set_site(CL_GUIWindowManagerSite *new_site)
{
	site = new_site;
}

void WindowManager::create_window(CL_GUITopLevelWindow *handle, CL_GUITopLevelWindow *owner, CL_GUITopLevelDescription description)
{
	window_map[handle] = new TopLevelWindow(this, owner, description);
}

void WindowManager::destroy_window(CL_GUITopLevelWindow *handle)
{
	delete window_map[handle];
	window_map.erase(window_map.find(handle));
}

void WindowManager::enable_window(CL_GUITopLevelWindow *handle, bool enable)
{
	window_map[handle]->set_enabled(enable);
}

bool WindowManager::has_focus(CL_GUITopLevelWindow *handle) const
{
	return false;
}

void WindowManager::set_visible(CL_GUITopLevelWindow *handle, bool visible, bool activate_root_win)
{
	window_map[handle]->set_visible(visible, activate_root_win);
}

void WindowManager::set_geometry(CL_GUITopLevelWindow *handle, const CL_Rect &geometry, bool client_area)
{
	window_map[handle]->set_geometry(geometry);
}

CL_Rect WindowManager::get_geometry(CL_GUITopLevelWindow *handle, bool client_area) const
{
	return get_toplevel_window(handle)->get_geometry();
}

CL_Point WindowManager::screen_to_window(CL_GUITopLevelWindow *handle, const CL_Point &screen_point, bool client_area) const
{
	return screen_point;
}

CL_Point WindowManager::window_to_screen(CL_GUITopLevelWindow *handle, const CL_Point &window_point, bool client_area) const
{
	return window_point;
}

CL_GraphicContext& WindowManager::get_gc(CL_GUITopLevelWindow *handle) const
{
	return dispwindow.get_gc();
}

CL_InputContext& WindowManager::get_ic(CL_GUITopLevelWindow *handle) const
{
	return dispwindow.get_ic();
}

CL_GraphicContext WindowManager::begin_paint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region)
{
	return dispwindow.get_gc();
}

void WindowManager::set_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc, const CL_Rect &rect)
{
	gc.set_cliprect(rect);
}

void WindowManager::reset_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc)
{
	gc.reset_cliprect();
}

void WindowManager::push_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc, const CL_Rect &rect)
{
	gc.push_cliprect(rect);
}

void WindowManager::pop_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc)
{
	gc.pop_cliprect();
}

void WindowManager::end_paint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region)
{
}

void WindowManager::request_repaint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region)
{
}

void WindowManager::bring_to_front(CL_GUITopLevelWindow *handle)
{
}

bool WindowManager::is_minimized(CL_GUITopLevelWindow *handle) const
{
	return get_toplevel_window(handle)->is_minimized();
}

bool WindowManager::is_maximized(CL_GUITopLevelWindow *handle) const
{
	return get_toplevel_window(handle)->is_maximized();
}

void WindowManager::capture_mouse(CL_GUITopLevelWindow *handle, bool state)
{
}

CL_DisplayWindow WindowManager::get_display_window(CL_GUITopLevelWindow *handle) const
{
	return dispwindow;
}

void WindowManager::set_cursor(CL_GUITopLevelWindow *handle, const CL_Cursor &cursor)
{
}

void WindowManager::set_cursor(CL_GUITopLevelWindow *handle, enum CL_StandardCursor type)
{
}

void WindowManager::update()
{
}

void WindowManager::setup_painting()
{
}

void WindowManager::complete_painting()
{
}

CL_GUIWindowManager::CL_WindowManagerType WindowManager::get_window_manager_type() const
{
	return CL_GUIWindowManager::cl_wm_type_texture;
}

TopLevelWindow *WindowManager::get_toplevel_window(CL_GUITopLevelWindow *handle) const
{
	return window_map.find(handle)->second;
}
