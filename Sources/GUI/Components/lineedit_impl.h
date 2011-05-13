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

#pragma once

class CL_LineEdit_Impl
{
public:
	CL_LineEdit_Impl() :
		alignment(CL_LineEdit::align_left),
		cursor_pos(0),
		max_length(-1),
		lowercase(false),
		uppercase(false),
		password_mode(false),
		numeric_mode(false),
		numeric_mode_decimals(false),
		readonly(false),
		mouse_moves_left(false),
		mouse_selecting(false),
		selection_start(-1),
		selection_length(0),
		cursor_blink_visible(true),
		clip_start_offset(0),
		clip_end_offset(0),
		decimal_char("."),
		ignore_mouse_events(false),
		cursor_drawing_enabled_when_parent_focused(false),
		select_all_on_focus_gain(true)
	{
		prop_text_color = CL_GUIThemePartProperty(CssStr::text_color, "black");
	}

	~CL_LineEdit_Impl()
	{
		timer.stop();
		scroll_timer.stop();
	}

	void on_process_message(CL_GUIMessage &msg);
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_style_changed();
	void on_timer_expired();
	void on_resized();
	void on_scroll_timer_expired();
	void on_enable_changed();
	void update_text_clipping();
	void create_parts();

	CL_Callback_v1<CL_InputEvent &> func_before_edit_changed;
	CL_Callback_v1<CL_InputEvent &> func_after_edit_changed;
	CL_Callback_v0 func_selection_changed;
	CL_Callback_v0 func_focus_gained;
	CL_Callback_v0 func_focus_lost;
	CL_Callback_v0 func_enter_pressed;

	CL_LineEdit *lineedit;
	CL_Timer timer;
	CL_String text;
	CL_LineEdit::Alignment alignment;
	int cursor_pos;
	int max_length;
	bool mouse_selecting;
	bool lowercase;
	bool uppercase;
	bool password_mode;
	bool numeric_mode;
	bool numeric_mode_decimals;
	bool readonly;
	int selection_start;
	int selection_length;
	CL_Colorf text_color;
	CL_String input_mask;
	CL_String decimal_char;
	bool cursor_drawing_enabled_when_parent_focused;

	static CL_String break_characters;

	CL_GUIThemePart part_component;
	CL_GUIThemePart part_selection;
	CL_GUIThemePart part_cursor;
	CL_GUIThemePartProperty prop_text_color;
	CL_Rect content_rect;

	void move(int steps, CL_InputEvent &e);
	void insert_text(int pos, const CL_StringRef &str);
	void backspace();
	void del();
	int get_character_index(int mouse_x_wincoords);
	int find_next_break_character(int pos);
	int find_previous_break_character(int pos);
	CL_String get_visible_text_before_selection();
	CL_String get_visible_text_after_selection();
	CL_String get_visible_selected_text();
	CL_String create_password(CL_String::size_type num_letters) const;
	CL_Size get_visual_text_size(CL_GraphicContext &gc, CL_Font &font, int pos, int npos) const;
	CL_Size get_visual_text_size(CL_GraphicContext &gc, CL_Font &font) const;
	CL_Rect get_cursor_rect();
	CL_Rect get_selection_rect();
	bool input_mask_accepts_input(int cursor_pos, const CL_StringRef &str);

	CL_GUIThemePart::VerticalTextPosition vertical_text_align;
	CL_Timer scroll_timer;

	bool mouse_moves_left;
	bool cursor_blink_visible;
	unsigned int blink_timer;
	int clip_start_offset;
	int clip_end_offset;
	bool ignore_mouse_events;

	struct UndoInfo
	{
		/* set undo text when:
  		   - added char after moving
		   - destructive block operation (del, cut etc)
		   - beginning erase
		*/

		UndoInfo() : first_erase(0), first_text_insert(0) {}
		CL_String undo_text;
		bool first_erase;
		bool first_text_insert;
	} undo_info;

	bool select_all_on_focus_gain;

	static const CL_StringRef numeric_mode_characters;
};
