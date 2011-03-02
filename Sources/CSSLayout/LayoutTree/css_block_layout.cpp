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
#include "css_block_layout.h"
#include "css_block_formatting_context.h"
#include "css_layout_cursor.h"
#include "css_inline_layout.h"
#include "css_table_layout.h"
#include "css_stacking_context.h"
#include "../BoxTree/css_box_element.h"
#include "../BoxTree/css_box_text.h"
#include "../css_resource_cache.h"

CL_CSSBlockLayout::CL_CSSBlockLayout(CL_CSSBoxElement *element_node)
: CL_CSSLayoutTreeNode(element_node)
{
}

CL_CSSBlockLayout::~CL_CSSBlockLayout()
{
	for (size_t i = 0; i < children.size(); i++)
		delete children[i];
}

void CL_CSSBlockLayout::calculate_content_top_down_sizes()
{
	for (size_t i = 0; i < children.size(); i++)
	{
		children[i]->containing_width = width;
		children[i]->containing_height = height;
		children[i]->calculate_top_down_sizes();
	}
}

void CL_CSSBlockLayout::set_content_expanding_width()
{
	for (size_t i = 0; i < children.size(); i++)
	{
		children[i]->containing_width.value = width.value;
		children[i]->set_expanding_width(width.value - children[i]->margin.left - children[i]->margin.right - children[i]->border.left - children[i]->border.right - children[i]->padding.left - children[i]->padding.right);
	}
}

void CL_CSSBlockLayout::layout_content(CL_GraphicContext &gc, CL_CSSLayoutCursor &cursor, LayoutStrategy strategy)
{
	if (children.empty() && height.value > 0.0f)
		cursor.apply_margin();

	for (size_t i = 0; i < children.size(); i++)
	{
		if (children[i]->get_element_node()->computed_properties.position.type == CL_CSSBoxPosition::type_absolute ||
			children[i]->get_element_node()->computed_properties.position.type == CL_CSSBoxPosition::type_fixed)
		{
			// We layout absolute or fixed elements later since they may rely on the calculated height of the normal flow.

			children[i]->static_position.left = cursor.x;
			children[i]->static_position.top = cursor.y + cursor.get_total_margin();
			children[i]->static_position.right = children[i]->static_position.left;
			children[i]->static_position.bottom = children[i]->static_position.top;
			children[i]->relative_x = relative_x + children[i]->get_local_relative_x();
			children[i]->relative_y = relative_y + children[i]->get_local_relative_y();
			children[i]->calc_preferred(gc, cursor.resources);
			children[i]->calculate_top_down_sizes();
			children[i]->set_expanding_width(children[i]->preferred_width);
			children[i]->layout_formatting_root_helper(gc, cursor.resources, normal_strategy);
			children[i]->static_position.right = children[i]->static_position.left + children[i]->width.value;
			children[i]->static_position.bottom = children[i]->static_position.top + children[i]->height.value;
		}
		else
		{
			if (children[i]->get_element_node()->is_float() ||
				children[i]->get_element_node()->is_table() ||
				!children[i]->get_element_node()->is_overflow_visible() ||
				children[i]->is_replaced())
			{
				layout_float(cursor, i, gc, strategy);
			}
			else
			{
				children[i]->containing_width = width;
				children[i]->containing_height = height;
				children[i]->layout_normal(gc, cursor, strategy);
			}
			if (strategy != normal_strategy && width.expanding)
				width.value = cl_max(width.value, cl_actual_to_used(children[i]->get_block_width()));
		}
	}

	if (strategy != normal_strategy)
		cursor.apply_written_width(width.value);
}

