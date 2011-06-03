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
#include "API/CSSLayout/PropertyTypes/css_box_background_size.h"

CL_CSSBoxBackgroundSize::CL_CSSBoxBackgroundSize()
: type(type_value)
{
	values.push_back(Size());
}

void CL_CSSBoxBackgroundSize::compute(const CL_CSSBoxBackgroundSize *parent, CL_CSSResourceCache *layout, float em_size, float ex_size)
{
	if (type == type_inherit)
	{
		if (parent)
		{
			type = parent->type;
			values = parent->values;
		}
		else
		{
			type = type_value;
			values.clear();
			values.push_back(Size());
		}
	}
}

CL_String CL_CSSBoxBackgroundSize::to_string() const
{
	if (type == type_inherit)
		return "inherit";

	CL_String s;
	for (size_t i = 0; i < values.size(); i++)
	{
		if (i > 0)
			s += ", ";

		if (values[i].type == size_contain)
		{
			s += "contain";
		}
		else if (values[i].type == size_cover)
		{
			s += "cover";
		}
		else
		{
			switch (values[i].value_x)
			{
			case value_type_auto:
				s += "auto";
				break;
			case value_type_percentage:
				s += CL_StringHelp::float_to_text(values[i].percentage_x) + "%";
				break;
			case value_type_length:
				s += values[i].length_x.to_string();
				break;
			}
			s += " ";
			switch (values[i].value_y)
			{
			case value_type_auto:
				s += "auto";
				break;
			case value_type_percentage:
				s += CL_StringHelp::float_to_text(values[i].percentage_y) + "%";
				break;
			case value_type_length:
				s += values[i].length_y.to_string();
				break;
			}
		}
	}
	return s;
}
