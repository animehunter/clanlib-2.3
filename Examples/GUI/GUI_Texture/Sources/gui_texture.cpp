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
#include "gui_texture.h"
#include "GUI.h"
#include "app.h"

GUI_Texture::GUI_Texture(GUI *gui) : gui(gui), window_ptr(gui->get_app()->get_window()), wm(*window_ptr)
{
	CL_GUIManager *gui_manager = &gui->get_gui_manager();

	gui_manager->set_window_manager(wm);

	// Use a texture group to store all the gui textures
	CL_GraphicContext gc = window_ptr->get_gc();
	CL_TextureGroup texture_group(CL_Size(1024, 1024));
	wm.set_texture_group(texture_group);	// Note: This line is optional

	resources_gui = CL_ResourceManager(gui->get_resources_location());

	theme.set_resources(resources_gui);
	gui_manager->set_theme(theme); 

	gui_manager->set_css_document(gui->get_theme_location());

	// Since this example rebuilds the gui when the theme changes, we have to manually delete the components
	// (We could have instead recreated the CL_GUIManager, so it's destructor deletes them)

	pushbutton.reset(new PushButton(gui->get_gui_manager(), gui->get_resources_internal()));
	lineedit.reset(new LineEdit(gui->get_gui_manager()));
	checkbox.reset(new CheckBox(gui->get_gui_manager()));
	slider.reset(new Slider(gui->get_gui_manager()));
	radiobutton.reset(new RadioButton(gui->get_gui_manager()));
	scrollbar.reset(new ScrollBar(gui->get_gui_manager()));
	progressbar.reset(new ProgressBar(gui->get_gui_manager()));
	tabpage.reset(new TabPage(gui->get_gui_manager()));
	menubar.reset(new MenuBar(gui->get_gui_manager(), gui->get_resources_internal()));
	spin.reset(new Spin(gui->get_gui_manager()));
	combobox.reset(new ComboBox(gui->get_gui_manager()));
	listview.reset(new ListView(gui->get_gui_manager()));
	window_theme.reset(new Theme(gui->get_gui_manager(), gui->get_theme()));
}

GUI_Texture::~GUI_Texture()
{
}

bool GUI_Texture::run(CL_GraphicContext &gc)
{
	wm.process();
	wm.draw_windows(gc);

	return true;
}

