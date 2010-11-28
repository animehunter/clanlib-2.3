/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "css_table_layout.h"
#include "css_table_column.h"
#include "css_table_row.h"
#include "css_block_layout.h"
#include "css_inline_layout.h"
#include "css_layout_cursor.h"
#include "css_block_formatting_context.h"
#include "../css_resource_cache.h"
#include "../BoxTree/css_box_element.h"
#include "../BoxTree/css_box_text.h"
#include "clan_image_stretch.h"

CL_CSSTableLayout::CL_CSSTableLayout(CL_CSSBoxElement *block_element)
: CL_CSSLayoutTreeNode(block_element), next_column(0)
{
}

CL_CSSTableLayout::~CL_CSSTableLayout()
{
	for (size_t i = 0; i < columns.size(); i++)
	{
		for (size_t j = 0; j < columns[i].rows.size(); j++)
		{
			delete columns[i].rows[j];
		}
	}
}

void CL_CSSTableLayout::add_row(CL_CSSBoxElement *row_element)
{
	next_column = 0;
	CL_CSSTableRow row;
	row.element = row_element;
	rows.push_back(row);
}

void CL_CSSTableLayout::add_cell(CL_CSSLayoutTreeNode *cell_layout)
{
	// To do: Implement row and cell span
	if (columns.size() < next_column + 1)
		columns.resize(next_column + 1);
	if (columns[next_column].rows.size() + 1 < rows.size())
		columns[next_column].rows.resize(rows.size() - 1);
	columns[next_column].rows.push_back(cell_layout);
	if (cell_layout->get_element_node()->computed_properties.width.type == CL_CSSBoxWidth::type_clan_expanding)
		columns[next_column].expanding = true;
	next_column++;
}

void CL_CSSTableLayout::calculate_content_top_down_sizes()
{
	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (columns[cell].rows.size() > row && columns[cell].rows[row])
			{
				columns[cell].rows[row]->containing_width = width;
				columns[cell].rows[row]->containing_height = height;
				columns[cell].rows[row]->calculate_content_top_down_sizes();
			}
		}
	}
}

void CL_CSSTableLayout::layout_content(CL_GraphicContext &gc, CL_CSSLayoutCursor &cursor, LayoutStrategy strategy)
{
	cursor.apply_margin();
	CL_CSSUsedValue start_y = cursor.y;
	calculate_cell_widths(gc, cursor, strategy);
	layout_cells(gc, cursor);
	position_cells(cursor);
	height.value = cl_max(height.value, cursor.y-start_y);
}

void CL_CSSTableLayout::render(CL_GraphicContext &gc, CL_CSSResourceCache *resources)
{
	render_non_content(gc, resources);
	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (columns[cell].rows.size() > row && columns[cell].rows[row])
			{
				render_cell_non_content(gc, resources, row, cell);
				columns[cell].rows[row]->render(gc, resources);
			}
		}
	}
}

CL_Rect CL_CSSTableLayout::get_cell_border_box(size_t row, size_t col)
{
	CL_CSSUsedValue left = 0.0f;
	CL_CSSUsedValue top = 0.0f;
	CL_CSSUsedValue right = 0.0f;
	CL_CSSUsedValue bottom = 0.0f;

	for (size_t i = 0; i < col; i++)
		left += size_grid.get_non_content_width(i) + size_grid.get_width(i);
	left += size_grid.get_non_content_width(col);

	for (size_t i = 0; i < row; i++)
		top += size_grid.get_non_content_height(i) + size_grid.get_height(i);
	top += size_grid.get_non_content_height(row);

	right = left + size_grid.get_width(col);
	bottom = top + size_grid.get_height(row);

	left -= columns[col].rows[row]->border.left;
	right += columns[col].rows[row]->border.right;
	top -= columns[col].rows[row]->border.top;
	bottom += columns[col].rows[row]->border.bottom;

	CL_Rect box((int)(left+0.5f),(int)(top+0.5f),(int)(right+0.5f),(int)(bottom+0.5f));
	box.translate(formatting_context->get_x(), formatting_context->get_y());
	return box;
}

