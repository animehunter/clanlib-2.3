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
**    Harry Storbacka
**    Magnus Norddahl
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/gui_message_focus_change.h"
#include "API/Core/System/timer.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/GUI/Components/slider.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Font/font.h"
#include "API/Display/Window/keys.h"
#include "../gui_css_strings.h"


/*
            ........... - Ticks
		   O====||=====O 
		   |     |  |  |
		   |     |  |  |
		   |     |  |  '- Track end
		   |     |  '- Track    
		   |     '- Thumb
		   '--- Track Start
*/

/////////////////////////////////////////////////////////////////////////////
// CL_Slider_Impl Class:

class CL_Slider_Impl
{
public:
	CL_Slider_Impl() : 
		vertical(false),
		lock_to_ticks(false),
		slider_min(0),
		slider_max(100),
		tick_count(10),
		position(0),
		tick_width(10),
		page_step(10),
		last_step_size(0),
		slider_track_width(0)
	{
	}

	void on_process_message(CL_GUIMessage &msg);
	void on_enablemode_changed();
	void on_mouse_move(CL_GUIMessage_Input &input, CL_InputEvent &input_event);
	void on_mouse_lbutton_down(CL_GUIMessage_Input &input, CL_InputEvent &input_event);
	void on_mouse_lbutton_up(CL_GUIMessage_Input &input, CL_InputEvent &input_event);
	void on_mouse_leave();
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_timer_expired();
	void on_resized();

	void create_parts();

	void update_part_positions();

	bool vertical;

	bool lock_to_ticks;

	int slider_min, slider_max, tick_count, position;
	int tick_width, page_step;
	int last_step_size;
	int slider_track_width;
	int slider_track_height;
	int mouse_drag_start_value;

	enum MouseDownMode
	{
		mouse_down_none,
		mouse_down_track_decr,
		mouse_down_track_incr,
		mouse_down_thumb_drag
	} mouse_down_mode;

	CL_Slider *slider;

	CL_Rect rect_thumb, rect_track, rect_track_decrement, rect_track_increment;

	CL_GUIThemePart part_component;
	CL_GUIThemePart part_track;
	CL_GUIThemePart part_thumb;
	CL_GUIThemePart part_focus;

	CL_Timer mouse_down_timer;

	int thumb_start_position;

	CL_Point mouse_drag_start_pos;

	CL_Callback_v0 func_value_changed;
	CL_Callback_v0 func_slider_decrement;
	CL_Callback_v0 func_slider_increment;
	CL_Callback_v0 func_slider_moved;
};

/////////////////////////////////////////////////////////////////////////////
// CL_Slider Construction:

CL_Slider::CL_Slider(CL_GUIComponent *parent)
: CL_GUIComponent(parent), impl(new CL_Slider_Impl)
{
	set_type_name(CssStr::Slider::type_name);
	set_focus_policy(focus_local);
	impl->slider = this;
	impl->vertical = false;
	impl->position = 0;
	func_process_message().set(impl.get(), &CL_Slider_Impl::on_process_message);
	func_render().set(impl.get(), &CL_Slider_Impl::on_render);
	func_resized().set(impl.get(), &CL_Slider_Impl::on_resized);
	func_enablemode_changed().set(impl.get(), &CL_Slider_Impl::on_enablemode_changed);
	impl->create_parts();
}

CL_Slider::~CL_Slider()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Slider Attributes:

CL_Slider *CL_Slider::get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id)
{
	CL_Slider *object = NULL;
	if (reference_component)
		object = dynamic_cast<CL_Slider*>(reference_component->get_named_item(id));

	if (!object)
		throw CL_Exception(cl_format("Cannot find CL_Slider named item: %1", id));

	return object;
}

bool CL_Slider::is_vertical() const
{
	return impl->vertical;
}

bool CL_Slider::is_horizontal() const
{
	return !impl->vertical;
}

int CL_Slider::get_min() const
{
	return impl->slider_min;
}

int CL_Slider::get_max() const
{
	return impl->slider_max;
}

int CL_Slider::get_tick_count() const
{
	return impl->tick_count;
}

int CL_Slider::get_position() const
{
	return impl->position;
}

int CL_Slider::get_page_step() const
{
	return impl->page_step;
}

bool CL_Slider::get_lock_to_ticks() const
{
	return impl->lock_to_ticks;
}

CL_Size CL_Slider::get_preferred_size() const
{
	return impl->part_component.get_preferred_size();
}

/////////////////////////////////////////////////////////////////////////////
// CL_Slider Operations:

void CL_Slider::set_vertical(bool enable)
{
	impl->vertical = enable;
	impl->create_parts();
	impl->update_part_positions();
	request_repaint();
}

