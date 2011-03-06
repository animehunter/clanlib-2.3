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
#include "css_replaced_layout.h"
#include "css_layout_cursor.h"
#include "css_block_formatting_context.h"
#include "../BoxTree/css_box_object.h"
#include "API/CSSLayout/css_replaced_component.h"

CL_CSSReplacedLayout::CL_CSSReplacedLayout(CL_CSSBoxObject *block_element)
: CL_CSSLayoutTreeNode(block_element), component(0)
{
	component = block_element->get_component();
	intrinsic.width = component->intrinsic_width;
	intrinsic.height = component->intrinsic_height;
	intrinsic.ratio = component->intrinsic_ratio;
	intrinsic.has_width = component->intrinsic_has_width;
	intrinsic.has_height = component->intrinsic_has_height;
	intrinsic.has_ratio = component->intrinsic_has_ratio;
}

CL_CSSReplacedLayout::~CL_CSSReplacedLayout()
{
}

void CL_CSSReplacedLayout::calculate_top_down_sizes()
{
	margin.left = get_css_margin_width(element_node->computed_properties.margin_width_left, containing_width);
	margin.right = get_css_margin_width(element_node->computed_properties.margin_width_right, containing_width);
	border.left = element_node->computed_properties.border_width_left.length.value;
	border.right = element_node->computed_properties.border_width_right.length.value;
	padding.left = get_css_padding_width(element_node->computed_properties.padding_width_left, containing_width);
	padding.right = get_css_padding_width(element_node->computed_properties.padding_width_right, containing_width);

	if (element_node->computed_properties.width.type == CL_CSSBoxWidth::type_auto && element_node->computed_properties.height.type == CL_CSSBoxHeight::type_auto && intrinsic.has_width)
	{
		width.value = intrinsic.width;
		width.expanding = false;
	}
	else if (element_node->computed_properties.width.type == CL_CSSBoxWidth::type_auto && element_node->computed_properties.height.type == CL_CSSBoxHeight::type_auto && intrinsic.has_height && intrinsic.has_ratio)
	{
		width.value = intrinsic.height / intrinsic.ratio;
		width.expanding = false;
	}
	else if (element_node->computed_properties.width.type == CL_CSSBoxWidth::type_auto && element_node->computed_properties.height.type == CL_CSSBoxHeight::type_auto && intrinsic.has_ratio)
	{
		if (containing_width.expanding)
		{
			width.value = 0.0f;
			width.expanding = true;
		}
		else
		{
			width.value = containing_width.value - margin.left - margin.right - border.left - border.right - padding.left - padding.right;
			width.expanding = false;
		}
	}
	else if (element_node->computed_properties.width.type == CL_CSSBoxWidth::type_auto && intrinsic.has_width)
	{
		width.value = intrinsic.width;
		width.expanding = false;
	}
	else if (element_node->computed_properties.width.type == CL_CSSBoxWidth::type_auto)
	{
		width.value = 300.0f; // bug: Should be 300px (css physical length)
		width.expanding = false;
		/*if (width > device.width)
		{
			width = largest_2_1_ratio_rect();
		}*/
	}
	else if (element_node->computed_properties.width.type == CL_CSSBoxWidth::type_length)
	{
		width.value = element_node->computed_properties.width.length.value;
		width.expanding = false;
	}
	else if (element_node->computed_properties.width.type == CL_CSSBoxWidth::type_percentage)
	{
		width.value = element_node->computed_properties.width.percentage * containing_width.value / 100.0f;
		width.expanding = containing_width.expanding;
	}
	else
	{
		// Unreachable code (unless something is seriously wrong elsewhere).
		width.value = 0.0f;
		width.expanding = false;
	}

	if (!width.expanding)
	{
		if (element_node->computed_properties.max_width.type == CL_CSSBoxMaxWidth::type_length)
			width.value = cl_min(width.value, element_node->computed_properties.max_width.length.value);
		else if (element_node->computed_properties.max_width.type == CL_CSSBoxMaxWidth::type_percentage && !containing_width.expanding)
			width.value = cl_min(width.value, element_node->computed_properties.max_width.percentage * containing_width.value / 100.0f);

		if (element_node->computed_properties.min_width.type == CL_CSSBoxMinWidth::type_length)
			width.value = cl_max(width.value, element_node->computed_properties.min_width.length.value);
		else if (element_node->computed_properties.min_width.type == CL_CSSBoxMinWidth::type_percentage && !containing_width.expanding)
			width.value = cl_max(width.value, element_node->computed_properties.min_width.percentage * containing_width.value / 100.0f);

		if (!containing_width.expanding)
		{
			if (element_node->computed_properties.margin_width_left.type == CL_CSSBoxMarginWidth::type_auto && element_node->computed_properties.margin_width_right.type == CL_CSSBoxMarginWidth::type_auto)
			{
				margin.left = cl_max(0.0f, (containing_width.value-border.left-border.right-padding.left-padding.right-width.value)/2.0f);
				margin.right = cl_max(0.0f, containing_width.value-border.left-border.right-padding.left-padding.right-width.value-margin.left);
			}
			else if (element_node->computed_properties.margin_width_left.type == CL_CSSBoxMarginWidth::type_auto)
			{
				margin.left = cl_max(0.0f, containing_width.value-margin.right-border.left-border.right-padding.left-padding.right-width.value);
			}
			else if (element_node->computed_properties.margin_width_right.type == CL_CSSBoxMarginWidth::type_auto)
			{
				margin.right = cl_max(0.0f, containing_width.value-margin.left-border.left-border.right-padding.left-padding.right-width.value);
			}

			if (margin.left + border.left + width.value + border.right + padding.right + margin.right > containing_width.value)
			{
				if (element_node->computed_properties.direction.type == CL_CSSBoxDirection::type_ltr)
					margin.right = cl_max(0.0f, containing_width.value-margin.left-border.left-border.right-padding.left-padding.right-width.value);
				else
					margin.left = cl_max(0.0f, containing_width.value-margin.right-border.left-border.right-padding.left-padding.right-width.value);
			}
		}
	}

	margin.top = get_css_margin_height(element_node->computed_properties.margin_width_top, containing_height);
	margin.bottom = get_css_margin_height(element_node->computed_properties.margin_width_bottom, containing_height);
	border.top = element_node->computed_properties.border_width_top.length.value;
	border.bottom = element_node->computed_properties.border_width_bottom.length.value;
	padding.top = get_css_padding_height(element_node->computed_properties.padding_width_top, containing_height);
	padding.bottom = get_css_padding_height(element_node->computed_properties.padding_width_bottom, containing_height);

	if (element_node->computed_properties.height.type == CL_CSSBoxHeight::type_length)
	{
		height.value = element_node->computed_properties.height.length.value;
		height.use_content = false;
	}
	else if (element_node->computed_properties.height.type == CL_CSSBoxHeight::type_percentage)
	{
		if (containing_height.use_content)
		{
			height.value = 0.0f;
			height.use_content = true;
		}
		else
		{
			height.value = containing_height.value * element_node->computed_properties.height.percentage / 100.0f;
			height.use_content = false;
		}
	}
	else if (element_node->computed_properties.height.type == CL_CSSBoxHeight::type_auto)
	{
		if (intrinsic.has_ratio)
		{
			height.value = width.value * intrinsic.ratio;
			height.use_content = false;
		}
		else
		{
			height.value = 0.0f;
			height.use_content = true;
		}
	}
	else
	{
		// Unreachable code (unless something is seriously wrong elsewhere).
		height.value = 0.0f;
		height.use_content = false;
	}

	if (!height.use_content)
	{
		if (element_node->computed_properties.max_height.type == CL_CSSBoxMaxHeight::type_length)
		{
			height.value = cl_min(height.value, element_node->computed_properties.max_height.length.value);
		}
		else if (element_node->computed_properties.max_height.type == CL_CSSBoxMaxHeight::type_percentage && !containing_height.use_content)
		{
			height.value = cl_min(height.value, element_node->computed_properties.max_height.percentage * containing_height.value / 100.0f);
		}

		if (element_node->computed_properties.min_height.type == CL_CSSBoxMinHeight::type_length)
		{
			height.value = cl_max(height.value, element_node->computed_properties.min_height.length.value);
		}
		else if (element_node->computed_properties.min_height.type == CL_CSSBoxMinHeight::type_percentage && !containing_height.use_content)
		{
			height.value = cl_max(height.value, element_node->computed_properties.min_height.percentage * containing_height.value / 100.0f);
		}
	}

}