void CL_CSSTableLayout::render_cell_non_content(CL_GraphicContext &gc, CL_CSSResourceCache *resources, size_t row, size_t col)
{
	CL_CSSLayoutTreeNode *cell = columns[col].rows[row];

	CL_Rect border_box = get_cell_border_box(row, col);
	if (cell->get_element_node()->computed_properties.background_color.type == CL_CSSBoxBackgroundColor::type_color)
		CL_Draw::fill(gc, border_box, cell->get_element_node()->computed_properties.background_color.color);

	if (cell->get_element_node()->computed_properties.background_image.type == CL_CSSBoxBackgroundImage::type_uri)
	{
		CL_Image &image = resources->get_image(gc, cell->get_element_node()->computed_properties.background_image.url);
		if (!image.is_null())
		{
			if (cell->get_element_node()->computed_properties.background_repeat.type == CL_CSSBoxBackgroundRepeat::type_repeat)
			{
				image.draw(gc, border_box);
			}
/*			else if (cell->get_element_node()->computed_properties.background_repeat.type == CL_CSSBoxBackgroundRepeat::type_clan_stretch)
			{
				int sizing_left = (int)(cell->get_element_node()->computed_properties.clan_background_border_left.length.value+0.5f);
				int sizing_top = (int)(cell->get_element_node()->computed_properties.clan_background_border_top.length.value+0.5f);
				int sizing_right = (int)(cell->get_element_node()->computed_properties.clan_background_border_right.length.value+0.5f);
				int sizing_bottom = (int)(cell->get_element_node()->computed_properties.clan_background_border_bottom.length.value+0.5f);
				CL_ClanImageStretch::draw_image(gc, border_box, image, sizing_left, sizing_top, sizing_right, sizing_bottom);
			}*/
		}
	}

	if (cell->get_element_node()->computed_properties.border_style_top.type == CL_CSSBoxBorderStyle::type_solid)
	{
		CL_Draw::fill(
			gc,
			(float)border_box.left,
			(float)border_box.top,
			(float)border_box.right,
			(float)(border_box.top+cell->border.top),
			cell->get_element_node()->computed_properties.border_color_top.color);
	}
	if (cell->get_element_node()->computed_properties.border_style_bottom.type == CL_CSSBoxBorderStyle::type_solid)
	{
		CL_Draw::fill(
			gc,
			(float)border_box.left,
			(float)(border_box.bottom-cell->border.bottom),
			(float)border_box.right,
			(float)border_box.bottom,
			cell->get_element_node()->computed_properties.border_color_bottom.color);
	}
	if (cell->get_element_node()->computed_properties.border_style_left.type == CL_CSSBoxBorderStyle::type_solid)
	{
		CL_Draw::fill(
			gc,
			(float)border_box.left,
			(float)(border_box.top+cell->border.top),
			(float)(border_box.left+cell->border.left),
			(float)(border_box.bottom-cell->border.bottom),
			cell->get_element_node()->computed_properties.border_color_left.color);
	}
	if (cell->get_element_node()->computed_properties.border_style_right.type == CL_CSSBoxBorderStyle::type_solid)
	{
		CL_Draw::fill(
			gc,
			(float)(border_box.right-cell->border.right),
			(float)(border_box.top+cell->border.top),
			(float)border_box.right,
			(float)(border_box.bottom-cell->border.bottom),
			cell->get_element_node()->computed_properties.border_color_right.color);
	}
}

void CL_CSSTableLayout::set_component_geometry()
{
	for (size_t row = 0; row < rows.size(); row++)
		for (size_t cell = 0; cell < columns.size(); cell++)
			if (columns[cell].rows.size() > row && columns[cell].rows[row])
				columns[cell].rows[row]->set_component_geometry();
}

int CL_CSSTableLayout::get_first_line_baseline()
{
	int first_line_baseline = -1;
	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (columns[cell].rows.size() > row && columns[cell].rows[row])
			{
				int baseline = columns[cell].rows[row]->get_first_line_baseline();
				if (first_line_baseline == -1)
					first_line_baseline = baseline;
				else if (baseline != -1)
					first_line_baseline = cl_min(first_line_baseline, baseline);
			}
		}
	}
	if (first_line_baseline != -1 && formatting_context_root)
		first_line_baseline += formatting_context->get_local_y();
	return first_line_baseline;
}

