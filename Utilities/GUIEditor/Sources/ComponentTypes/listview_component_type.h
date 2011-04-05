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
**    Kenneth Gangstoe
*/

#pragma once

#include "component_type.h"
#include "../MainWindow/gui_editor_window.h"
#include "../MainWindow/edit_columns_window.h"
#include "../GridComponent/grid_component.h"
#include "../GridComponent/grid_object.h"

class ListViewComponentType : public ComponentType
{
public:
	ListViewComponentType(const CL_String &xmlname, const CL_StringRef &name, int id, const CL_StringRef &icon)	
	: ComponentType(xmlname, name, id, icon) {}

	virtual CL_GUIComponent *create_component(CL_GUIComponent *parent) const
	{
		CL_ListView *component = new CL_ListView(parent);
		component->set_geometry(CL_Rect(0, 0, 200, 100));
		return component;
	}

	virtual void on_show_context_menu(CL_PopupMenu &menu, GridObject *grid_object)
	{ 
		CL_PopupMenuItem item = menu.insert_item("Edit columns...");
		item.func_clicked().set(this, &ListViewComponentType::on_edit_columns_clicked, grid_object);
	}

	void on_edit_columns_clicked(GridObject *grid_object)
	{
		CL_ListView *list_view = dynamic_cast<CL_ListView*>(grid_object->get_component());
		if(list_view == 0)
			throw CL_Exception("Can't get listview object");

		GridComponent *grid_component = grid_object->get_grid_component();
		GuiEditorWindow *main_window = grid_component->get_main_window();
		EditColumnsWindow edit_columns_window(main_window);

		edit_columns_window.set_columns(get_columns(list_view));
		if(edit_columns_window.exec() == 1)
		{
			remove_columns(list_view);
			std::vector<EditColumnsWindow::Column> columns = edit_columns_window.get_columns();
			create_columns(list_view, columns);
		}
	}

	std::vector<EditColumnsWindow::Column> get_columns(CL_ListView *list_view) 
	{
		std::vector<EditColumnsWindow::Column> columns;

		CL_ListViewHeader *header = list_view->get_header();
		CL_ListViewColumnHeader column = header->get_first_column();
		while (!column.is_null())
		{
			int width = column.get_width();
			CL_String caption = column.get_caption();
			CL_String id = column.get_column_id();

			columns.push_back(EditColumnsWindow::Column(caption, id, width));

			column = column.get_next_sibling();
		}

		return columns;
	}

	void remove_columns(CL_ListView *list_view) 
	{
		CL_ListViewHeader *header = list_view->get_header();

		CL_ListViewColumnHeader column = header->get_first_column();
		while(!column.is_null())
		{
			header->remove(column.get_column_id());
			column = header->get_first_column();
		}
	}

	void create_columns(CL_ListView *list_view, std::vector<EditColumnsWindow::Column> &columns) 
	{
		CL_ListViewHeader *header = list_view->get_header();

		for(size_t i = 0; i < columns.size(); ++i)
		{
			CL_ListViewColumnHeader column = header->create_column(columns[i].id, columns[i].text);
			column.set_width(columns[i].width);
			header->append(column);
		}
	}
};
