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

#pragma once

class EditColumnsWindow : public CL_Window
{
public:
	EditColumnsWindow(CL_GUIComponent *owner);
	~EditColumnsWindow();

// Attributes:
public:

// Operations:
public:

// Implementation:
private:
	CL_GUITopLevelDescription get_startup_description();

	void load_components();

	void on_button_ok_clicked();
	void on_button_cancel_clicked();
	void on_button_move_column_up_clicked();
	void on_button_move_column_down_clicked();
	void on_button_add_column_clicked();
	void on_button_remove_column_clicked();
	bool on_close();

	CL_PushButton *button_ok;
	CL_PushButton *button_cancel;
	CL_PushButton *button_move_column_up;
	CL_PushButton *button_move_column_down;
	CL_PushButton *button_add_column;
	CL_PushButton *button_remove_column;
	CL_ListView *listview_columns;
	CL_LineEdit *lineedit_text;
	CL_LineEdit *lineedit_id;
	CL_LineEdit *lineedit_width;
};
