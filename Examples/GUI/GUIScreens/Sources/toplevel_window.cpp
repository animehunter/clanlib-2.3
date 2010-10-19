
#include "precomp.h"
#include "toplevel_window.h"
#include "window_manager.h"

TopLevelWindow::TopLevelWindow(WindowManager *wm, CL_GUITopLevelWindow *owner, CL_GUITopLevelDescription description)
: wm(wm), owner(owner), description(description)
{
}

void TopLevelWindow::set_visible(bool show, bool activate)
{
	if (show != visible)
	{
		visible = show;
		if (visible)
			create_texture();
		else
			destroy_texture();
	}
}

void TopLevelWindow::set_geometry(const CL_Rect &box)
{
	if (box != geometry)
	{
		geometry = box;
		if (visible)
		{
			destroy_texture();
			create_texture();
		}
	}
}

void TopLevelWindow::create_texture()
{
	texture = CL_Texture(wm->dispwindow.get_gc(), geometry.get_width(), geometry.get_height());
}

void TopLevelWindow::destroy_texture()
{
	texture = CL_Texture();
}
