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
**    Magnus Norddahl
*/

#include "precomp.h"
#include "property_component.h"
#include "property_item.h"
#include "holder_component.h"
#include "grid_component.h"
#include "custom_component.h"
#include "selection.h"
#include "main_window.h"
#include "property_item_position.h"
#include "property_item_text.h"
#include "property_item_group_name.h"
#include "property_item_class_name.h"
#include "property_item_id_name.h"
#include "property_item_min.h"
#include "property_item_max.h"
#include "property_item_tick_count.h"
#include "property_item_step_size.h"
#include "property_item_page_label.h"
#include "property_item_type_name.h"

PropertyComponent::PropertyComponent(MainWindow *main_window)
: CL_GUIComponent(main_window), main_window(main_window), name_column_width(75), active_item(0), active_component(0), scrollbar(0)
{
	set_type_name("property-component");
	set_clip_children(true);
	part_background = CL_GUIThemePart(this);
	part_header = CL_GUIThemePart(this, "header");
	part_header_name = CL_GUIThemePart(this, "header name");
	part_header_value = CL_GUIThemePart(this, "header value");
	part_row[0] = CL_GUIThemePart(this, "row");
	part_row[1] = CL_GUIThemePart(this, "row");
	part_row[1].set_state("alternate", true);
	part_item_name = CL_GUIThemePart(this, "item name");
	part_item_value = CL_GUIThemePart(this, "item value");

	func_input_pressed().set(this, &PropertyComponent::on_input_pressed);
	func_input_released().set(this, &PropertyComponent::on_input_released);
	func_input_doubleclick().set(this, &PropertyComponent::on_input_doubleclick);
	func_input_pointer_moved().set(this, &PropertyComponent::on_input_pointer_moved);
	func_render().set(this, &PropertyComponent::on_render);

	scrollbar = new CL_ScrollBar(this);
	scrollbar->set_vertical();
	scrollbar->set_visible(false);

	edit_state.set_component(this);

	slots.connect(main_window->get_selection()->sig_selection_changed(), this, &PropertyComponent::on_selection_changed);
	slots.connect(get_gui_manager().sig_filter_message(), this, &PropertyComponent::on_filter_message);
}

PropertyComponent::~PropertyComponent()
{
	clear();
}

void PropertyComponent::clear()
{
	deactivate();
	for (size_t i = 0; i < items.size(); i++)
		delete items[i];
	items.clear();
	request_repaint();
}

void PropertyComponent::add_property(PropertyItem *item)
{
	item->property_component = this;
	items.push_back(item);
	request_repaint();
}