void CL_CSSReplacedLayout::prepare_children()
{
}

void CL_CSSReplacedLayout::layout_content(CL_GraphicContext &gc, CL_CSSLayoutCursor &cursor, LayoutStrategy strategy)
{
	if (strategy != normal_strategy)
	{
		if (width.expanding)
			width.value = intrinsic.width;
	}

	if (height.use_content && intrinsic.has_height)
		height.value = intrinsic.height;
	else if (height.use_content && intrinsic.has_ratio)
		height.value = width.value * intrinsic.ratio;

	if (height.value > 0.0f)
		cursor.apply_margin();
	box = CL_Rect(cursor.x, cursor.y, cursor.x + cl_used_to_actual(width.value), cursor.y + cl_used_to_actual(height.value));
	box.translate(cl_used_to_actual(relative_x), cl_used_to_actual(relative_y));
	cursor.y += cl_used_to_actual(height.value);
}

void CL_CSSReplacedLayout::set_component_geometry()
{
	if (component)
	{
		CL_Rect b = box;
		b.translate(formatting_context->get_x(), formatting_context->get_y());
		component->set_geometry(b);
	}
}

int CL_CSSReplacedLayout::get_first_line_baseline()
{
	return box.bottom;
}

int CL_CSSReplacedLayout::get_last_line_baseline()
{
	return get_first_line_baseline();
}

void CL_CSSReplacedLayout::render_layer_background(CL_GraphicContext &gc, CL_CSSResourceCache *resources, bool root)
{
	render_non_content(gc, resources, root);
}

void CL_CSSReplacedLayout::render_layer_non_inline(CL_GraphicContext &gc, CL_CSSResourceCache *resources)
{
}

void CL_CSSReplacedLayout::render_layer_floats(CL_GraphicContext &gc, CL_CSSResourceCache *resources)
{
}

void CL_CSSReplacedLayout::render_layer_inline(CL_GraphicContext &gc, CL_CSSResourceCache *resources)
{
}

void CL_CSSReplacedLayout::render_layer_positioned(CL_GraphicContext &gc, CL_CSSResourceCache *resources)
{
}
