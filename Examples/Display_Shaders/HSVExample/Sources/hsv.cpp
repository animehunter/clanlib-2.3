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
#include "hsv.h"


HSV::HSV()
: quit(false)
{
}

int HSV::start(const std::vector<CL_String> &args)
{
	CL_DisplayWindow window("ClanLib HSV Example", 1024, 768);
	CL_Slot slot = window.sig_window_close().connect(this, &HSV::on_close);
	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &HSV::on_input_up);

	CL_GraphicContext gc = window.get_gc();
	CL_InputContext ic = window.get_ic();
	CL_ProgramObject program = create_shader_program(gc);
	CL_Texture texture = create_texture(gc);

	CL_Font font(gc, "tahoma", 24);

	unsigned int last_time = CL_System::get_time();

	float hue_offset = 0.0;
	while (!quit)
	{
		unsigned int current_time = CL_System::get_time();
		float time_delta_ms = static_cast<float> (current_time - last_time);
		last_time = current_time;

		if (ic.get_keyboard().get_keycode(CL_KEY_LEFT))
			hue_offset += 0.0005f * time_delta_ms;
		else if (ic.get_keyboard().get_keycode(CL_KEY_RIGHT))
			hue_offset -= 0.0005f * time_delta_ms;
		if (hue_offset < -1.0f)
			hue_offset += 1.0f;
		if (hue_offset > 1.0f)
			hue_offset -= 1.0f;

		render_texture(gc, program, texture, hue_offset);

		font.draw_text(gc, 32, 700, "Use cursor keys left and right");
		window.flip();
		CL_KeepAlive::process(10);
	}

	return 0;
}

void HSV::on_close()
{
	quit = true;
}

void HSV::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
	{
		quit = true;
	}
}

CL_ProgramObject HSV::create_shader_program(CL_GraphicContext &gc)
{
	CL_ProgramObject program = CL_ProgramObject::load(gc, "Resources/vertex.glsl", "Resources/fragment.glsl");
	program.bind_attribute_location(0, "Position");
	program.bind_attribute_location(1, "HueOffset0");
	program.bind_attribute_location(2, "TexCoord0");
	if (!program.link())
		throw CL_Exception("Unable to link program");
	return program;
}

CL_Texture HSV::create_texture(CL_GraphicContext &gc)
{
	return CL_Texture(gc, "../../Game/DiceWar/Resources/lobby_background2.png");
}

void HSV::render_texture(CL_GraphicContext &gc, CL_ProgramObject &program, CL_Texture &texture, float hue_offset)
{
	CL_Rectf rect(0.0f, 0.0f, (float)gc.get_width(), (float)gc.get_height());
	CL_Rectf texture_unit1_coords(0.0f, 0.0f, 1.0f, 1.0f);

	CL_Vec2f positions[6] =
	{
		CL_Vec2f(rect.left, rect.top),
		CL_Vec2f(rect.right, rect.top),
		CL_Vec2f(rect.left, rect.bottom),
		CL_Vec2f(rect.right, rect.top),
		CL_Vec2f(rect.left, rect.bottom),
		CL_Vec2f(rect.right, rect.bottom)
	};

	CL_Vec2f tex1_coords[6] =
	{
		CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.top),
		CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.top),
		CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.bottom),
		CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.top),
		CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.bottom),
		CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.bottom)
	};

	CL_PrimitivesArray primarray(gc);
	primarray.set_attributes(0, positions);
	primarray.set_attribute(1, CL_Vec1f(hue_offset));
	primarray.set_attributes(2, tex1_coords);

	gc.set_program_object(program, cl_program_matrix_modelview_projection);
	gc.draw_primitives(cl_triangles, 6, primarray);
	gc.reset_program_object();
}
