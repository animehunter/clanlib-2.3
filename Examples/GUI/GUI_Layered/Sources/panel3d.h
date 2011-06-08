/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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

#include "../../CommonCode/Sources/info.h"

class GUI;

class Panel3D : public CL_Window
{
public:
	Panel3D(GUI *gui);

	float get_zoffset();
	bool is_animated();

	float get_light_position_x();
	float get_light_position_y();
	float get_light_position_z();
	float get_light_specular();
	float get_light_diffuse();
	float get_material_shininess();
	float get_material_emission();
	float get_material_specular();
	float get_material_transparency();

private:
	void on_slider_changed(CL_Slider *slider);
	void set_slider_to_spin(CL_Slider *slider, CL_Spin *spin);
	void on_spin_changed(CL_Spin *spin);

	void set_control_group(int xoffset, int yoffset, CL_Slider *&slider,
			CL_Spin *&spin, CL_Label *&label, const char *label_name, 
			float min_value, float max_value, float step_value, float initial_value);

private:
	CL_Slider *slider_zoffset;
	CL_Spin *spin_zoffset;
	CL_Label *label_zoffset;

	CL_Slider *slider_light_position_x;
	CL_Spin *spin_light_position_x;
	CL_Label *label_light_position_x;

	CL_Slider *slider_light_position_y;
	CL_Spin *spin_light_position_y;
	CL_Label *label_light_position_y;

	CL_Slider *slider_light_position_z;
	CL_Spin *spin_light_position_z;
	CL_Label *label_light_position_z;

	CL_Slider *slider_light_specular;
	CL_Spin *spin_light_specular;
	CL_Label *label_light_specular;

	CL_Slider *slider_light_diffuse;
	CL_Spin *spin_light_diffuse;
	CL_Label *label_light_diffuse;

	CL_Slider *slider_material_shininess;
	CL_Spin *spin_material_shininess;
	CL_Label *label_material_shininess;

	CL_Slider *slider_material_emission;
	CL_Spin *spin_material_emission;
	CL_Label *label_material_emission;

	CL_Slider *slider_material_specular;
	CL_Spin *spin_material_specular;
	CL_Label *label_material_specular;

	CL_Slider *slider_material_transparency;
	CL_Spin *spin_material_transparency;
	CL_Label *label_material_transparency;

	CL_CheckBox *checkbox_animate;

private:
	GUI *gui;

};
