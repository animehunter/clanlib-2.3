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
#include "API/CSSLayout/PropertyTypes/css_box_border_radius.h"
#include "../../css_resource_cache.h"

CL_CSSBoxBorderRadius::CL_CSSBoxBorderRadius()
: type(type_inherit), value_type1(value_type_length), percentage1(0.0f), value_type2(value_type_length), percentage2(0.0f)
{
}

void CL_CSSBoxBorderRadius::compute(const CL_CSSBoxBorderRadius *parent, CL_CSSResourceCache *layout, float em_size, float ex_size)
{
	if (type == type_inherit)
	{
		if (parent)
		{
			type = parent->type;
			length1 = parent->length1;
			length2 = parent->length2;
		}
		else
		{
			type = type_two_values;
			value_type1 = value_type_length;
			value_type2 = value_type_length;
			length1 = CL_CSSBoxLength(0.0f, CL_CSSBoxLength::type_px);
			length2 = CL_CSSBoxLength(0.0f, CL_CSSBoxLength::type_px);
		}
	}

	if (type == type_one_value)
	{
		type = type_two_values;
		value_type2 = value_type1;
		length2 = length1;
		percentage2 = percentage1;
	}

	length1 = layout->compute_length(length1, em_size, ex_size);
	length2 = layout->compute_length(length2, em_size, ex_size);
}

CL_String CL_CSSBoxBorderRadius::to_string() const
{
	switch (type)
	{
	default:
	case type_one_value:
		switch (value_type1)
		{
		default:
		case value_type_length:
			return length1.to_string();
		case value_type_percentage:
			return CL_StringHelp::float_to_text(percentage1) + "%";
		}
	case type_two_values:
		{
			CL_String s;
			switch (value_type1)
			{
			default:
			case value_type_length:
				s += length1.to_string();
			case value_type_percentage:
				s += CL_StringHelp::float_to_text(percentage1) + "%";
			}
			s += " ";
			switch (value_type2)
			{
			default:
			case value_type_length:
				s += length2.to_string();
			case value_type_percentage:
				s += CL_StringHelp::float_to_text(percentage2) + "%";
			}
			return s;
		}
	case type_inherit:
		return "inherit";
	}
}
