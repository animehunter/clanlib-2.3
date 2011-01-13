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
**    Harry Storbacka
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_theme.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/GUI/Components/listview.h"
#include "API/GUI/Components/listview_header.h"
#include "API/GUI/Components/listview_column_data.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_metrics.h"
#include "../../gui_css_strings.h"
#include "listview_layout.h"
#include "listview_layout_thumbnails.h"


/////////////////////////////////////////////////////////////////////////////
// CL_ListViewLayoutThumbnails Construction:

CL_ListViewLayoutThumbnails::CL_ListViewLayoutThumbnails(CL_ListView *listview)
: CL_ListViewLayout(listview), opener_gap(0), icon_gap(0), indent_width(0),
  max_rows_visible(0), max_grid_cols_visible(0), row_draw_y_pos(0)
{
	prop_icon_width = CL_GUIThemePartProperty(CssStr::icon_width, "100");
	prop_icon_height = CL_GUIThemePartProperty(CssStr::icon_height, "100");
	prop_line_edit_offset_left = CL_GUIThemePartProperty("line-edit-offset-left");
	prop_line_edit_offset_top = CL_GUIThemePartProperty("line-edit-offset-top");
	prop_line_edit_offset_bottom = CL_GUIThemePartProperty("line-edit-offset-bottom");
	prop_line_edit_offset_right = CL_GUIThemePartProperty("line-edit-offset-right");
}

CL_ListViewLayoutThumbnails::~CL_ListViewLayoutThumbnails()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewLayoutThumbnails Attributes:

ListViewShownItem CL_ListViewLayoutThumbnails::get_item_at(const CL_Point &point)
{
	std::vector<ListViewShownItem>::iterator it;
	for (it = shown_items.begin(); it!=shown_items.end(); ++it)
	{
		const CL_Rect &r = (*it).rect_cell[0];
		if (r.contains(point))
			return (*it);
	}

	return ListViewShownItem();
}

CL_Size CL_ListViewLayoutThumbnails::get_total_size()
{
	update_rows_cols_visible();

	int items = root_item.get_child_count();
	int rows = items/max_grid_cols_visible + ((items%max_grid_cols_visible) > 0);

	int total_height = rows * size_cell.height;

	return CL_Size(rect_view.get_width(), total_height);
}

CL_ListViewItem CL_ListViewLayoutThumbnails::get_neighbour(CL_ListViewItem item, Neighbour neighbour)
{
	CL_ListViewItem retval = item;

	if (neighbour == neighbour_left)
	{
		retval = item.get_prev_sibling();
	}
	else if (neighbour == neighbour_right)
	{
		retval = item.get_next_sibling();
	}
	else if (neighbour == neighbour_up)
	{
		int i = 0;
		while (i < (max_grid_cols_visible-1) && !retval.is_null())
		{
			retval = retval.get_prev_sibling();
			i++;
		}
	}
	else if (neighbour == neighbour_down)
	{
		int i = 0;
		retval = item;
		while (i < (max_grid_cols_visible-1) && !retval.is_null())
		{
			retval = retval.get_next_sibling();
			i++;
		}
	}

	if (retval.is_null())
		return item;

	return retval;
}

std::vector<ListViewShownItem> &CL_ListViewLayoutThumbnails::get_shown_items()
{
	if (valid)
		return shown_items;

	update_rows_cols_visible();

	shown_items.clear();
	rows.clear();
	row_draw_y_pos = rect_view.top - scroll_y;
	row_counter = 0;

	CL_Font font = part_cell.get_font();
	update_shown_items_grid(font, root_item);

	valid = true;
	return shown_items;
}

std::vector<ListViewColumn> &CL_ListViewLayoutThumbnails::get_columns()
{
	if (columns_valid)
		return columns;

	columns.clear();

	ListViewColumn col;
	col.header = header->get_first_column();
	col.rect = rect_view;
	columns.push_back(col);

	columns_valid = true;
	return columns;	
}

std::vector<ListViewRow> &CL_ListViewLayoutThumbnails::get_rows()
{
	std::vector<ListViewShownItem> &foo = get_shown_items(); // updates rows, if necessary
	return rows;
}


void CL_ListViewLayoutThumbnails::create_parts()
{
	part_row = CL_GUIThemePart(listview, "row");
	part_opener = CL_GUIThemePart(listview, "row_opener");
	part_cell = CL_GUIThemePart(listview, "cell");

	part_cell.set_state("thumbnails", true);
	part_cell.set_state(CssStr::normal, true);
	part_row.set_state(CssStr::normal, true);

	size_icon.width = part_cell.get_property_int(prop_icon_width);
	size_icon.height = part_cell.get_property_int(prop_icon_height);

	CL_Font font = part_cell.get_font();
	height_text = font.get_text_size(gc, "l").height;

	size_cell = part_cell.get_preferred_size();
	height_row = size_cell.height;

	update_rows_cols_visible();

	lineedit_textrect_offset.left = part_cell.get_property_int(prop_line_edit_offset_left);
	lineedit_textrect_offset.top = part_cell.get_property_int(prop_line_edit_offset_top);
	lineedit_textrect_offset.bottom = part_cell.get_property_int(prop_line_edit_offset_bottom);
	lineedit_textrect_offset.right = part_cell.get_property_int(prop_line_edit_offset_right);

	CL_GUIThemePart part_icon_selection(listview, "icon_selection");
	icon_sel_shrink_box = part_icon_selection.get_content_shrink_box();

	rect_cell_shrink = part_cell.get_content_shrink_box();
}

