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

/// \addtogroup clanGUI_System clanGUI System
/// \{

#pragma once

#include "API/GUI/gui_window_manager.h"

class CL_GUIWindowManagerDirectWindow_Impl;

/// \brief Direct window manager window.
///
/// \xmlonly !group=GUI/System! !header=gui.h! \endxmlonly
class CL_GUIWindowManagerDirectWindow
{
/// \name Construction
/// \{
public:
	CL_GUIWindowManagerDirectWindow();

	/// \brief Constructs a GUIWindowManagerTextureWindow
	///
	/// \param window = GUITop Level Window
	/// \param geometry = Rect
	CL_GUIWindowManagerDirectWindow(CL_GUITopLevelWindow *window, const CL_Rect &geometry);

	~CL_GUIWindowManagerDirectWindow();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Get Geometry
	///
	/// \return geometry
	CL_Rect get_geometry() const;

	/// \brief Get Window
	///
	/// \return window
	CL_GUITopLevelWindow *get_window() const;

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_GUIWindowManagerDirectWindow_Impl> impl;
/// \}
};

/// \}
