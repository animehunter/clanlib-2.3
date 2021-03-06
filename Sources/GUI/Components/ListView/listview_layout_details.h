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

/// \addtogroup clanGUI_Components clanGUI Components
/// \{


#pragma once


#include "listview_layout.h"
#include "API/GUI/gui_theme_part_property.h"

class CL_ListViewLayoutDetails : public CL_ListViewLayout
{
/// \name Construction
/// \{
public:
	CL_ListViewLayoutDetails(CL_ListView *listview);
	virtual ~CL_ListViewLayoutDetails();


/// \}
/// \name Attributes
/// \{
public:
	virtual ListViewShownItem get_item_at(const CL_Point &point);
	virtual CL_Size get_total_size();
	virtual CL_ListViewItem get_neighbour(CL_ListViewItem item, Neighbour neighbour);
	virtual std::vector<ListViewShownItem> &get_shown_items();
	virtual std::vector<ListViewColumn> &get_columns();
	virtual std::vector<ListViewRow> &get_rows();
	virtual CL_Rect get_lineedit_rect(CL_ListViewItem &si, const CL_Size &size) const;

/// \}
/// \name Operations
/// \{
public:
	virtual void create_parts();

	virtual void set_show_detail_icon(bool enable) { show_detail_icon = enable; invalidate(); }
	virtual void set_show_detail_opener(bool enable) { show_detail_opener = enable; invalidate(); }

/// \}
/// \name Callbacks
/// \{
public:

/// \}
/// \name Implementation
/// \{
public:
	void update_shown_items_rows(CL_Font &font, CL_ListViewItem item);
	CL_Rect get_opener_rect(const CL_Rect &cell_content_rect, CL_ListViewItem item, int offset_x);
	CL_Rect get_icon_rect(const CL_Rect &cell_content_rect, CL_ListViewItem item, int offset_x);

	std::vector<ListViewShownItem> shown_items;
	CL_Size size_opener;
	CL_Point opener_offset;
	CL_Rect rect_cell_shrink;
	int icon_offset_y;
	int opener_gap;
	int icon_gap;
	int indent_width;
	int max_rows_visible;
	int row_draw_y_pos;
	bool columns_valid;

	CL_GUIThemePartProperty prop_opener_gap;
	CL_GUIThemePartProperty prop_opener_offset_x;
	CL_GUIThemePartProperty prop_opener_offset_y;
	CL_GUIThemePartProperty prop_icon_text_gap;
	CL_GUIThemePartProperty prop_icon_width;
	CL_GUIThemePartProperty prop_icon_height;
	CL_GUIThemePartProperty prop_icon_offset_y;
	CL_GUIThemePartProperty prop_indent_width;
	CL_GUIThemePartProperty prop_lineedit_offset_left;
	CL_GUIThemePartProperty prop_lineedit_offset_top;
	CL_GUIThemePartProperty prop_lineedit_offset_right;
	CL_GUIThemePartProperty prop_lineedit_offset_bottom;
	bool show_detail_icon;
	bool show_detail_opener;
/// \}
};


/// \}
