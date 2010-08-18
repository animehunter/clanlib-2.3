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
**    Kenneth Gangstoe
*/

#include "precomp.h"
#include "edit_columns_window.h"

EditColumnsWindow::EditColumnsWindow(CL_GUIComponent *owner)
: CL_Window(owner, get_startup_description())
{
	set_id_name("edit_columns_window");

	func_close().set(this, &EditColumnsWindow::on_close);

	load_components();
}

EditColumnsWindow::~EditColumnsWindow()
{
}

CL_GUITopLevelDescription EditColumnsWindow::get_startup_description()
{
	CL_GUITopLevelDescription desc;
	desc.set_title("Edit columns");
	desc.set_allow_resize(false);
	return desc;
}

void EditColumnsWindow::load_components()
{
	set_layout(CL_GUILayoutCorners());
	create_components("Dialogs/edit_columns.gui");

	button_ok = CL_PushButton::get_named_item(this, "button_ok");
	button_ok->func_clicked().set(this, &EditColumnsWindow::on_button_ok_clicked);

	button_cancel = CL_PushButton::get_named_item(this, "button_cancel");
	button_cancel->func_clicked().set(this, &EditColumnsWindow::on_button_cancel_clicked);

	button_move_column_up = CL_PushButton::get_named_item(this, "button_move_column_up");
	button_move_column_up->func_clicked().set(this, &EditColumnsWindow::on_button_move_column_up_clicked);

	button_move_column_down = CL_PushButton::get_named_item(this, "button_move_column_down");
	button_move_column_down->func_clicked().set(this, &EditColumnsWindow::on_button_move_column_down_clicked);

	button_add_column = CL_PushButton::get_named_item(this, "button_add_column");
	button_add_column->func_clicked().set(this, &EditColumnsWindow::on_button_add_column_clicked);

	button_remove_column = CL_PushButton::get_named_item(this, "button_remove_column");
	button_remove_column->func_clicked().set(this, &EditColumnsWindow::on_button_remove_column_clicked);

	listview_columns = CL_ListView::get_named_item(this, "listview_columns");

	lineedit_text = CL_LineEdit::get_named_item(this, "lineedit_text");

	lineedit_id = CL_LineEdit::get_named_item(this, "lineedit_id");

	lineedit_width = CL_LineEdit::get_named_item(this, "lineedit_width");
}

void EditColumnsWindow::on_button_ok_clicked()
{
	exit_with_code(0);
}

void EditColumnsWindow::on_button_cancel_clicked()
{
	exit_with_code(0);
}

void EditColumnsWindow::on_button_move_column_up_clicked()
{
}

void EditColumnsWindow::on_button_move_column_down_clicked()
{
}

void EditColumnsWindow::on_button_add_column_clicked()
{
}

void EditColumnsWindow::on_button_remove_column_clicked()
{
}

bool EditColumnsWindow::on_close()
{
	exit_with_code(0);
	return true;
}