int CL_CSSTableLayout::get_last_line_baseline()
{
	int last_line_baseline = -1;
	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (columns[cell].rows.size() > row && columns[cell].rows[row])
			{
				int baseline = columns[cell].rows[row]->get_last_line_baseline();
				if (last_line_baseline == -1)
					last_line_baseline = baseline;
				else if (baseline != -1)
					last_line_baseline = cl_max(last_line_baseline, baseline);
			}
		}
	}
	if (last_line_baseline != -1 && formatting_context_root)
		last_line_baseline += formatting_context->get_local_y();
	return last_line_baseline;
}

CL_CSSLayoutHitTestResult CL_CSSTableLayout::hit_test(CL_GraphicContext &gc, CL_CSSResourceCache *resource_cache, const CL_Point &pos) const
{
	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			CL_Rect r = columns[cell].rows[row]->content_box;
			r.translate(formatting_context->get_x(), formatting_context->get_y());
			if (r.contains(pos))
			{
				CL_CSSLayoutHitTestResult result = columns[cell].rows[row]->hit_test(gc, resource_cache, pos);
				if (result.node)
					return result;
			}
		}
	}
	return CL_CSSLayoutHitTestResult();
}

CL_CSSInlineLayout *CL_CSSTableLayout::find_inline_layout(CL_CSSBoxText *text_node)
{
	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			CL_CSSBlockLayout *block_child = dynamic_cast<CL_CSSBlockLayout*>(columns[cell].rows[row]);
			CL_CSSInlineLayout *inline_child = dynamic_cast<CL_CSSInlineLayout*>(columns[cell].rows[row]);
			CL_CSSTableLayout *table_child = dynamic_cast<CL_CSSTableLayout*>(columns[cell].rows[row]);
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
	}
	return 0;
}

void CL_CSSTableLayout::prepare_children()
{
	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (columns[cell].rows.size() > row && columns[cell].rows[row])
			{
				columns[cell].rows[row]->prepare(formatting_context, stacking_context);
			}
		}
	}
}

void CL_CSSTableLayout::calculate_minimum_cell_widths(CL_GraphicContext & gc, CL_CSSLayoutCursor & cursor)
{
	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (row == 0)
				columns[cell].minimum_width = 0;

			if (columns[cell].rows.size() > row)
			{
				if (columns[cell].rows[row])
				{
					columns[cell].rows[row]->calc_minimum(gc, cursor.resources);
					CL_CSSUsedValue cell_width = columns[cell].rows[row]->min_width;
					cell_width += columns[cell].rows[row]->padding.left + columns[cell].rows[row]->padding.right;

					columns[cell].minimum_width = cl_max(columns[cell].minimum_width, cell_width);
				}
			}
		}
	}
}

void CL_CSSTableLayout::calculate_preferred_cell_widths(CL_GraphicContext & gc, CL_CSSLayoutCursor & cursor)
{
	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (row == 0)
				columns[cell].maximum_width = 0;

			if (columns[cell].rows.size() > row)
			{
				if (columns[cell].rows[row])
				{
					columns[cell].rows[row]->calc_preferred(gc, cursor.resources);
					CL_CSSUsedValue cell_width = cell_width = columns[cell].rows[row]->preferred_width;
					cell_width += columns[cell].rows[row]->padding.left + columns[cell].rows[row]->padding.right;

					columns[cell].maximum_width = cl_max(columns[cell].maximum_width, cell_width);
				}
			}
		}
	}
}

CL_CSSTableSizeGrid CL_CSSTableLayout::create_preferred_width_grid(CL_GraphicContext & gc, CL_CSSLayoutCursor & cursor)
{
	calculate_preferred_cell_widths(gc, cursor);
	CL_CSSTableSizeGrid grid(rows.size(), columns.size());
	apply_non_content(grid);
	for (size_t col = 0; col < columns.size(); col++)
		grid.apply_width(col, columns[col].maximum_width);
	return grid;
}

