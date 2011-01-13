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
*/

#pragma once

#include "Popups/line_edit_popup.h"

class PropertyItemLineEdit : public PropertyItem
{
public:
	PropertyItemLineEdit(const CL_String &name) { this->name = name; }
	PropertyItemLineEdit(const CL_String &name, const CL_String &value) { this->name = name; this->value = value; }

	CL_GUIComponent *activate()
	{
		LineEditPopup *c = new LineEditPopup(property_component);
		c->get_lineedit()->func_enter_pressed().set(this, &PropertyItemLineEdit::on_enter_pressed);
		c->get_lineedit()->set_text(value);
		return c;
	}

	void deactivate(CL_GUIComponent *component)
	{
		value = static_cast<LineEditPopup *>(component)->get_lineedit()->get_text();
		delete component;
	}

	int get_inactive_height(CL_GraphicContext &gc, CL_GUIThemePart &part, int width)
	{
		return part.get_font().get_text_size(gc, value).height;
	}

	void render_inactive(CL_GraphicContext &gc, CL_GUIThemePart &part, const CL_Rect &rect, const CL_Rect &clip_rect)
	{
		part.render_text(gc, value, rect, clip_rect);
	}

	void on_enter_pressed()
	{
		property_component->deactivate();
	}

	CL_String value;
};