void PropertyComponent::on_selection_changed()
{
	clear();
	std::vector<HolderComponent *> selection = main_window->get_selection()->get_selection();
	if (!selection.empty())
	{
		HolderComponent *item = selection[0];
		CL_GUIComponent *comp = item->get_component();

		// add_property(new PropertyItemHeader("Properties"));

		CL_String type = comp->get_type_name();

		if (type != "grid")
		{
			CL_Size comp_size = comp->get_geometry().get_size();
			bool enable_width = true;
			bool enable_height = true;

			HolderComponent *holder = dynamic_cast<HolderComponent*>(comp->get_parent_component());
			if (holder)
			{
				add_property(new PropertyItemPosition(PropertyItemPosition::type_x1));
				add_property(new PropertyItemPosition(PropertyItemPosition::type_y1));
				add_property(new PropertyItemPosition(PropertyItemPosition::type_x2));
				add_property(new PropertyItemPosition(PropertyItemPosition::type_y2));
/*
				CL_String equ_x = holder->get_position_equation_x();
				CL_String equ_y = holder->get_position_equation_y();

				if (equ_x.empty())
					add_property(new PropertyItemPosition("x1", CL_StringHelp::int_to_text(holder->get_geometry().left)));
				else
					add_property(new PropertyItemPosition("x1", equ_x));

				if (equ_y.empty())
					add_property(new PropertyItemPosition("y1", CL_StringHelp::int_to_text(holder->get_geometry().top)));
				else
					add_property(new PropertyItemPosition("y1", equ_y));


				CL_String equ_x2 = holder->get_position_equation_x2();
				CL_String equ_y2 = holder->get_position_equation_y2();

				add_property(new PropertyItemLineEdit("x2", equ_x2));
				add_property(new PropertyItemLineEdit("y2", equ_y2));

				if (!equ_x2.empty())
					enable_width = false;
				if (!equ_y2.empty())
					enable_height = false;
*/
			}

			add_property(new PropertyItemPosition(PropertyItemPosition::type_width));
			add_property(new PropertyItemPosition(PropertyItemPosition::type_height));
			//add_property(new PropertyItemLineEdit("Width", CL_StringHelp::int_to_text(comp_size.width)));
			//add_property(new PropertyItemLineEdit("Height", CL_StringHelp::int_to_text(comp_size.height)));
			// le_w->set_enabled(enable_width);
			// le_h->set_enabled(enable_height);
		}

		add_property(new PropertyItemText());

		if (type == "button")
		{
			CL_PushButton *co = dynamic_cast<CL_PushButton*>(comp);
			//add_property(new PropertyItemLineEdit("Text", co->get_text()));
		}
		else if (type == "lineedit")
		{
			CL_LineEdit *co = dynamic_cast<CL_LineEdit*>(comp);
			//add_property(new PropertyItemLineEdit("Text",co->get_text()));
		}
		else if (type == "checkbox")
		{
			CL_CheckBox *co = dynamic_cast<CL_CheckBox*>(comp);
			//add_property(new PropertyItemLineEdit("Text", co->get_text()));
		}
		else if (type == "radiobutton")
		{
			CL_RadioButton *co = dynamic_cast<CL_RadioButton*>(comp);
			//add_property(new PropertyItemLineEdit("Text", co->get_text()));
			//add_property(new PropertyItemLineEdit("Group Name", co->get_group_name()));
			add_property(new PropertyItemGroupName());
		}
		else if (type == "label")
		{
			CL_Label *co = dynamic_cast<CL_Label*>(comp);
			//add_property(new PropertyItemLineEdit("Text", co->get_text()));
		}
		else if (type == "statusbar")
		{
			CL_StatusBar *co = dynamic_cast<CL_StatusBar*>(comp);
		}
		else if (type == "frame")
		{
			CL_Frame *co = dynamic_cast<CL_Frame*>(comp);
			//add_property(new PropertyItemLineEdit("Text", co->get_header_text()));
		}
		else if (type == "slider")
		{
			CL_Slider *co = dynamic_cast<CL_Slider*>(comp);
			add_property(new PropertyItemMin());
			add_property(new PropertyItemMax());
			add_property(new PropertyItemTickCount());
			add_property(new PropertyItemStepSize());
			//add_property(new PropertyItemLineEdit("Min", CL_StringHelp::int_to_text(co->get_min())));
			//add_property(new PropertyItemLineEdit("Max", CL_StringHelp::int_to_text(co->get_max())));
			//add_property(new PropertyItemLineEdit("Tick Count", CL_StringHelp::int_to_text(co->get_tick_count())));
			//add_property(new PropertyItemLineEdit("Step Size", CL_StringHelp::int_to_text(co->get_page_step())));
		}
		else if (type == "grid")
		{
			GridComponent *co = dynamic_cast<GridComponent*>(comp);
			CL_Size s = co->get_dialog_size();
			//add_property(new PropertyItemLineEdit("Width", CL_StringHelp::int_to_text(s.width)));
			//add_property(new PropertyItemLineEdit("Height", CL_StringHelp::int_to_text(s.height)));
		}
		else if (type == "menubar")
		{
		}
		else if (type == "spin")
		{
		}
		else if (type == "statusbar")
		{
		}
		else if (type == "combobox")
		{
		}
		else if (type == "listview")
		{
		}
		else if (type == "tab")
		{
			CL_Tab *tab = dynamic_cast<CL_Tab*>(comp);
			int current_page = tab->get_current_page_index();
			CL_TabPage *tab_page = tab->get_page(current_page);
			add_property(new PropertyItemPageLabel());
			//add_property(new PropertyItemLineEdit("Page label", tab_page->get_label()));
		}
		else
		{
			add_property(new PropertyItemTypeName());
			//add_property(new PropertyItemLineEdit("Type", comp->get_type_name()));
		}

		add_property(new PropertyItemIdName());
		add_property(new PropertyItemClassName());
		//add_property(new PropertyItemLineEdit("#id", comp->get_id_name()));
		//add_property(new PropertyItemLineEdit(".class", comp->get_class_name()));

		add_property(new PropertyItemAnchor());
/*
		// Layout
		if (type != "grid")
		{
			add_anchor_radios();
		}

		// Other non-lineedit controls go here...
		if (type == "tab")
		{
			CL_Rect child_rect = get_child_rect(container);

			CL_PushButton *add_tab = new CL_PushButton(container);
			add_tab->set_geometry(CL_RectPS(child_rect.left, child_rect.bottom+10, 80,24));
			add_tab->set_text("Add tab");

			CL_PushButton *del_tab = new CL_PushButton(container);
			del_tab->set_geometry(CL_RectPS(child_rect.left, child_rect.bottom+10+32, 80,24));
			del_tab->set_text("Delete tab");

			CL_Tab *tab = dynamic_cast<CL_Tab*>(comp);
			add_tab->func_clicked().set(this, &PropertyComponent::on_add_tab, tab);
			del_tab->func_clicked().set(this, &PropertyComponent::on_delete_tab, tab);
		}

		// Callback functions:
		add_property(new PropertyItemHeader(type + " Callbacks:"));
		std::vector<CallbackData> callbacks = callback_info.get_callbacks(type);
		std::vector<CallbackData>::iterator it_cb;
		for (it_cb = callbacks.begin(); it_cb != callbacks.end(); ++it_cb)
		{
			CallbackData &cb = (*it_cb);
			add_property(new PropertyItemLineEdit(cb.get_callback_name(), cb.get_handler_function_name()));
		}

		add_header("Component Callbacks:");
		callbacks = callback_info.get_callbacks("gui_component");
		for (it_cb = callbacks.begin(); it_cb != callbacks.end(); ++it_cb)
		{
			CallbackData &cb = (*it_cb);
			add_property(new PropertyItemLineEdit(cb.get_callback_name(), cb.get_handler_function_name()));
		}
*/

		for (size_t i = 0; i < items.size(); i++)
			items[i]->selection_changed(selection);
		request_repaint();
	}
}

