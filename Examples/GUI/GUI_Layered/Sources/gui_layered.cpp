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
**    Mark Page
*/

#include "precomp.h"
#include "gui_layered.h"
#include "GUI.h"
#include "app.h"

GUI_Layered::GUI_Layered(GUI *gui) : gui(gui), window_ptr(gui->get_app()->get_window()), wm(*window_ptr)
{
	CL_GUIManager *gui_manager = &gui->get_gui_manager();

	gui_manager->set_window_manager(wm);

	wm.func_input_intercept().set(this, &GUI_Layered::wm_input_intercept);

	// Use a texture group to store all the gui textures
	//CL_GraphicContext gc = window_ptr->get_gc();
	//CL_TextureGroup texture_group(CL_Size(1024, 1024));
	//wm.set_texture_group(texture_group);	// Note: This line is optional

	resources_gui = CL_ResourceManager(gui->get_resources_location());

	theme.set_resources(resources_gui);
	gui_manager->set_theme(theme); 

	gui_manager->set_css_document(gui->get_theme_location());


	setup_shader();

	wobble_offset = 0.0f;
	last_time = CL_System::get_time();
	lens_zoom = 3.2f;
	lens_near = 0.1f;
	lens_far = 10000.0f;
	lens_aspect = 1.0f;

	// Note, CL_GUIManager deletes these automatically, after GUI_Direct has gone out of scope in the CL_GUIManager destructor
	pushbutton = new PushButton(gui->get_gui_manager(), gui->get_resources_internal());
	lineedit = new LineEdit(gui->get_gui_manager());
	checkbox = new CheckBox(gui->get_gui_manager());
	slider = new Slider(gui->get_gui_manager());
	radiobutton = new RadioButton(gui->get_gui_manager());
	scrollbar = new ScrollBar(gui->get_gui_manager());
	progressbar = new ProgressBar(gui->get_gui_manager());
	tabpage = new TabPage(gui->get_gui_manager());
	menubar = new MenuBar(gui->get_gui_manager(), gui->get_resources_internal());
	spin = new Spin(gui->get_gui_manager());
	combobox = new ComboBox(gui->get_gui_manager());
	listview = new ListView(gui->get_gui_manager());

	panel3d = new Panel3D(gui);

	pushbutton->func_close().set(this, &GUI_Layered::on_close,  dynamic_cast<CL_Window *>(pushbutton));
	lineedit->func_close().set(this, &GUI_Layered::on_close,  dynamic_cast<CL_Window *>(lineedit));
	checkbox->func_close().set(this, &GUI_Layered::on_close,  dynamic_cast<CL_Window *>(checkbox));
	slider->func_close().set(this, &GUI_Layered::on_close,  dynamic_cast<CL_Window *>(slider));
	radiobutton->func_close().set(this, &GUI_Layered::on_close,  dynamic_cast<CL_Window *>(radiobutton));
	scrollbar->func_close().set(this, &GUI_Layered::on_close,  dynamic_cast<CL_Window *>(scrollbar));
	progressbar->func_close().set(this, &GUI_Layered::on_close,  dynamic_cast<CL_Window *>(progressbar));
	tabpage->func_close().set(this, &GUI_Layered::on_close,  dynamic_cast<CL_Window *>(tabpage));
	menubar->func_close().set(this, &GUI_Layered::on_close,  dynamic_cast<CL_Window *>(menubar));
	spin->func_close().set(this, &GUI_Layered::on_close,  dynamic_cast<CL_Window *>(spin));
	combobox->func_close().set(this, &GUI_Layered::on_close,  dynamic_cast<CL_Window *>(combobox));
	listview->func_close().set(this, &GUI_Layered::on_close,  dynamic_cast<CL_Window *>(listview));
	panel3d->func_close().set(this, &GUI_Layered::on_close,  dynamic_cast<CL_Window *>(panel3d));
}

GUI_Layered::~GUI_Layered()
{
}

bool GUI_Layered::run()
{
	wm.process();
	run3d();
	return true;
}

CL_ResourceManager &GUI_Layered::get_resources_internal()
{
	return gui->get_resources_internal();
}

CL_ResourceManager &GUI_Layered::get_resources_gui()
{
	return resources_gui;
}

CL_GUIManager &GUI_Layered::get_gui_manager()
{
	return gui->get_gui_manager();
}

bool GUI_Layered::on_close(CL_Window *win)
{
	gui->get_gui_manager().exit_with_code(0);
	return true;
}

