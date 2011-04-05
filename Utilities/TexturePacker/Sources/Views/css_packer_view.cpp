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

#include "precomp.h"
#include "css_packer_view.h"

CssPackerView::CssPackerView(CL_GUIComponent *parent, MainWindow *mainwindow)
: View(parent, mainwindow, "CSS Packer")
{
	func_resized().set(this, &CssPackerView::on_resized);

	label_selected_file = new CL_Label(this);
	label_selected_file->set_text("Input CSS file:");

	lineedit_selected_file = new CL_LineEdit(this);

	button_browse_file = new CL_PushButton(this);
	button_browse_file->set_text("...");
	button_browse_file->func_clicked().set(this, &CssPackerView::on_button_browse);

	button_save = new CL_PushButton(this);
	button_save->set_text("Save CSS file");
	button_save->func_clicked().set(this, &CssPackerView::on_button_save);

	label_result = new CL_Label(this);

	on_resized();
}

CssPackerView::~CssPackerView()
{
}

void CssPackerView::on_resized()
{
	CL_Size size = get_size();

	label_selected_file->set_geometry(CL_Rect(10, 13, 80, 30));
	lineedit_selected_file->set_geometry(CL_Rect(86, 10, size.width - 20 - 20, 30));
	button_browse_file->set_geometry(CL_Rect(10 + size.width - 20 - 30, 10, 10 + size.width - 20, 30));

	button_save->set_geometry(CL_Rect(size.width - 130, 70, CL_Size(120, 25)));

	label_result->set_geometry(CL_Rect(10, 100, size.width - 20, 130));
}

void CssPackerView::on_button_browse()
{
	CL_OpenFileDialog dlg(this);
	dlg.add_filter("CSS files", "*.css", true);
	dlg.add_filter("All files", "*.*");

	dlg.set_initial_directory(CL_System::get_exe_path());

	if(dlg.show())
	{
		CL_String filename = dlg.get_filename();

		try
		{
			int imports = load_css_file(filename);

			label_result->set_text(cl_format("Loaded CSS file with %1 imports", imports));

			lineedit_selected_file->set_text(filename);
		}
		catch(CL_Exception error)
		{
			label_result->set_text(cl_format("Error: %1", error.message));
		}
	}
}

void CssPackerView::on_button_save()
{
	CL_SaveFileDialog dlg(this);

	dlg.add_filter("CSS files", "*.css", true);
	dlg.add_filter("All files", "*.*");

	dlg.set_initial_directory(CL_System::get_exe_path());

	if(dlg.show())
	{
		CL_String filename = dlg.get_filename();

		try
		{
			save_css_file(filename);

			label_result->set_text(cl_format("Saved resources: %1", filename));
		}
		catch(CL_Exception error)
		{
			label_result->set_text(cl_format("Save error: %1", error.message));
		}
	}
}

int CssPackerView::load_css_file(const CL_String &file)
{
	return packer.load(file);
}

void CssPackerView::save_css_file(const CL_String &file)
{
	packer.save(file);
}