bool PropertyComponent::on_input_pressed(const CL_InputEvent &input_event)
{
	return edit_state.on_input_pressed(input_event);
}

bool PropertyComponent::on_input_released(const CL_InputEvent &input_event)
{
	return edit_state.on_input_released(input_event);
}

bool PropertyComponent::on_input_doubleclick(const CL_InputEvent &input_event)
{
	return edit_state.on_input_doubleclick(input_event);
}

bool PropertyComponent::on_input_pointer_moved(const CL_InputEvent &input_event)
{
	return edit_state.on_input_pointer_moved(input_event);
}

void PropertyComponent::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	update_layout();
	update_scrollbar_size(gc);

	part_background.render_box(gc, get_size(), update_rect);
	part_header.render_box(gc, header, update_rect);
	part_header_name.render_box(gc, header_name, update_rect);
	part_header_value.render_box(gc, header_value, update_rect);
	part_header_name.render_text(gc, "Property", part_header_name.get_content_box(header_name), update_rect);
	part_header_value.render_text(gc, "Value", part_header_value.get_content_box(header_value), update_rect);

	value_boxes.clear();
	int row_width = content.get_width(); // to do: account for shrink box
	int y = body.top;
	for (size_t i = scrollbar->get_position(); i < items.size() && y < body.bottom; i++)
	{
		int row_height = items[i]->get_inactive_height(gc, part_item_value, row_width-name_column_width);
		if (row_height < part_item_name.get_preferred_height())
			row_height = part_item_name.get_preferred_height();
		CL_Rect row(body.left, y, body.right, y+row_height);
		CL_Rect row_content = part_row[i%2].get_content_box(row);
		CL_Rect name_box(row_content.left, row_content.top, row_content.left+name_column_width, row_content.bottom);
		CL_Rect value_box(row_content.left+name_column_width, row_content.top, row_content.right, row_content.bottom);
		part_row[i%2].render_box(gc, row, update_rect);
		part_item_name.render_box(gc, name_box, update_rect);
		part_item_name.render_text(gc, items[i]->name, part_item_name.get_content_box(name_box), update_rect);
		part_item_value.render_box(gc, value_box, update_rect);
		value_boxes.push_back(value_box);

		if (items[i] != active_item || active_component == 0)
			items[i]->render_inactive(gc, part_item_value, value_box, update_rect);
		else
			active_component->set_geometry(value_box);
		y = row.bottom;
	}
}

