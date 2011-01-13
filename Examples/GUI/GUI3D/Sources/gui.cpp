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

#include "gui.h"
#include "app.h"
#include "panel.h"

GUI::GUI(App *app) : app(app), window(app->get_window()), wm(window)
{
	CL_String theme;
	if (CL_FileHelp::file_exists("../../../Resources/GUIThemeAero/theme.css"))
		theme = "../../../Resources/GUIThemeAero";
	else if (CL_FileHelp::file_exists("../../../Resources/GUIThemeBasic/theme.css"))
		theme = "../../../Resources/GUIThemeBasic";
	else
		throw CL_Exception("No themes found");

	gui = CL_GUIManager(wm, theme);

	wm.func_input_intercept().set(this, &GUI::wm_input_intercept);
}

GUI::~GUI()
{
}

bool GUI::run()
{
	wm.process();
	return true;
}

void GUI::draw()
{
	CL_GraphicContext gc = window.get_gc();

	std::vector<CL_GUIWindowManagerTextureWindow> windows = wm.get_windows();
	std::vector<CL_GUIWindowManagerTextureWindow>::size_type index, size;
	size = windows.size();

	for (index = 0; index < size; index++)
	{
		CL_GUIWindowManagerTextureWindow texture_window = windows[index];

		Component3D *component3d = dynamic_cast<Component3D *> (texture_window.get_toplevel_component());
		if (!component3d)
		{
			// Note, it may be possible to handle popup windows at this point
			continue;
		}

		component3d->resultant_matrix = CL_Mat4f::multiply(component3d->object_matrix, gc.get_modelview());
		gc.push_modelview();
		gc.mult_modelview(component3d->object_matrix);

		component3d->resultant_matrix = CL_Mat4f::multiply(gc.get_modelview(), app->get_projection_matrix());

		// Translate 2D coords to 3D coords
		//gc.mult_translate(-1.0f, 1.0f);
		gc.mult_scale(2.0f/gc.get_width(), -2.0f/gc.get_height());

		// Prepare to draw
		CL_Subtexture subtexture = texture_window.get_texture();
		CL_Texture texture = subtexture.get_texture();

		CL_Rect window_geometry = texture_window.get_geometry();
		CL_Rect subtexture_geometry = subtexture.get_geometry();
		CL_Rectf texture_unit1_coords(subtexture_geometry);

		// Note, we ignore the GUI position, and simply center the window
		CL_Size window_size = subtexture_geometry.get_size();
		CL_Rectf rect(-window_size.width/2.0f, -window_size.height/2.0f, CL_Sizef(window_size));

		CL_Vec2f positions[6] =
		{
			CL_Vec2f(rect.left, rect.top),
			CL_Vec2f(rect.right, rect.top),
			CL_Vec2f(rect.left, rect.bottom),
			CL_Vec2f(rect.right, rect.top),
			CL_Vec2f(rect.left, rect.bottom),
			CL_Vec2f(rect.right, rect.bottom)
		};

		float texture_width = (float) texture.get_width();
		float texture_height = (float) texture.get_height();

		float texel_centre = 0.375f;

		float src_left = (texture_unit1_coords.left + texel_centre) / texture_width;
		float src_right = (texture_unit1_coords.right + texel_centre) / texture_width;
		float src_top = (texture_unit1_coords.top + texel_centre) / texture_height;
		float src_bottom = (texture_unit1_coords.bottom + texel_centre) / texture_height;

		CL_Vec2f tex1_coords[6] =
		{
			CL_Vec2f(src_left, src_top),
			CL_Vec2f(src_right, src_top),
			CL_Vec2f(src_left, src_bottom),
			CL_Vec2f(src_right, src_top),
			CL_Vec2f(src_left, src_bottom),
			CL_Vec2f(src_right, src_bottom)
		};

		gc.set_texture(0, texture);
		CL_PrimitivesArray prim_array(gc);
		prim_array.set_attributes(0, positions);
		prim_array.set_attribute(1, component3d->component_color);
		prim_array.set_attributes(2, tex1_coords);
		gc.set_program_object(cl_program_single_texture);
		gc.draw_primitives(cl_triangles, 6, prim_array);
		gc.reset_program_object();
		gc.reset_texture(0);
		gc.pop_modelview();

	}
}


void GUI::wm_input_intercept(CL_InputEvent &input_event, CL_InputState &input_state)
{
	// Get the windows
	std::vector<CL_GUIWindowManagerTextureWindow> windows = wm.get_windows();

	if (windows.empty())	// Exit now if no windows exist
		return;

	CL_GraphicContext gc = window.get_gc();
	float gc_width = (float) gc.get_width();
	float gc_height = (float) gc.get_height();

	// Transform points to range -1.0f to 1.0f
	float mouse_x = ((2.0f * input_event.mouse_pos.x) / gc_width) - 1.0f;
	float mouse_y = ((2.0f * input_event.mouse_pos.y) / gc_height) - 1.0f;
	mouse_y = -mouse_y;

	CL_Point final_point;

	std::vector<CL_GUIWindowManagerTextureWindow>::size_type index, size;
	size = windows.size();
	for (index = 0; index < size; index++)
	{
		CL_GUIComponent *component = windows[index].get_toplevel_component();
		if (!component)
			continue;

		Component3D *component3d = dynamic_cast<Component3D *> (component);
		if (!component3d)
			continue;

		CL_Mat4d resultant_matrix_double = CL_Mat4d(component3d->resultant_matrix);
		CL_Mat4d inverse_matrix = resultant_matrix_double.inverse();

		CL_Vec3f point;
		float mouse_z;
		float mouse_low_z = 0.5f;
		float mouse_high_z = 1.5f;

		for (int max_iterations = 0; max_iterations < 100; ++max_iterations)
		{
			// Find the Z position of the panel at this 2d point
			// Keep bisecting, until we have found the correct Z
			mouse_z = ( mouse_high_z + mouse_low_z ) / 2.0f;
			point = transform_point(CL_Vec3d( mouse_x, mouse_y, mouse_z), inverse_matrix);
			float diff = - point.z;
			if (diff < 0.0f)
				diff = -diff;

			if (diff <= 0.00001)
				break;

			if (0.0f > point.z)
			{
				mouse_low_z = mouse_z;
			}
			else
			{
				mouse_high_z = mouse_z;
			}
		}

		CL_Point window_point;
		window_point.x = (int) ((((point.x)) * gc_width ) / 2.0f);
		window_point.y = (int) ((((-point.y)) * gc_height ) / 2.0f);

		// Adjust the GUI postions (as it is drawn relative to center of the window)
		CL_Rect component_rect = component->get_window_geometry();
		window_point.x += component_rect.left + component_rect.get_width()/2;
		window_point.y += component_rect.top + component_rect.get_height()/2;

		// If the point is within the current panel, exit loop
		if (component_rect.contains(window_point))
		{
			final_point = window_point;
			break;
		}

		// Store final point only for the active window
		if (!index)
		{
			final_point = window_point;
		
		}

	}
	input_event.mouse_pos = final_point;
}

CL_Vec3f GUI::transform_point(const CL_Vec3d &src_point, const CL_Mat4d &matrix)
{
	return matrix.get_transformed_point(src_point);
}


