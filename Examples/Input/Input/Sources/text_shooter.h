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

class TextShooter
{
public:
	TextShooter(CL_Font_Vector &new_vector_font, const CL_String &new_text);

	void set_start_position(const CL_Vec3f &new_start_position);
	void set_end_position(const CL_Vec3f &new_end_position);
	void set_duration(int new_duration);
	void set_start_time(int new_start_time);
	void set_end_fade_time(int new_end_fade_time);
	void set_initial_white_time(int new_initial_white_time);
	void set_color_component(bool new_red, bool new_green, bool new_blue);

	// Returns false when the text has reached the destination position
	bool draw(CL_GraphicContext &gc, unsigned int current_time);

private:
	CL_Font_Vector vector_font;
	CL_String text;

	CL_Vec3f start_position;
	CL_Vec3f end_position;
	int duration;
	unsigned int start_time;

	float end_fade_time;
	float initial_white_time;

	bool use_red_component;
	bool use_green_component;
	bool use_blue_component;
};