void CL_CSSBlockLayout::layout_float(CL_CSSLayoutCursor &cursor, size_t i, CL_GraphicContext & gc, LayoutStrategy strategy)
{
	int box_y = cl_used_to_actual(cursor.y+cursor.get_total_margin());
	if (children[i]->get_element_node()->computed_properties.clear.type == CL_CSSBoxClear::type_left || children[i]->get_element_node()->computed_properties.clear.type == CL_CSSBoxClear::type_both)
	{
		int clear_left = formatting_context->find_left_clearance();
		box_y = cl_max(box_y, clear_left);
	}
	if (children[i]->get_element_node()->computed_properties.clear.type == CL_CSSBoxClear::type_right || children[i]->get_element_node()->computed_properties.clear.type == CL_CSSBoxClear::type_both)
	{
		int clear_right = formatting_context->find_right_clearance();
		box_y = cl_max(box_y, clear_right);
	}


	children[i]->containing_width = width;
	children[i]->containing_height = height;
	children[i]->relative_x = relative_x + children[i]->get_local_relative_x();
	children[i]->relative_y = relative_y + children[i]->get_local_relative_y();
	children[i]->layout_formatting_root(gc, cursor.resources, strategy);

	CL_Rect float_box(0, 0, children[i]->get_block_width(), children[i]->get_block_height());
	float_box.translate(cursor.x, box_y);
	if (strategy != normal_strategy && width.expanding)
	{
		if (children[i]->get_element_node()->computed_properties.float_box.type == CL_CSSBoxFloat::type_left)
		{
			float_box = formatting_context->float_left(float_box, cursor.x+1000000);
		}
		else if (children[i]->get_element_node()->computed_properties.float_box.type == CL_CSSBoxFloat::type_right)
		{
			float_box = formatting_context->float_right_shrink_to_fit(float_box, cursor.x+1000000);
		}
		else
		{
			float_box = formatting_context->place_left(float_box, cursor.x+1000000);
			cursor.apply_margin();
			cursor.y = float_box.bottom;
		}
	}
	else
	{
		if (children[i]->get_element_node()->computed_properties.float_box.type == CL_CSSBoxFloat::type_left)
		{
			float_box = formatting_context->float_left(float_box, cursor.x+width.value);
		}
		else if (children[i]->get_element_node()->computed_properties.float_box.type == CL_CSSBoxFloat::type_right)
		{
			float_box.translate(width.value-float_box.get_width(), 0);
			float_box = formatting_context->float_right(float_box, cursor.x+width.value);
		}
		else
		{
			float_box = formatting_context->place_left(float_box, cursor.x+width.value);
			cursor.apply_margin();
			cursor.y = float_box.bottom;
		}
	}

	cursor.apply_written_width(float_box.right);
	children[i]->set_root_block_position(float_box.left, float_box.top);
}

void CL_CSSBlockLayout::layout_absolute_and_fixed_content(CL_GraphicContext &gc, CL_CSSResourceCache *resources, CL_Rect containing_block, const CL_Size &viewport_size)
{
	if (get_element_node()->computed_properties.position.type != CL_CSSBoxPosition::type_static)
	{
		CL_Rect rect = get_padding_box();
		containing_block = rect;
	}

	for (size_t i = 0; i < children.size(); i++)
	{
		if (children[i]->get_element_node()->computed_properties.position.type == CL_CSSBoxPosition::type_absolute ||
			children[i]->get_element_node()->computed_properties.position.type == CL_CSSBoxPosition::type_fixed)
		{
			children[i]->relative_x = relative_x + children[i]->get_local_relative_x();
			children[i]->relative_y = relative_y + children[i]->get_local_relative_y();
			children[i]->layout_absolute_or_fixed(gc, resources, containing_block, viewport_size);
		}

		children[i]->layout_absolute_and_fixed_content(gc, resources, containing_block, viewport_size);
	}
}

bool CL_CSSBlockLayout::add_content_margin_top(CL_CSSLayoutCursor &cursor)
{
	for (size_t i = 0; i < children.size(); i++)
	{
		if (children[i]->add_margin_top(cursor))
			return true;
	}
	return !is_empty();
}

bool CL_CSSBlockLayout::is_empty() const
{
	if (height.use_content)
	{
		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->is_empty())
				return false;
		}
		return true;
	}
	else
	{
		return height.value == 0.0f;
	}
}

void CL_CSSBlockLayout::set_component_geometry()
{
	for (size_t i = 0; i < children.size(); i++)
		children[i]->set_component_geometry();
}

int CL_CSSBlockLayout::get_first_line_baseline()
{
	int first_line_baseline = -1;
	for (size_t i = 0; i < children.size(); i++)
	{
		int baseline = children[i]->get_first_line_baseline();
		if (baseline != -1 && children[i]->is_formatting_context_root())
			baseline += children[i]->get_formatting_context()->get_local_y();

		if (first_line_baseline == -1)
			first_line_baseline = baseline;
		else if (baseline != -1)
			first_line_baseline = cl_min(first_line_baseline, baseline);
	}
	return first_line_baseline;
}

int CL_CSSBlockLayout::get_last_line_baseline()
{
	int last_line_baseline = -1;
	for (size_t i = 0; i < children.size(); i++)
	{
		int baseline = children[i]->get_last_line_baseline();
		if (baseline != -1 && children[i]->is_formatting_context_root())
			baseline += children[i]->get_formatting_context()->get_local_y();

		if (last_line_baseline == -1)
			last_line_baseline = baseline;
		else if (baseline != -1)
			last_line_baseline = cl_max(last_line_baseline, baseline);
	}
	return last_line_baseline;
}
/*
CL_CSSInlineLayout *CL_CSSBlockLayout::find_inline_layout(CL_CSSBoxText *text_node)
{
	for (size_t i = 0; i < children.size(); i++)
	{
		CL_CSSBlockLayout *block_child = dynamic_cast<CL_CSSBlockLayout*>(children[i]);
		CL_CSSInlineLayout *inline_child = dynamic_cast<CL_CSSInlineLayout*>(children[i]);
		CL_CSSTableLayout *table_child = dynamic_cast<CL_CSSTableLayout*>(children[i]);
		if (block_child)
		{
			CL_CSSInlineLayout *inline_layout = block_child->find_inline_layout(text_node);
			if (inline_layout)
				return inline_layout;
		}
		else if (inline_child)
		{
			if (inline_child->contains_node(text_node))
				return inline_child;
		}
		else if (table_child)
		{
			CL_CSSInlineLayout *inline_layout = table_child->find_inline_layout(text_node);
			if (inline_layout)
				return inline_layout;
		}
	}
	return 0;
}
*/
CL_CSSLayoutHitTestResult CL_CSSBlockLayout::hit_test(CL_GraphicContext &gc, CL_CSSResourceCache *resource_cache, const CL_Point &pos) const
{
	for (size_t i = 0; i < children.size(); i++)
	{
		CL_CSSLayoutHitTestResult result = children[i]->hit_test(gc, resource_cache, pos);
		if (result.node)
			return result;
	}
	return CL_CSSLayoutHitTestResult();
}

