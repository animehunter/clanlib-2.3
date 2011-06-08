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
#include "progressbar.h"

ProgressBar::ProgressBar(CL_GUIManager &manager) :
	CL_Window(&manager, CL_GUITopLevelDescription("Progress Bar", CL_Rect(24 + 256*2, 256*1 + 180*2 + 32, CL_Size(256, 180)), false))
{
	set_draggable(true);

	CL_Rect client_area = get_client_area();

	progressbar1 = new CL_ProgressBar(this);
	progressbar1->set_geometry(CL_Rect(client_area.left + 5, client_area.top + 5, CL_Size(128, 32)));
	progressbar1->set_min(0);
	progressbar1->set_max(100);
	progressbar1->set_step_size(10);
	progressbar1->set_position(20);
	progressbar1->set_marquee_animation_speed(1000);
	progressbar1->set_marquee_mode(false);

	CL_Size lineedit_size(48, 20);
	CL_Size label_size(50, 15);
	int lineedit_xpos = client_area.left + 5;
	int lineedit_ypos = client_area.top + 40;
	int label_xpos = client_area.left + 55;
	const int lineedit_gap = 25;

	lineedit_min = new CL_LineEdit(this);
	lineedit_min->set_geometry(CL_Rect(lineedit_xpos, lineedit_ypos, lineedit_size));
	lineedit_min->set_text("0");
	lineedit_min->set_numeric_mode(true);
	lineedit_min->func_enter_pressed().set(this, &ProgressBar::on_min_enter_pressed, lineedit_min);

	lineedit_label_min = new CL_Label(this);
	lineedit_label_min->set_geometry(CL_Rect(label_xpos, lineedit_ypos, label_size));
	lineedit_label_min->set_text("Min");
	lineedit_ypos += lineedit_gap;

	lineedit_max = new CL_LineEdit(this);
	lineedit_max->set_geometry(CL_Rect(lineedit_xpos, lineedit_ypos, lineedit_size));
	lineedit_max->set_text("100");
	lineedit_max->set_numeric_mode(true);
	lineedit_max->func_enter_pressed().set(this, &ProgressBar::on_max_enter_pressed, lineedit_max);

	lineedit_label_max = new CL_Label(this);
	lineedit_label_max->set_geometry(CL_Rect(label_xpos, lineedit_ypos, label_size));
	lineedit_label_max->set_text("Max");
	lineedit_ypos += lineedit_gap;


	lineedit_step_size = new CL_LineEdit(this);
	lineedit_step_size->set_geometry(CL_Rect(lineedit_xpos, lineedit_ypos, lineedit_size));
	lineedit_step_size->set_text("10");
	lineedit_step_size->func_enter_pressed().set(this, &ProgressBar::on_step_size_enter_pressed, lineedit_step_size);

	lineedit_label_step_size = new CL_Label(this);
	lineedit_label_step_size->set_geometry(CL_Rect(label_xpos, lineedit_ypos, label_size));
	lineedit_label_step_size->set_text("Step Size");
	lineedit_ypos += lineedit_gap;

	lineedit_position = new CL_LineEdit(this);
	lineedit_position->set_geometry(CL_Rect(lineedit_xpos, lineedit_ypos, lineedit_size));
	lineedit_position->set_text("20");
	lineedit_position->func_enter_pressed().set(this, &ProgressBar::on_position_enter_pressed, lineedit_position);

	lineedit_label_position = new CL_Label(this);
	lineedit_label_position->set_geometry(CL_Rect(label_xpos, lineedit_ypos, label_size));
	lineedit_label_position->set_text("Position");
	lineedit_ypos += lineedit_gap;

	lineedit_xpos = client_area.left + 110;
	lineedit_ypos = client_area.top + 40;
	label_xpos = client_area.left + 160;


	checkbox_marquee_mode = new CL_CheckBox(this);
	checkbox_marquee_mode->set_geometry(CL_Rect(lineedit_xpos, lineedit_ypos, CL_Size(100, 15)));
	checkbox_marquee_mode->func_checked().set(this, &ProgressBar::on_checked_marquee_mode, checkbox_marquee_mode);
	checkbox_marquee_mode->func_unchecked().set(this, &ProgressBar::on_unchecked_marquee_mode, checkbox_marquee_mode);
	checkbox_marquee_mode->set_text("Marquee Mode");

	lineedit_ypos += lineedit_gap;

	lineedit_marquee_speed = new CL_LineEdit(this);
	lineedit_marquee_speed->set_geometry(CL_Rect(lineedit_xpos, lineedit_ypos, lineedit_size));
	lineedit_marquee_speed->set_text("1000");
	lineedit_marquee_speed->func_enter_pressed().set(this, &ProgressBar::on_marquee_speed_enter_pressed, lineedit_marquee_speed);

	lineedit_label_marquee_speed = new CL_Label(this);
	lineedit_label_marquee_speed->set_geometry(CL_Rect(label_xpos, lineedit_ypos, label_size));
	lineedit_label_marquee_speed->set_text("Marquee Speed");
	lineedit_ypos += lineedit_gap;
	lineedit_marquee_speed->set_enabled(false);

	pushbutton_apply = new CL_PushButton(this);
	pushbutton_apply->set_geometry(CL_Rect(lineedit_xpos, lineedit_ypos, CL_Size(48, 20)));
	pushbutton_apply->set_text("Apply");
	pushbutton_apply->func_clicked().set(this, &ProgressBar::on_apply_clicked, pushbutton_apply);

	lineedit_ypos += lineedit_gap;

	pushbutton_step_position = new CL_PushButton(this);
	pushbutton_step_position->set_geometry(CL_Rect(lineedit_xpos, lineedit_ypos, CL_Size(92, 20)));
	pushbutton_step_position->set_text("Step Position");
	pushbutton_step_position->func_clicked().set(this, &ProgressBar::on_step_position_clicked, pushbutton_step_position);

}