CL_Rect CL_ListViewLayoutThumbnails::get_lineedit_rect(CL_ListViewItem &item, const CL_Size &text_size) const
{
	if (shown_items.empty())
		return CL_Rect(0,0,0,0);
	
	ListViewShownItem si;

	// 1. Find shown items, si.
	std::vector<ListViewShownItem>::const_iterator it;
	for (it=shown_items.begin(); it!=shown_items.end(); ++it)
	{
		if ((*it).item == item)
		{
			si = (*it);
			break;
		}
	}

	if (si.valid == false)
		return CL_Rect(0,0,0,0);


	// 2. Calculate text rect for 'str'.
	CL_Rect text_rect = si.rect_text[0];
	text_rect.right = text_rect.left + text_size.width + 3; // add a few pixels extra to allow cursor to be at end of string without scrolling.

	// 3. Expand text rect with line-edit-offset values defined in css.

	text_rect.top -= lineedit_textrect_offset.top;
	text_rect.left -= lineedit_textrect_offset.left;
	text_rect.bottom += lineedit_textrect_offset.bottom;
	text_rect.right += lineedit_textrect_offset.right;

	return text_rect;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewLayoutThumbnails Implementation:

void CL_ListViewLayoutThumbnails::update_shown_items_grid(CL_Font &font, CL_ListViewItem doc_item)
{
	CL_GraphicContext &gc = listview->get_gc();
	shown_items.clear();
	rows.clear();
	row_counter = 0;

	CL_ListViewItem item = doc_item.get_first_child();

	int x = rect_view.left;
	int y = rect_view.top - scroll_y;

	int skip_items = (scroll_y/size_cell.height) * max_grid_cols_visible;
	int skip_rows = skip_items/max_grid_cols_visible;
	y += skip_rows * size_cell.height;

	int skip_item_counter = 0;

	while (item.is_item())
	{
		if (skip_item_counter < skip_items)
		{
			item = item.get_next_sibling();
			skip_item_counter++;
			continue;
		}

		ListViewShownItem si;
		si.valid = true;
		si.item = item;
		CL_Rect rect_cell(x, y, x+size_cell.width, y+size_cell.height);
		si.rect_cell.push_back(rect_cell);
		rect_cell.shrink(rect_cell_shrink.left, rect_cell_shrink.top, rect_cell_shrink.right, rect_cell_shrink.bottom); // part_cell.get_content_box(si.rect_cell);
		CL_Rect rect_cell_content = rect_cell;
		si.rect_icon = get_icon_rect(rect_cell_content, item, rect_cell_content.left);
		si.rect_icon_selection = si.rect_icon;
		si.rect_icon_selection.shrink(icon_sel_shrink_box.left, icon_sel_shrink_box.top, icon_sel_shrink_box.right, icon_sel_shrink_box.bottom);

		const CL_StringRef &text = si.item.get_column(header->get_first_column().get_column_id()).get_text();
		CL_Size text_size = font.get_text_size(gc, text);
		CL_Rect text_rect;
		text_rect.left = rect_cell_content.get_center().x - text_size.width/2;
		text_rect.top = rect_cell_content.bottom - text_size.height;
		text_rect.set_size(text_size);
		si.descent = font.get_font_metrics(gc).get_descent();
		si.rect_text.push_back(text_rect);

		shown_items.push_back(si);

		x += size_cell.width;

		if (x + size_cell.width > rect_view.right)
		{
			row_counter++;

			if (row_counter > (max_rows_visible+1))
				break;
			y += size_cell.height;
			x = rect_view.left;
		}


		item = item.get_next_sibling();
	}

	valid = true;
}

CL_Rect CL_ListViewLayoutThumbnails::get_icon_rect(const CL_Rect &cell_content_rect, CL_ListViewItem item, int offset_x)
{
	int icon_x = cell_content_rect.get_center().x - size_icon.width/2;
	int icon_y = cell_content_rect.top - icon_sel_shrink_box.top;
	CL_Rect rect_icon(CL_Point(icon_x, icon_y), size_icon);
	return rect_icon;
}

void CL_ListViewLayoutThumbnails::update_rows_cols_visible()
{
	max_rows_visible = rect_view.get_height() / size_cell.height + 1;
	max_grid_cols_visible = rect_view.get_width() / size_cell.width;

	if (max_grid_cols_visible == 0)
		max_grid_cols_visible = 1;

	if (max_rows_visible == 0)
		max_rows_visible = 1;
}
