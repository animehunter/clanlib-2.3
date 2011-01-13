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


typedef struct _BallPosition
{
	float xpos;
	float ypos;
	float xspeed;
	float yspeed;
	bool xdir;
	bool ydir;
} BallPosition;

class Options;

// This is the Application class (That is instantiated by the Program Class)
class App
{
public:
	App();

	int start(const std::vector<CL_String> &args);

private:
	void on_input_up(const CL_InputEvent &key, const CL_InputState &state);
	void on_window_close();
	void setup_balls();
	void move_balls(float time_diff, int num_balls);
	CL_String get_blendequation(CL_BlendEquation equation, const CL_String &source, const CL_String &dest, const CL_String &source_fragment, const CL_String &dest_fragment);
	CL_String get_blendfunc(CL_BlendFunc blendfunc, const CL_String &fragment_colour);
	void draw_equation(CL_GraphicContext &gc, CL_Font &font, Options *options);
	CL_String get_logic_operation(CL_LogicOp logic_operation, const CL_String &source_fragment, const CL_String &dest_fragment);

private:
	bool quit;
	float grid_space;
	float ball_alpha;

	static const int max_balls = 9;
	BallPosition balls[max_balls];

};
