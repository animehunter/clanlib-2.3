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
**    Harry Storbacka
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_component_description.h"
#include "API/Core/System/timer.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/GUI/Components/listview_item.h"
#include "API/GUI/Components/listview_column_data.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Font/font.h"
#include "API/Display/2D/sprite.h"
#include "API/Display/Window/keys.h"
#include "listview_layout.h"
#include "API/GUI/Components/listview_header.h"
#include "API/GUI/Components/listview_selected_item.h"
#include "API/GUI/Components/listview.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewLayout Construction:

CL_ListViewLayout::CL_ListViewLayout(CL_ListView *listview)
: listview(listview), gc(listview->get_gc()), header(0), scroll_x(0), scroll_y(0),
  height_row(0), height_text(0), row_counter(0), valid(false), columns_valid(false)
{
}

CL_ListViewLayout::~CL_ListViewLayout()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewLayout Attributes:

/*
CL_Size CL_ListViewLayout::get_total_size()
{
	return CL_Size(0,0);
}

CL_ListViewItem CL_ListViewLayout::get_neighbour(Neighbour neighbour)
{
	return CL_ListViewItem();
}*/

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewLayout Operations:

void CL_ListViewLayout::set_root_item(CL_ListViewItem &item)
{
	root_item = item;
}

void CL_ListViewLayout::set_view_rect(const CL_Rect &rect)
{
	rect_view = rect;
}

void CL_ListViewLayout::set_scroll_offset(const CL_Point &point)
{
	scroll_x = point.x;
	scroll_y = point.y;
}

void CL_ListViewLayout::set_listview_header(CL_ListViewHeader *header)
{
	this->header = header;
}

void CL_ListViewLayout::invalidate()
{
	valid = false;
	columns_valid = false;
}

bool CL_ListViewLayout::is_visible(CL_ListViewItem &item)
{
	std::vector<ListViewShownItem> &shown_items = get_shown_items();	
	std::vector<ListViewShownItem>::iterator it;
	for (it = shown_items.begin(); it != shown_items.end(); ++it)
	{
		if ((*it).item.impl == item.impl)
		{
			if ((*it).rect_cell[0].top > header->get_preferred_height() && (*it).rect_cell[0].bottom < rect_view.bottom)
				return true;
		}
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewLayout Implementation:




