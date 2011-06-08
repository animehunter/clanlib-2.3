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
#include "theme.h"

Theme::Theme(CL_GUIManager &manager, gui_theme default_theme) : current_theme(default_theme),
	CL_Window(&manager, CL_GUITopLevelDescription("Window Theme", CL_Rect(256*3 + 32, 256 + 180 + 24, CL_Size(256, 180)), false))
{
	set_draggable(true);

	CL_Rect client_area = get_client_area();

	groupbox = new CL_GroupBox(this);
	groupbox->set_geometry(CL_Rect(client_area.left + 5, client_area.top + 5, client_area.right - 5, client_area.bottom - 5));

	int xoffset = client_area.left + 150;
	int yoffset = client_area.top + 4;
	int ygap = 20;

	radiobutton_aero = new CL_RadioButton(this);
	radiobutton_aero->set_geometry(CL_Rect(xoffset, yoffset , CL_Size(64, 24)));
	radiobutton_aero->set_text("Aero");
	radiobutton_aero->set_selected(default_theme == theme_aero);
	radiobutton_aero->set_group_name("Theme");
	radiobutton_aero->func_selected().set(this, &Theme::on_theme_selected, radiobutton_aero);
	yoffset += ygap;

	radiobutton_aero_packed = new CL_RadioButton(this);
	radiobutton_aero_packed->set_geometry(CL_Rect(xoffset, yoffset , CL_Size(64, 24)));
	radiobutton_aero_packed->set_text("Aero Packed");
	radiobutton_aero_packed->set_selected(default_theme == theme_aero_packed);
	radiobutton_aero_packed->set_group_name("Theme");
	radiobutton_aero_packed->func_selected().set(this, &Theme::on_theme_selected, radiobutton_aero_packed);
	yoffset += ygap;

	radiobutton_luna = new CL_RadioButton(this);
	radiobutton_luna->set_geometry(CL_Rect(xoffset, yoffset , CL_Size(64, 24)));
	radiobutton_luna->set_text("Luna");
	radiobutton_luna->set_selected(default_theme == theme_luna);
	radiobutton_luna->set_group_name("Theme");
	radiobutton_luna->func_selected().set(this, &Theme::on_theme_selected, radiobutton_luna);
	yoffset += ygap;

	radiobutton_luna_packed = new CL_RadioButton(this);
	radiobutton_luna_packed->set_geometry(CL_Rect(xoffset, yoffset , CL_Size(64, 24)));
	radiobutton_luna_packed->set_text("Luna Packed");
	radiobutton_luna_packed->set_selected(default_theme == theme_luna_packed);
	radiobutton_luna_packed->set_group_name("Theme");
	radiobutton_luna_packed->func_selected().set(this, &Theme::on_theme_selected, radiobutton_luna_packed);
	yoffset += ygap;

	radiobutton_basic = new CL_RadioButton(this);
	radiobutton_basic->set_geometry(CL_Rect(xoffset, yoffset , CL_Size(64, 24)));
	radiobutton_basic->set_text("Basic");
	radiobutton_basic->set_selected(default_theme == theme_basic);
	radiobutton_basic->set_group_name("Theme");
	radiobutton_basic->func_selected().set(this, &Theme::on_theme_selected, radiobutton_basic);
	yoffset += ygap;

	radiobutton_basic_packed = new CL_RadioButton(this);
	radiobutton_basic_packed->set_geometry(CL_Rect(xoffset, yoffset , CL_Size(64, 24)));
	radiobutton_basic_packed->set_text("Basic Packed");
	radiobutton_basic_packed->set_selected(default_theme == theme_basic_packed);
	radiobutton_basic_packed->set_group_name("Theme");
	radiobutton_basic_packed->func_selected().set(this, &Theme::on_theme_selected, radiobutton_basic_packed);
	yoffset += ygap;

	if (!CL_FileHelp::file_exists("../../../Resources/GUIThemeAero/theme.css"))
	{
		radiobutton_aero->set_enabled(false);
		radiobutton_aero_packed->set_enabled(false);
		radiobutton_luna->set_enabled(false);
		radiobutton_luna_packed->set_enabled(false);
		radiobutton_basic->set_selected(true);

	}


}

void Theme::run(CL_GraphicContext &gc)
{
}

void Theme::on_theme_selected(CL_RadioButton *radiobutton)
{
	if (radiobutton == radiobutton_aero)
		current_theme = theme_aero;

	if (radiobutton == radiobutton_aero_packed)
		current_theme = theme_aero_packed;

	if (radiobutton == radiobutton_luna)
		current_theme = theme_luna;

	if (radiobutton == radiobutton_luna_packed)
		current_theme = theme_luna_packed;

	if (radiobutton == radiobutton_basic)
		current_theme = theme_basic;

	if (radiobutton == radiobutton_basic_packed)
		current_theme = theme_basic_packed;
}
