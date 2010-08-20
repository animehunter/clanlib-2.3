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
**    Harry Storbacka
**    Kenneth Gangstoe
*/

#include "precomp.h"
#include "component_types.h"
#include "component_type.h"
#include "component_type_template.h"
#include "custom_component.h"
#include "button_component_type.h"
#include "checkbox_component_type.h"
#include "frame_component_type.h"
#include "label_component_type.h"
#include "listview_component_type.h"
#include "radio_button_component_type.h"
#include "scrollbar_horz_component_type.h"
#include "scrollbar_vert_component_type.h"
#include "slider_component_type.h"
#include "tab_component_type.h"
#include "imageview_component_type.h"

void ComponentTypes::initialize()
{
	int next_id = 1;
	types.push_back(new CheckBoxComponentType("checkbox", "CheckBox", next_id++, "gfx/check.png"));
	types.push_back(new RadioButtonComponentType("radiobutton", "RadioButton", next_id++, "gfx/radio.png"));
	types.push_back(new LabelComponentType("label", "Label", next_id++, "gfx/label.png"));
	types.push_back(new ComponentTypeTemplate<CL_LineEdit>("lineedit", "LineEdit", next_id++, "gfx/lineedit.png"));
	types.push_back(new ComponentTypeTemplate<CL_ComboBox>("combobox", "ComboBox", next_id++, "gfx/combobox.png"));
	types.push_back(new ListViewComponentType("listview", "ListView", next_id++, "gfx/listview.png"));
	types.push_back(new ComponentTypeTemplate<CL_MenuBar>("menubar", "MenuBar", next_id++, "gfx/menu.png"));
	types.push_back(new ButtonComponentType("button", "PushButton", next_id++, "gfx/pushbutton.png"));
	types.push_back(new SliderComponentType("slider", "Slider", next_id++, "gfx/slider.png"));
	types.push_back(new ComponentTypeTemplate<CL_Spin>("spin", "Spin", next_id++, "gfx/spin.png"));
	types.push_back(new ImageViewComponentType("imageview", "ImageView", next_id++, "gfx/image.png"));
	types.push_back(new ScrollBarHorizontalComponentType("hscrollbar", "H Scroll Bar", next_id++, "gfx/scroll_horizontal.png"));
	types.push_back(new ScrollBarVerticalComponentType("vscrollbar", "V Scroll Bar", next_id++, "gfx/scroll_vertical.png"));
	types.push_back(new TabComponentType("tab", "Tab", next_id++, "gfx/tab.png"));
	types.push_back(new FrameComponentType("frame", "Frame", next_id++, "gfx/frame.png"));
	types.push_back(new ComponentTypeTemplate<CL_StatusBar>("statusbar", "StatusBar", next_id++, "gfx/statusbar.png"));
	types.push_back(new ComponentTypeTemplate<CL_ToolBar>("toolbar", "ToolBar", next_id++, "gfx/toolbar.png"));
	types.push_back(new ComponentTypeTemplate<CustomComponent>("customcomponent", "Custom", next_id++, "gfx/custom.png"));
}

void ComponentTypes::deinitialize()
{
	for (std::vector<ComponentType *>::size_type index = 0; index < types.size(); index++)
		delete types[index];
	types.clear();
}

ComponentType *ComponentTypes::find_component(int id)
{
	std::vector< CL_SharedPtr<ComponentType> >::size_type index, size;
	size = types.size();
	for (index = 0; index < size; index++)
	{
		if (types[index]->id == id)
		{
			return types[index];
		}
	}
	throw CL_Exception(cl_format("Unable to find component type, unknown id: %1", id));
}

ComponentType *ComponentTypes::find_from_xml(const CL_String &tag)
{
	std::vector< CL_SharedPtr<ComponentType> >::size_type index, size;
	size = types.size();
	for (index = 0; index < size; index++)
	{
		if (types[index]->xmlname == tag)
		{
			return types[index];
		}
	}
	return 0;
}

void ComponentTypes::set_id_name(CL_GUIComponent *new_component, int id)
{
	if (component_count.find(id) == component_count.end())
		component_count[id] = 1;

	new_component->set_id_name(cl_format("%1%2", new_component->get_type_name(), component_count[id]++));
}

const std::vector<ComponentType *> &ComponentTypes::get_types()
{
	return types;
}

std::vector<ComponentType *> ComponentTypes::types;
std::map<int, int> ComponentTypes::component_count;