CL_CSSTableSizeGrid CL_CSSTableLayout::create_minimum_width_grid(CL_GraphicContext & gc, CL_CSSLayoutCursor & cursor)
{
	calculate_minimum_cell_widths(gc, cursor);
	CL_CSSTableSizeGrid grid(rows.size(), columns.size());
	apply_non_content(grid);
	for (size_t col = 0; col < columns.size(); col++)
		grid.apply_width(col, columns[col].minimum_width);
	return grid;
}

void CL_CSSTableLayout::apply_non_content(CL_CSSTableSizeGrid &grid)
{
	if (element_node->computed_properties.border_collapse.type == CL_CSSBoxBorderCollapse::type_separate)
	{
		// grid.apply_separate_table_border(border.left, border.top, border.right, border.bottom, padding.left, padding.top, padding.right, padding.bottom);
		grid.apply_separate_spacing(element_node->computed_properties.border_spacing.length1.value, element_node->computed_properties.border_spacing.length2.value);
	}
	else
	{
		// bug: table border has already been applied by the parent but needs to be adjusted
		grid.apply_collapsed_table_border(border.left, border.top, border.right, border.bottom);
	}

	if (element_node->computed_properties.border_collapse.type == CL_CSSBoxBorderCollapse::type_separate)
	{
		for (size_t col = 0; col < columns.size(); col++)
		{
			CL_CSSUsedValue border_width_left = 0;
			CL_CSSUsedValue border_width_right = 0;
			for (size_t row = 0; row < rows.size(); row++)
			{
				border_width_left = cl_max(border_width_left, columns[col].rows[row]->border.left);
				border_width_right = cl_max(border_width_right, columns[col].rows[row]->border.right);
			}
			grid.apply_separate_border_width(col, border_width_left, border_width_right);
		}

		for (size_t row = 0; row < rows.size(); row++)
		{
			CL_CSSUsedValue border_height_top = 0;
			CL_CSSUsedValue border_height_bottom = 0;
			for (size_t col = 0; col < columns.size(); col++)
			{
				border_height_top = cl_max(border_height_top, columns[col].rows[row]->border.top);
				border_height_bottom = cl_max(border_height_bottom, columns[col].rows[row]->border.bottom);
			}
			grid.apply_separate_border_height(row, border_height_top, border_height_bottom);
		}
	}
	else
	{
		for (size_t col = 0; col < columns.size(); col++)
		{
			for (size_t row = 0; row < rows.size(); row++)
			{
				CL_CSSLayoutTreeNode *cell_node = columns[col].rows[row];
				grid.apply_collapsed_border(row, col, cell_node->border.left, cell_node->border.top, cell_node->border.right, cell_node->border.bottom);
			}
		}
	}
}

void CL_CSSTableLayout::calculate_cell_widths(CL_GraphicContext & gc, CL_CSSLayoutCursor & cursor, LayoutStrategy strategy)
{
	if (strategy != normal_strategy && (width.expanding || containing_width.expanding))
	{
		if (strategy == minimum_strategy)
		{
			size_grid = create_minimum_width_grid(gc, cursor);
		}
		else
		{
			size_grid = create_preferred_width_grid(gc, cursor);
		}
	}
	else
	{
		size_grid = create_preferred_width_grid(gc, cursor);
		if (element_node->computed_properties.width.type == CL_CSSBoxWidth::type_auto)
		{
			if (size_grid.get_table_width() > containing_width.value)
			{
				size_grid = create_minimum_width_grid(gc, cursor);
				if (size_grid.get_table_width() < containing_width.value)
					size_grid.expand_table_width(containing_width.value);
			}
		}
		else
		{
			if (size_grid.get_table_width() <= width.value)
			{
				size_grid.expand_table_width(width.value);
			}
			else
			{
				size_grid = create_minimum_width_grid(gc, cursor);
			}
		}
	}

	for (size_t col = 0; col < columns.size(); col++)
	{
		columns[col].cell_width = size_grid.get_width(col);
	}

	width.value = size_grid.get_table_width();
}

