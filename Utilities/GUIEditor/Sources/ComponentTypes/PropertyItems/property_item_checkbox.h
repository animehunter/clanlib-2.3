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
**    Kenneth Gangstoe
*/

#pragma once

class PropertyItemCheckbox : public PropertyItem
{
public:
	PropertyItemCheckbox(const CL_String &name) { this->name = name; }
	PropertyItemCheckbox(const CL_String &name, bool value) { this->name = name; this->value = value; }

	CL_GUIComponent *activate()
	{
		value = !value;
		property_component->deactivate();
		return 0;
	}

	void deactivate(CL_GUIComponent *component)
	{
	}

	int get_inactive_height(CL_GraphicContext &gc, CL_GUIThemePart &part, int width)
	{
		return 0;
	}

	void render_inactive(CL_GraphicContext &gc, CL_GUIThemePart &part, const CL_Rect &rect, const CL_Rect &clip_rect)
	{
		CL_String text;
		if(value == true)
			text = "true";
		else
			text = "false";
		part.render_text(gc, text, rect, clip_rect);
	}

	bool value;
};