void PropertyComponent::deactivate()
{
	if (active_item)
	{
		active_item->deactivate(active_component);
		std::vector<HolderComponent *> selection = main_window->get_selection()->get_selection();
		for (size_t i = 0; i < selection.size(); i++)
		{
			try
			{
				active_item->apply_changes(selection[i]);
			}
			catch (CL_Exception &)
			{
				// to do: display a message box here perhaps?
			}
		}

		active_item = 0;
		active_component = 0;
		request_repaint();
		main_window->get_grid_component()->request_repaint();
	}
}

int PropertyComponent::hit_test(const CL_Point &pos) const
{
	for (size_t i = 0; i < value_boxes.size(); i++)
	{
		if (value_boxes[i].contains(pos))
			return i;
	}
	return -1;
}

void PropertyComponent::activate_item(int index)
{
	if (items[index] != active_item)
	{
		deactivate();
		active_item = items[index];
		active_component = items[index]->activate();
		if(active_component)
			active_component->set_focus();
		request_repaint();
	}
}

void PropertyComponent::on_filter_message(CL_GUIMessage &message)
{
	if (active_component)
	{
		if (message.get_type() == CL_GUIMessage_Input::get_type_name())
		{
			CL_GUIMessage_Input input_message = message;
			CL_InputEvent e = input_message.get_event();
			if (e.type == CL_InputEvent::pressed && e.device.get_type() == CL_InputDevice::pointer)
			{
				if (active_component != message.get_target() && !active_component->is_ancestor_of(message.get_target()))
					deactivate();
			}
		}
	}
}

void PropertyComponent::update_layout()
{
	CL_Rect geometry = get_size();
	content = part_background.get_content_box(geometry);
	header = CL_Rect(content.left, content.top, content.right, content.top+part_header.get_preferred_height());
	body = CL_Rect(content.left, header.bottom, content.right, content.bottom);
	header_content = part_header.get_content_box(header);
	header_name = CL_Rect(header_content.left, header_content.top, header_content.left+name_column_width, header_content.bottom);
	header_value = CL_Rect(header_name.right, header_content.top, header_content.right, header_content.bottom);
}

void PropertyComponent::update_scrollbar_size(CL_GraphicContext &gc)
{
	int row_width = content.get_width(); // to do: account for shrink box
	int y = content.bottom;
	size_t i;
	for (i = items.size(); i > 0 && y > content.top; i--)
		y -= items[i-1]->get_inactive_height(gc, part_item_value, row_width-name_column_width);

	int page_step = items.size() - i;
	if (page_step > 0)
	{
		scrollbar->set_ranges(0, items.size()/*-page_step+1*/, 1, page_step);
		scrollbar->set_visible(true);
	}
	else
	{
		scrollbar->set_ranges(0, 1, 1, 1);
		scrollbar->set_visible(false);
	}
}

CL_Rect PropertyComponent::get_header_resize_grabber() const
{
	return CL_Rect(header_name.right-3, header.top, header_name.right+2, header.bottom);
}
