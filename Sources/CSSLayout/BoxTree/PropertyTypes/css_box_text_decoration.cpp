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
#include "API/CSSLayout/PropertyTypes/css_box_text_decoration.h"

CL_CSSBoxTextDecoration::CL_CSSBoxTextDecoration()
: type(type_inherit_special), underline(false), overline(false), line_through(false), blink(false)
{
}

void CL_CSSBoxTextDecoration::compute(const CL_CSSBoxTextDecoration *parent, CL_CSSResourceCache *layout, float em_size, float ex_size)
{
	if (type == type_inherit)
	{
		if (parent)
		{
			type = parent->type;
			underline = parent->underline;
			overline = parent->overline;
			line_through = parent->line_through;
			blink = parent->blink;
		}
		else
		{
			type = type_none;
		}
	}
}

CL_String CL_CSSBoxTextDecoration::to_string() const
{
	switch (type)
	{
	default:
	case type_none:
		return "none";
	case type_values:
		{
			CL_String s;
			if (underline)
				s += "underline";
			if (overline)
			{
				if (!s.empty())
					s += " ";
				s += "overline";
			}
			if (line_through)
			{
				if (!s.empty())
					s += " ";
				s += "line-through";
			}
			if (blink)
			{
				if (!s.empty())
					s += " ";
				s += "blink";
			}
			return s;
		}
	case type_inherit:
		return "inherit";
	case type_inherit_special:
		return "-clan-inherit-special";
	}
}
