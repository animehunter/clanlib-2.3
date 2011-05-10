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

#include "GUI/precomp.h"

#include "gui_window_manager_direct_window.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerDirectWindow_Impl class:

class CL_GUIWindowManagerDirectWindow_Impl
{
public:
	CL_Rect geometry;
	CL_GUITopLevelWindow *window;
};

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerDirectWindow construction:

CL_GUIWindowManagerDirectWindow::CL_GUIWindowManagerDirectWindow()
{
}

CL_GUIWindowManagerDirectWindow::CL_GUIWindowManagerDirectWindow(CL_GUITopLevelWindow *window, const CL_Rect &geometry)
: impl(new CL_GUIWindowManagerDirectWindow_Impl)
{
	impl->geometry = geometry;
	impl->window = window;
}

CL_GUIWindowManagerDirectWindow::~CL_GUIWindowManagerDirectWindow()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerDirectWindow attributes:

CL_Rect CL_GUIWindowManagerDirectWindow::get_geometry() const
{
	return impl->geometry;
}

CL_GUITopLevelWindow *CL_GUIWindowManagerDirectWindow::get_window() const
{
	return impl->window;
}


/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerDirectWindow implementation:
