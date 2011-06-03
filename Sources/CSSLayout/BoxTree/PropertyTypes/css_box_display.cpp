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
#include "API/CSSLayout/PropertyTypes/css_box_display.h"
#include "API/CSSLayout/PropertyTypes/css_box_position.h"
#include "API/CSSLayout/PropertyTypes/css_box_float.h"

CL_CSSBoxDisplay::CL_CSSBoxDisplay()
: type(type_inline)
{
}

void CL_CSSBoxDisplay::compute(const CL_CSSBoxDisplay *parent, CL_CSSResourceCache *layout, float em_size, float ex_size, const CL_CSSBoxPosition &position, CL_CSSBoxFloat &float_box)
{
	if (type == type_inherit)
	{
		if (parent)
			type = parent->type;
		else
			type = type_inline;
	}

	if (type != type_none)
	{
		if (position.type == CL_CSSBoxPosition::type_absolute || position.type == CL_CSSBoxPosition::type_fixed)
		{
			apply_table_9_7();
			float_box.type = CL_CSSBoxFloat::type_none;
		}
		else if (float_box.type != CL_CSSBoxFloat::type_none)
		{
			apply_table_9_7();
		}
		else if (!parent) // if is root element
		{
			apply_table_9_7();
		}
	}
}

void CL_CSSBoxDisplay::apply_table_9_7()
{
	switch (type)
	{
	case type_inline_table:
		type = type_table;
		break;
	case type_inline:
	case type_run_in:
	case type_table_row_group:
	case type_table_column:
	case type_table_column_group:
	case type_table_header_group:
	case type_table_footer_group:
	case type_table_row:
	case type_table_cell:
	case type_table_caption:
	case type_inline_block:
		type = type_block;
		break;
	default:
		break;
	}
}

CL_String CL_CSSBoxDisplay::to_string() const
{
	switch (type)
	{
	default:
	case type_inline:
		return "inline";
	case type_block:
		return "block";
	case type_list_item:
		return "list-item";
	case type_run_in:
		return "run-in";
	case type_inline_block:
		return "inline-block";
	case type_table:
		return "table";
	case type_inline_table:
		return "inline-table";
	case type_table_row_group:
		return "table-row-group";
	case type_table_header_group:
		return "table-header-group";
	case type_table_footer_group:
		return "table-footer-group";
	case type_table_row:
		return "table-row";
	case type_table_column_group:
		return "table-column-group";
	case type_table_column:
		return "table-column";
	case type_table_cell:
		return "table-cell";
	case type_table_caption:
		return "table-caption";
	case type_none:
		return "none";
	case type_inherit:
		return "inherit";
	}
}