void ProgressBar::on_checked_marquee_mode(CL_CheckBox *checkbox)
{
	progressbar1->set_marquee_mode(true);
	lineedit_marquee_speed->set_enabled(true);
}

void ProgressBar::on_unchecked_marquee_mode(CL_CheckBox *checkbox)
{
	progressbar1->set_marquee_mode(false);
	lineedit_marquee_speed->set_enabled(false);
}

void ProgressBar::on_min_enter_pressed(CL_LineEdit *lineedit)
{
	int value = CL_StringHelp::text_to_int(lineedit->get_text());
	progressbar1->set_min(value);
}

void ProgressBar::on_max_enter_pressed(CL_LineEdit *lineedit)
{
	int value = CL_StringHelp::text_to_int(lineedit->get_text());
	progressbar1->set_max(value);
}

void ProgressBar::on_step_size_enter_pressed(CL_LineEdit *lineedit)
{
	int value = CL_StringHelp::text_to_int(lineedit->get_text());
	progressbar1->set_step_size(value);
}

void ProgressBar::on_position_enter_pressed(CL_LineEdit *lineedit)
{
	int value = CL_StringHelp::text_to_int(lineedit->get_text());
	progressbar1->set_position(value);
}

void ProgressBar::on_marquee_speed_enter_pressed(CL_LineEdit *lineedit)
{
	int value = CL_StringHelp::text_to_int(lineedit->get_text());
	progressbar1->set_marquee_animation_speed(value);
}

void ProgressBar::on_apply_clicked(CL_PushButton *pushbutton)
{
	int value = CL_StringHelp::text_to_int(lineedit_min->get_text());
	progressbar1->set_min(value);

	value = CL_StringHelp::text_to_int(lineedit_max->get_text());
	progressbar1->set_max(value);

	value = CL_StringHelp::text_to_int(lineedit_step_size->get_text());
	progressbar1->set_step_size(value);

	value = CL_StringHelp::text_to_int(lineedit_position->get_text());
	progressbar1->set_position(value);

	if (lineedit_marquee_speed->is_enabled())
	{
		value = CL_StringHelp::text_to_int(lineedit_marquee_speed->get_text());
		progressbar1->set_marquee_animation_speed(value);
	}

}

void ProgressBar::on_step_position_clicked(CL_PushButton *pushbutton)
{
	progressbar1->step_position();
}
