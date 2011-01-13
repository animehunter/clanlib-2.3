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
**    Harry Storbacka
*/

#pragma once


#include "GUI/precomp.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/tab_page.h"
#include "API/Core/XML/dom_element.h"

/////////////////////////////////////////////////////////////////////////////
// CL_TabPage_Impl Class:

class CL_TabPage_Impl
{
public:
	void on_process_message(CL_GUIMessage &msg);

	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);

	void on_style_changed();

	void on_resized();

	CL_GUIThemePart part_background;

	CL_TabPage *component;

	int id;

	CL_String label;
};