void CL_Slider::set_horizontal(bool enable)
{
	set_vertical(!enable);
}

void CL_Slider::set_min(int slider_min)
{
	impl->slider_min = slider_min;
	request_repaint();
}

void CL_Slider::set_max(int slider_max)
{
	impl->slider_max = slider_max;
	request_repaint();
}

void CL_Slider::set_tick_count(int count)
{
	if (count < 2)
		throw CL_Exception("CL_Slider::set_tick_count: tick count in slider can not be < 2.");

	impl->tick_count = count;
	request_repaint();
}

void CL_Slider::set_ranges(int slider_min, int slider_max, unsigned int tick_count, int page_step)
{
	if (tick_count < 2)
		throw CL_Exception(cl_format("CL_Slider: invalid tick count: %1. Must be >= 2.", tick_count));

	impl->slider_min = slider_min;
	impl->slider_max = slider_max;
	impl->tick_count = tick_count;
	impl->page_step = page_step;
	request_repaint();
}

void CL_Slider::set_position(int pos)
{
	impl->position = pos;
	impl->update_part_positions();
	request_repaint();
}

void CL_Slider::set_page_step(int steps)
{
	impl->page_step = steps;
}

void CL_Slider::set_lock_to_ticks(bool value)
{
	impl->lock_to_ticks = value;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Slider Callbacks:

CL_Callback_v0 &CL_Slider::func_value_changed()
{
	return impl->func_value_changed;
}

CL_Callback_v0 &CL_Slider::func_value_decremented()
{
	return impl->func_slider_decrement;
}

CL_Callback_v0 &CL_Slider::func_value_incremented()
{
	return impl->func_slider_increment;
}

CL_Callback_v0 &CL_Slider::func_slider_moved()
{
	return impl->func_slider_moved;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Slider Implementation:

void CL_Slider_Impl::on_process_message(CL_GUIMessage &msg)
{
	if (!slider->is_enabled())
		return;

	if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
	{
		CL_GUIMessage_Input input = msg;
		CL_InputEvent e = input.get_event();
		if (e.type == CL_InputEvent::pointer_moved)
		{
			on_mouse_move(input, e);
			msg.set_consumed();
		}
		else if (e.type == CL_InputEvent::pressed && e.id == CL_MOUSE_LEFT)
		{
			on_mouse_lbutton_down(input, e);
			msg.set_consumed();
		}
		else if (e.type == CL_InputEvent::released && e.id == CL_MOUSE_LEFT)
		{
			on_mouse_lbutton_up(input, e);
			msg.set_consumed();
		}
		else if (e.type == CL_InputEvent::pressed && (e.id == CL_KEY_LEFT || e.id == CL_KEY_UP))
		{
			slider->set_position(position - page_step);
			if (!func_value_changed.is_null())
				func_value_changed.invoke();
			if (!func_slider_decrement.is_null())
				func_slider_decrement.invoke();
			if (!func_slider_moved.is_null())
				func_slider_moved.invoke();

			msg.set_consumed();
		}
		else if (e.type == CL_InputEvent::pressed && (e.id == CL_KEY_RIGHT || e.id == CL_KEY_DOWN))
		{
			slider->set_position(position + page_step);

			if (!func_value_changed.is_null())
				func_value_changed.invoke();
			if (!func_slider_increment.is_null())
				func_slider_increment.invoke();
			if (!func_slider_moved.is_null())
				func_slider_moved.invoke();
			msg.set_consumed();
		}
	}
	else if (msg.is_type(CL_GUIMessage_Pointer::get_type_name()))
	{
		CL_GUIMessage_Pointer pointer = msg;
		if (pointer.get_pointer_type() == CL_GUIMessage_Pointer::pointer_leave)
			on_mouse_leave();
	}
	else if (msg.is_type(CL_GUIMessage_FocusChange::get_type_name()))
	{
		CL_GUIMessage_FocusChange focus_msg = msg;
		if (focus_msg.get_focus_type() == CL_GUIMessage_FocusChange::gained_focus)
		{
			part_component.set_state(CssStr::focused, true);
			slider->request_repaint();
		}
		else 
		{
			part_component.set_state(CssStr::focused, false);
			slider->request_repaint();
		}
		msg.set_consumed();
	}
}

void CL_Slider_Impl::on_mouse_move(CL_GUIMessage_Input &input, CL_InputEvent &input_event)
{
	int original_slider_position = position;
	CL_Point mouse_pos = input_event.mouse_pos;
	part_component.set_state(CssStr::hot, true);
	part_track.set_state(CssStr::hot, true);
	part_component.set_state(CssStr::normal, false);
	part_track.set_state(CssStr::normal, false);
	part_focus.set_state(CssStr::focused, false);

	if (mouse_down_mode != mouse_down_thumb_drag)
	{
		bool thumb_hot = rect_thumb.contains(mouse_pos);
		part_thumb.set_state(CssStr::hot, thumb_hot );
		part_thumb.set_state(CssStr::normal, !thumb_hot);
	}
	slider->request_repaint();

	if (mouse_down_mode == mouse_down_thumb_drag)
	{
		if (mouse_pos.x < -100 || mouse_pos.x > slider->get_width()+100 ||
			mouse_pos.y < -100 || mouse_pos.y > slider->get_height()+100)
		{
			position = thumb_start_position;
		}
		else
		{
			if (vertical)
			{
				int delta = mouse_pos.y - mouse_drag_start_pos.y;
				position = thumb_start_position + (delta*(slider_max-slider_min)/rect_track.get_height());
			}
			else
			{
				int delta = mouse_pos.x - mouse_drag_start_pos.x;
				position = thumb_start_position + (delta*(slider_max-slider_min)/rect_track.get_width());
			}
			if (position < slider_min) position = slider_min;
			if (position > slider_max) position = slider_max;
		}
		slider->request_repaint();
	}

	if (position != original_slider_position)
	{
		if (!func_value_changed.is_null())
			func_value_changed.invoke();
	}
}

void CL_Slider_Impl::on_mouse_lbutton_down(CL_GUIMessage_Input &input, CL_InputEvent &input_event)
{
	int original_position = position;
	CL_Point pos = input_event.mouse_pos;
	mouse_drag_start_pos = pos;
	mouse_drag_start_value = position;

	if (rect_thumb.contains(pos))
	{
		mouse_down_mode = mouse_down_thumb_drag;
		thumb_start_position = position;
	}
	else if (rect_track_decrement.contains(pos))
	{
		mouse_down_mode = mouse_down_track_decr;

		position -= page_step;
		last_step_size = -page_step;
		if (position < slider_min) position = slider_min;
		if (position > slider_max) position = slider_max;
		if (!func_slider_decrement.is_null())
			func_slider_decrement.invoke();
	}
	else if (rect_track_increment.contains(pos))
	{
		mouse_down_mode = mouse_down_track_incr;

		position += page_step;
		last_step_size = page_step;
		if (position < slider_min) position = slider_min;
		if (position > slider_max) position = slider_max;
		if (!func_slider_increment.is_null())
			func_slider_increment.invoke();
	}

	mouse_down_timer.start(100,false);

	slider->request_repaint();
	slider->capture_mouse(true);

	if (position != original_position)
	{
		if (!func_value_changed.is_null())
			func_value_changed.invoke();
	}
}

void CL_Slider_Impl::on_mouse_lbutton_up(CL_GUIMessage_Input &input, CL_InputEvent &input_event)
{
	mouse_down_timer.stop();

	if ((mouse_down_mode == mouse_down_thumb_drag) && (mouse_drag_start_value != position))
		if (!func_slider_moved.is_null())
			func_slider_moved.invoke();

	mouse_down_mode = mouse_down_none;

	slider->request_repaint();
	slider->capture_mouse(false);
}

void CL_Slider_Impl::on_mouse_leave()
{
	part_component.set_state(CssStr::hot, false);
	part_thumb.set_state(CssStr::hot, false);
	part_component.set_state(CssStr::normal, true);
	part_thumb.set_state(CssStr::normal, true);
	slider->request_repaint();
}

void CL_Slider_Impl::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	update_part_positions();
	
	CL_Rect rect = slider->get_geometry();
	part_component.render_box(gc, rect, update_rect);
	part_track.render_box(gc, rect_track, update_rect);
	part_thumb.render_box(gc, rect_thumb, update_rect);
	
	if (slider->has_focus())
	{
		CL_Rect content_rect = part_component.get_content_box(rect.get_size());
		part_focus.render_box(gc, content_rect, update_rect);
	}
}

void CL_Slider_Impl::create_parts()
{
	bool vertical = slider->is_vertical();
	part_component = CL_GUIThemePart(slider);
	part_track = CL_GUIThemePart(slider, vertical ? CssStr::Slider::part_track_vertical : CssStr::Slider::part_track_horizontal);
	part_thumb = CL_GUIThemePart(slider, vertical ? CssStr::Slider::part_thumb_vertical : CssStr::Slider::part_thumb_horizontal);
	part_focus = CL_GUIThemePart(slider, CssStr::Slider::part_focus);

	bool enabled = slider->is_enabled();

	part_component.set_state(CssStr::normal, enabled);
	part_track.set_state(CssStr::normal, enabled);
	part_thumb.set_state(CssStr::normal, enabled);
	part_focus.set_state(CssStr::normal, true);

	part_component.set_state(CssStr::disabled, !enabled);
	part_track.set_state(CssStr::disabled, !enabled);
	part_thumb.set_state(CssStr::disabled, !enabled);

}

void CL_Slider_Impl::update_part_positions()
{
	if (position < slider_min) position = slider_min;
	if (position > slider_max) position = slider_max;

	if (lock_to_ticks)
	{
		int remainder = (position - slider_min) % tick_count;
		position -= remainder;
	}

	int offset_position = position - slider_min;

	bool vertical = slider->is_vertical();

	CL_Rect rect(slider->get_geometry().get_size());
	CL_Rect content_rect = part_component.get_content_box(rect);
	CL_Size thumb_size = part_thumb.get_preferred_size();

	if (vertical)
	{
		int height = content_rect.get_height();
		slider_track_height = height - thumb_size.height / 2 - 1;

		// track rect (for drawing the visuals)
		rect_track.left = content_rect.get_center().x - part_track.get_preferred_width()/2;
		rect_track.right = rect_track.left + part_track.get_preferred_width();
		rect_track.top = content_rect.top;
		rect_track.bottom = content_rect.bottom;

		// thumb rect
		int thumb_y = int((float(offset_position)/float(slider_max-slider_min)) * slider_track_height);

		rect_thumb.left = content_rect.left;
		rect_thumb.right = content_rect.right;
		rect_thumb.top = thumb_y - thumb_size.height/2;
		if (rect_thumb.top < 0) 
			rect_thumb.top = 0;
		rect_thumb.bottom = rect_thumb.top + thumb_size.height;

		// track decrement rect (for clicking)
		rect_track_increment.left = content_rect.left;
		rect_track_increment.right = content_rect.right;
		rect_track_increment.top = rect_thumb.bottom;
		rect_track_increment.bottom = content_rect.bottom;

		// track increment rect (for clicking)
		rect_track_decrement.left = content_rect.left;
		rect_track_decrement.right = content_rect.right;
		rect_track_decrement.top = content_rect.top;
		rect_track_decrement.bottom = rect_thumb.top;
	}
	else
	{
		int width = content_rect.get_width();
		slider_track_width = width - thumb_size.width / 2 - 1;

		// track rect (for drawing the visuals)
		rect_track.left = content_rect.left;
		rect_track.right = rect_track.left + width;
		rect_track.top = content_rect.get_height()/2 - part_track.get_preferred_height()/2;
		rect_track.bottom = rect_track.top + part_track.get_preferred_height();

		// thumb rect
		int thumb_x = int((float(offset_position)/float(slider_max-slider_min)) * slider_track_width);

		rect_thumb.left = thumb_x - thumb_size.width/2;
		if (rect_thumb.left < 0) rect_thumb.left = 0;
		rect_thumb.right = rect_thumb.left + thumb_size.width;
		rect_thumb.top = content_rect.get_height()/2 - thumb_size.height/2;
		rect_thumb.bottom = rect_thumb.top + thumb_size.height;

		// track decrement rect (for clicking)
		rect_track_decrement.left = content_rect.left;
		rect_track_decrement.right = rect_thumb.left;
		rect_track_decrement.top = content_rect.top;
		rect_track_decrement.bottom = content_rect.bottom;

		// track increment rect (for clicking)
		rect_track_increment.left = rect_thumb.right;
		rect_track_increment.right = content_rect.right;
		rect_track_increment.top = content_rect.top;
		rect_track_increment.bottom = content_rect.bottom;
	}
}

void CL_Slider_Impl::on_timer_expired()
{
	if (mouse_down_mode == mouse_down_thumb_drag)
		return;
	mouse_down_timer.start(100, false);
	position += last_step_size;
	if (position < slider_min) position = slider_min;
	if (position > slider_max) position = slider_max;

	slider->request_repaint();
}

void CL_Slider_Impl::on_resized()
{
	CL_GraphicContext &gc = slider->get_gc();
	update_part_positions();
}

void CL_Slider_Impl::on_enablemode_changed()
{
	bool enabled = slider->is_enabled();

	part_component.set_state(CssStr::normal, enabled);
	part_track.set_state(CssStr::normal, enabled);
	part_thumb.set_state(CssStr::normal, enabled);

	part_component.set_state(CssStr::disabled, !enabled);
	part_track.set_state(CssStr::disabled, !enabled);
	part_thumb.set_state(CssStr::disabled, !enabled);
	slider->request_repaint();
}


