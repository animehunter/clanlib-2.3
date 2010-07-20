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
*/

#pragma once

#include <ClanLib/gui.h>
#include <ClanLib/display.h>

class RadialMenu;

class GameComponent : public CL_GUIComponent
{
public:
	GameComponent(const CL_Rect &position, CL_GUIManager *gui_manager);
	~GameComponent();

private:
	void on_render(CL_GraphicContext &gc, const CL_Rect &cliprect);
	void on_message(CL_GUIMessage &message);

	void show_radial_menu(CL_Point &position);
	void on_radial_menu_itemselected(int selectedIndex);

	RadialMenu *radial_menu;

	CL_Image background;

	CL_String selected_text;

	CL_Font font;
};
