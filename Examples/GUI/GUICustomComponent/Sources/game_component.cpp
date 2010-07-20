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

#include "precomp.h"
#include "game_component.h"
#include "radial_menu.h"

GameComponent::GameComponent(const CL_Rect &position, CL_GUIManager* gui_manager)
: CL_GUIComponent(gui_manager, CL_GUITopLevelDescription(position, false)),
  radial_menu(0)
{
	set_constant_repaint(true);

	func_render().set(this, &GameComponent::on_render);
	func_process_message().set(this, &GameComponent::on_message);

	CL_GraphicContext gc = get_gc();
	background = CL_Image(gc, "Resources/Images/background.jpg");

	radial_menu = new RadialMenu(this);
	radial_menu->func_selected.set(this, &GameComponent::on_radial_menu_itemselected);

	font = CL_Font(get_gc(), "Tahoma", -30);
}

GameComponent::~GameComponent()
{
}

void GameComponent::on_render(CL_GraphicContext &gc, const CL_Rect &clip_rect)
{
	background.draw(gc, 0, 0);
	font.draw_text(gc, 0, 24, selected_text);
}

void GameComponent::on_message(CL_GUIMessage &message)
{
	if (message.is_type(CL_GUIMessage_Input::get_type_name()))
	{
		CL_GUIMessage_Input msg_input = message;
		CL_InputEvent e = msg_input.get_event();
		if (e.type == CL_InputEvent::pressed)
		{
			if(e.id == CL_KEY_ESCAPE)
			{
				get_gui_manager().exit_with_code(0);
			}

			if(e.id == CL_MOUSE_LEFT)
			{
				show_radial_menu(e.mouse_pos);
			}
		}
	}
}

void GameComponent::show_radial_menu(CL_Point &position)
{
	radial_menu->clear_items();
	radial_menu->add_item("Examine");
	radial_menu->add_item("Open");
	radial_menu->add_item("Close");
	radial_menu->add_item("Push");
	radial_menu->add_item("Pick up"); 
	radial_menu->add_item("Kick");
	//	radialMenu->add_item("Item 6");
	//	radialMenu->add_item("Item 7");
	//	radialMenu->add_item("Item 8");

	radial_menu->show(position);
}

void GameComponent::on_radial_menu_itemselected(int selected_index)
{
	selected_text = radial_menu->get_item(selected_index);
}
