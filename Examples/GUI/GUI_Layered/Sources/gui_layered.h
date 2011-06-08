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

#pragma once

#include "../../CommonCode/Sources/scrollbar.h"
#include "../../CommonCode/Sources/pushbutton.h"
#include "../../CommonCode/Sources/lineedit.h"
#include "../../CommonCode/Sources/checkbox.h"
#include "../../CommonCode/Sources/slider.h"
#include "../../CommonCode/Sources/radiobutton.h"
#include "../../CommonCode/Sources/progressbar.h"
#include "../../CommonCode/Sources/tabpage.h"
#include "../../CommonCode/Sources/menubar.h"
#include "../../CommonCode/Sources/spin.h"
#include "../../CommonCode/Sources/combobox.h"
#include "../../CommonCode/Sources/theme.h"
#include "../../CommonCode/Sources/listview.h"
#include "panel3d.h"

class GUI;

class GUI_Layered
{
public:
	GUI_Layered(GUI *gui);
	~GUI_Layered();

	bool run();

	CL_DisplayWindow *get_window() { return window_ptr; }
	CL_GUIManager &get_gui_manager();
	CL_ResourceManager &get_resources_internal();
	CL_ResourceManager &get_resources_gui();

private:
	void set_projection_matrix();
	void wm_input_intercept(CL_InputEvent &input_event, CL_InputState &input_state);
	CL_Vec3f transform_point(const CL_Vec3d &src_point, const CL_Mat4d &matrix);
	float get_zpos_at_position(float xpos, float ypos);
	bool run3d();
	void setup_shader();
	bool on_close(CL_Window *win);

private:
	CL_ResourceManager resources_gui;
	CL_GUIThemeDefault theme;
	GUI *gui;
	CL_DisplayWindow *window_ptr;
	CL_GUIWindowManagerTexture wm;

	ScrollBar *scrollbar;
	PushButton *pushbutton;
	LineEdit *lineedit;
	CheckBox *checkbox;
	Slider *slider;
	RadioButton *radiobutton;
	ProgressBar *progressbar;
	TabPage *tabpage;
	MenuBar *menubar;
	Spin *spin;
	ComboBox *combobox;
	ListView *listview;
	Panel3D *panel3d;

	CL_ProgramObject gui_shader;

	float lens_zoom;
	float lens_aspect;
	float lens_near;
	float lens_far;

	float saved_x1;
	float saved_x2;
	float saved_y1;
	float saved_y2;
	float saved_z1;
	float saved_z2;

	CL_Mat4f projection_matrix;
	CL_Mat4f modelview_matrix;
	CL_Mat4f resultant_matrix;

	float wobble_offset;
	unsigned int last_time;

	float current_zoffset;

};



