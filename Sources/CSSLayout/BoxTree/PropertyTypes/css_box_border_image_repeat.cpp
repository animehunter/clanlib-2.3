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

#include "CSSLayout/precomp.h"
#include "API/CSSLayout/PropertyTypes/css_box_border_image_repeat.h"

CL_CSSBoxBorderImageRepeat::CL_CSSBoxBorderImageRepeat()
: type(type_values), repeat_x(repeat_type_stretch), repeat_y(repeat_type_stretch)
{
}

void CL_CSSBoxBorderImageRepeat::compute(const CL_CSSBoxBorderImageRepeat *parent, CL_CSSResourceCache *layout, float em_size, float ex_size)
{
	if (type == type_inherit)
	{
		if (parent)
		{
			type = parent->type;
			repeat_x = parent->repeat_x;
			repeat_y = parent->repeat_y;
		}
		else
		{
			type = type_values;
			repeat_x = repeat_type_stretch;
			repeat_y = repeat_type_stretch;
		}
	}
}

CL_String CL_CSSBoxBorderImageRepeat::to_string() const
{
	if (type == type_inherit)
		return "inherit";

	CL_String s;
	switch (repeat_x)
	{
	default:
	case repeat_type_stretch:
		s += "stretch";
		break;
	case repeat_type_repeat:
		s += "repeat";
		break;
	case repeat_type_round:
		s += "round";
		break;
	case repeat_type_space:
		s += "space";
		break;
	}
	s += " ";
	switch (repeat_y)
	{
	default:
	case repeat_type_stretch:
		s += "stretch";
		break;
	case repeat_type_repeat:
		s += "repeat";
		break;
	case repeat_type_round:
		s += "round";
		break;
	case repeat_type_space:
		s += "space";
		break;
	}
	return s;
}