void CL_CSSTableLayout::layout_cells(CL_GraphicContext & gc, CL_CSSLayoutCursor & cursor)
{
	int rows_expanding = 0;
	CL_CSSUsedValue height_used = 0;
	for (size_t row = 0; row < rows.size(); row++)
	{
		if (rows[row].element->computed_properties.height.type == CL_CSSBoxWidth::type_clan_expanding)
		{
			rows_expanding++;
			continue;
		}

		switch (rows[row].element->computed_properties.height.type)
		{
		default:
		case CL_CSSBoxHeight::type_auto:
			rows[row].height = 0.0f;
			break;
		case CL_CSSBoxHeight::type_length:
			rows[row].height = rows[row].element->computed_properties.height.length.value;
			break;
		case CL_CSSBoxHeight::type_percentage:
			rows[row].height = height.use_content ? 0.0f : height.value * rows[row].element->computed_properties.height.percentage / 100.0f;
			break;
		}

		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (columns[cell].rows.size() > row)
			{
				if (columns[cell].rows[row])
				{
					CL_CSSUsedValue width = columns[cell].cell_width - columns[cell].rows[row]->padding.left - columns[cell].rows[row]->padding.right;
					columns[cell].rows[row]->calculate_top_down_sizes();
					columns[cell].rows[row]->set_expanding_width(width);
					columns[cell].rows[row]->layout_formatting_root_helper(gc, cursor.resources, normal_strategy);
					CL_CSSUsedValue cell_height = columns[cell].rows[row]->height.value + columns[cell].rows[row]->padding.top + columns[cell].rows[row]->padding.bottom;
					rows[row].height = cl_max(rows[row].height, cell_height);
				}
			}
		}
		height_used += rows[row].height;
	}

	if (rows_expanding > 0)
	{
		CL_CSSUsedValue extra_height = height.value - height_used;
		for (size_t row = 0; row < rows.size(); row++)
		{
			if (rows[row].element->computed_properties.height.type == CL_CSSBoxWidth::type_clan_expanding)
			{
				for (size_t cell = 0; cell < columns.size(); cell++)
				{
					if (columns[cell].rows.size() > row)
					{
						if (columns[cell].rows[row])
						{
							CL_CSSUsedValue width = columns[cell].cell_width - columns[cell].rows[row]->padding.left - columns[cell].rows[row]->padding.right;
							columns[cell].rows[row]->calculate_top_down_sizes();
							columns[cell].rows[row]->width.value = width;
							columns[cell].rows[row]->layout_formatting_root_helper(gc, cursor.resources, normal_strategy);
						}
					}
				}
				rows[row].height = extra_height/rows_expanding;
			}
		}
	}

	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t col = 0; col < columns.size(); col++)
		{
			CL_CSSUsedValue cell_height = columns[col].rows[row]->height.value + columns[col].rows[row]->padding.top + columns[col].rows[row]->padding.bottom;
			size_grid.apply_height(row, cell_height);
		}
	}
}

void CL_CSSTableLayout::position_cells(CL_CSSLayoutCursor &cursor)
{
	CL_CSSUsedValue y = 0;
	for (size_t row = 0; row < rows.size(); row++)
	{
		y += size_grid.get_non_content_height(row);

		CL_CSSUsedValue x = 0;
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			x += size_grid.get_non_content_width(cell);

			if (columns[cell].rows.size() > row)
			{
				if (columns[cell].rows[row])
				{
					CL_CSSUsedValue offset_border_left = columns[cell].rows[row]->border.left;
					CL_CSSUsedValue offset_border_top = columns[cell].rows[row]->border.top;
					if (columns[cell].rows[row]->get_element_node()->computed_properties.vertical_align.type == CL_CSSBoxVerticalAlign::type_middle)
					{
						CL_CSSUsedValue offset = size_grid.get_height(row)/2.0f - columns[cell].rows[row]->height.value/2.0f;
						columns[cell].rows[row]->set_root_block_position(cursor.x + x - offset_border_left, cursor.y + y + offset - offset_border_top);
					}
					else
					{
						columns[cell].rows[row]->set_root_block_position(cursor.x + x - offset_border_left, cursor.y + y - offset_border_top);
					}
				}
			}

			x += size_grid.get_width(cell);
		}
		cursor.apply_written_width(cursor.x + size_grid.get_table_width());
		y += size_grid.get_height(row);
	}
	if (!rows.empty())
		y += size_grid.get_non_content_height(rows.size());
	cursor.y += y;
}
