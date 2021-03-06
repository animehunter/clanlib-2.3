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
**    Magnus Norddahl
*/

#include "precomp.h"
#include "property_component_edit_state.h"

PropertyComponentEditState::PropertyComponentEditState()
: state(state_none)
{
	edit_state_handlers[state_none] = &edit_state_none;
	edit_state_handlers[state_header_size] = &edit_state_header_size;
	edit_state_handlers[state_node_opener] = &edit_state_node_opener;
	edit_state_handlers[state_revert_button] = &edit_state_revert_button;
}

void PropertyComponentEditState::set_component(PropertyComponent *c)
{
	for (int i = 0; i < num_edit_states; i++)
		edit_state_handlers[i]->property_component = c;
}

void PropertyComponentEditState::set_state(State new_state)
{
	state = new_state;
}

bool PropertyComponentEditState::on_input_pressed(const CL_InputEvent &input_event)
{
	if (state == state_none)
	{
		for (int i = num_edit_states; i > 0; i--)
		{
			if (edit_state_handlers[i-1]->on_input_pressed(input_event))
			{
				state = (State)(i-1);
				return true;
			}
		}
		return false;
	}
	else
	{
		return edit_state_handlers[state]->on_input_pressed(input_event);
	}
}

bool PropertyComponentEditState::on_input_released(const CL_InputEvent &input_event)
{
	return edit_state_handlers[state]->on_input_released(input_event);
}

bool PropertyComponentEditState::on_input_doubleclick(const CL_InputEvent &input_event)
{
	return edit_state_handlers[state]->on_input_doubleclick(input_event);
}

bool PropertyComponentEditState::on_input_pointer_moved(const CL_InputEvent &input_event)
{
	return edit_state_handlers[state]->on_input_pointer_moved(input_event);
}
