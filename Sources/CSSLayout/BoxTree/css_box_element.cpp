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
#include "css_box_element.h"
#include "API/CSSLayout/css_box_properties.h"

CL_CSSBoxElement::CL_CSSBoxElement()
{
}

bool CL_CSSBoxElement::has_block_level_children() const
{
	const CL_CSSBoxNode *cur = get_first_child();
	while (cur)
	{
		const CL_CSSBoxElement *child_element = dynamic_cast<const CL_CSSBoxElement*>(cur);
		if (child_element && child_element->is_block_level())
			return true;
		cur = cur->get_next_sibling();
	}
	return false;
}

bool CL_CSSBoxElement::is_block_level() const
{
	switch (computed_properties.display.type)
	{
	case CL_CSSBoxDisplay::type_block:
	case CL_CSSBoxDisplay::type_run_in:
	case CL_CSSBoxDisplay::type_list_item:
	case CL_CSSBoxDisplay::type_table:
	case CL_CSSBoxDisplay::type_table_row_group:
	case CL_CSSBoxDisplay::type_table_header_group:
	case CL_CSSBoxDisplay::type_table_footer_group:
	case CL_CSSBoxDisplay::type_table_row:
	case CL_CSSBoxDisplay::type_table_column_group:
	case CL_CSSBoxDisplay::type_table_column:
	case CL_CSSBoxDisplay::type_table_cell:
	case CL_CSSBoxDisplay::type_table_caption:
		return true;
	default:
		return false;
	}
}

bool CL_CSSBoxElement::is_inline_block_level() const
{
	switch (computed_properties.display.type)
	{
	case CL_CSSBoxDisplay::type_inline_block:
	case CL_CSSBoxDisplay::type_inline_table:
		return true;
	default:
		return false;
	}
}

bool CL_CSSBoxElement::is_shrink_to_fit() const
{
	if (computed_properties.display.type == CL_CSSBoxDisplay::type_inline_block ||
		computed_properties.display.type == CL_CSSBoxDisplay::type_inline_table ||
		computed_properties.display.type == CL_CSSBoxDisplay::type_table ||
		/*computed_properties.display.type == CL_CSSBoxDisplay::type_table_cell ||*/
		computed_properties.float_box.type != CL_CSSBoxFloat::type_none ||
		is_absolute() || is_fixed())
	{
		return computed_properties.width.type == CL_CSSBoxWidth::type_auto;
	}
	else
	{
		return false;
	}
}

bool CL_CSSBoxElement::is_float() const
{
	return computed_properties.float_box.type != CL_CSSBoxFloat::type_none;
}

bool CL_CSSBoxElement::is_table() const
{
	return computed_properties.display.type == CL_CSSBoxDisplay::type_table;
}

bool CL_CSSBoxElement::is_inline_table() const
{
	return computed_properties.display.type == CL_CSSBoxDisplay::type_inline_table;
}

bool CL_CSSBoxElement::is_table_row() const
{
	return computed_properties.display.type == CL_CSSBoxDisplay::type_table_row;
}

bool CL_CSSBoxElement::is_table_cell() const
{
	return computed_properties.display.type == CL_CSSBoxDisplay::type_table_cell;
}

bool CL_CSSBoxElement::is_table_row_group() const
{
	return computed_properties.display.type == CL_CSSBoxDisplay::type_table_row_group;
}

bool CL_CSSBoxElement::is_table_header_group() const
{
	return computed_properties.display.type == CL_CSSBoxDisplay::type_table_header_group;
}

bool CL_CSSBoxElement::is_table_footer_group() const
{
	return computed_properties.display.type == CL_CSSBoxDisplay::type_table_footer_group;
}

bool CL_CSSBoxElement::is_table_column() const
{
	return computed_properties.display.type == CL_CSSBoxDisplay::type_table_column;
}

bool CL_CSSBoxElement::is_table_column_group() const
{
	return computed_properties.display.type == CL_CSSBoxDisplay::type_table_column_group;
}

bool CL_CSSBoxElement::is_table_caption() const
{
	return computed_properties.display.type == CL_CSSBoxDisplay::type_table_caption;
}

bool CL_CSSBoxElement::is_absolute() const
{
	return computed_properties.position.type == CL_CSSBoxPosition::type_absolute;
}

bool CL_CSSBoxElement::is_fixed() const
{
	return computed_properties.position.type == CL_CSSBoxPosition::type_fixed;
}

bool CL_CSSBoxElement::is_display_none() const
{
	return computed_properties.display.type == CL_CSSBoxDisplay::type_none;
}

bool CL_CSSBoxElement::is_overflow_visible() const
{
	return computed_properties.overflow.type == CL_CSSBoxOverflow::type_visible;
}