void CL_CSSBlockLayout::prepare_children()
{
	for (size_t i = 0; i < children.size(); i++)
		children[i]->prepare(formatting_context, stacking_context);
}

bool CL_CSSBlockLayout::find_content_box(CL_CSSBoxElement *element, CL_Rect &out_rect)
{
	for (size_t i = 0; i < children.size(); i++)
	{
		if (children[i]->get_element_node() == element)
		{
			CL_Rect rect = children[i]->content_box;
			rect.translate(formatting_context->get_x(), formatting_context->get_y());
			out_rect = rect;
			return true;
		}
		else
		{
			if (children[i]->find_content_box(element, out_rect))
				return true;
		}
	}
	return false;
}

void CL_CSSBlockLayout::render_layer_background(CL_GraphicContext &gc, CL_CSSResourceCache *resources, bool root)
{
	render_non_content(gc, resources, root);
}

void CL_CSSBlockLayout::render_layer_non_inline(CL_GraphicContext &gc, CL_CSSResourceCache *resources)
{
	for (size_t i = 0; i < children.size(); i++)
	{
		bool is_same_stacking_context = (stacking_context == children[i]->get_stacking_context());
		bool is_positioned = (children[i]->get_element_node()->computed_properties.position.type != CL_CSSBoxPosition::type_static);
		bool is_float = children[i]->get_element_node()->is_float();
		if (is_same_stacking_context && !is_positioned && !is_float)
		{
			children[i]->render_layer_background(gc, resources, false);
			children[i]->render_layer_non_inline(gc, resources);
		}
	}
}

void CL_CSSBlockLayout::render_layer_floats(CL_GraphicContext &gc, CL_CSSResourceCache *resources)
{
	for (size_t i = 0; i < children.size(); i++)
	{
		bool is_same_stacking_context = (stacking_context == children[i]->get_stacking_context());
		bool is_positioned = (children[i]->get_element_node()->computed_properties.position.type != CL_CSSBoxPosition::type_static);
		bool is_float = children[i]->get_element_node()->is_float();
		if (is_same_stacking_context && !is_positioned)
		{
			if (is_float)
			{
				children[i]->render_layer_background(gc, resources, false);
				children[i]->render_layer_non_inline(gc, resources);
				children[i]->render_layer_floats(gc, resources);
				children[i]->render_layer_inline(gc, resources);
			}
			else
			{
				children[i]->render_layer_floats(gc, resources);
			}
		}
	}
}

void CL_CSSBlockLayout::render_layer_inline(CL_GraphicContext &gc, CL_CSSResourceCache *resources)
{
	for (size_t i = 0; i < children.size(); i++)
	{
		bool is_same_stacking_context = (stacking_context == children[i]->get_stacking_context());
		bool is_positioned = (children[i]->get_element_node()->computed_properties.position.type != CL_CSSBoxPosition::type_static);
		bool is_float = children[i]->get_element_node()->is_float();
		if (is_same_stacking_context && !is_positioned && !is_float)
		{
			children[i]->render_layer_inline(gc, resources);
		}
	}
}

void CL_CSSBlockLayout::render_layer_positioned(CL_GraphicContext &gc, CL_CSSResourceCache *resources)
{
	for (size_t i = 0; i < children.size(); i++)
	{
		bool is_same_stacking_context = (stacking_context == children[i]->get_stacking_context());
		bool is_positioned = (children[i]->get_element_node()->computed_properties.position.type != CL_CSSBoxPosition::type_static);
		bool is_float = children[i]->get_element_node()->is_float();
		int level = children[i]->get_stacking_context()->get_level();
		if (is_same_stacking_context)
		{
			if (is_positioned)
			{
				children[i]->render_layer_background(gc, resources, false);
				children[i]->render_layer_non_inline(gc, resources);
				children[i]->render_layer_floats(gc, resources);
				children[i]->render_layer_inline(gc, resources);
			}
			children[i]->render_layer_positioned(gc, resources);
		}
		else if (!is_same_stacking_context && level == 0)
		{
			children[i]->get_stacking_context()->render(gc, resources);
		}
	}
}
