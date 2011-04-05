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

#include "../view.h"
#include "../css_packer.h"

class CssPackerView : public View
{
public:
	CssPackerView(CL_GUIComponent *parent, MainWindow *mainwindow);
	~CssPackerView();

private:
	void on_resized();
	void on_button_browse();
	void on_button_save();

	int load_css_file(const CL_String &file);
	void save_css_file(const CL_String &file);

	CL_Label *label_selected_file;
	CL_LineEdit *lineedit_selected_file;
	CL_PushButton *button_browse_file;

	CL_Label *label_result;

	CL_PushButton *button_save;

	CssPacker packer;
};
