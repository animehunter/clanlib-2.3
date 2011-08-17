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
**    Mark Page
*/

#include "precomp.h"
#include "textfade.h"

// The start of the Application
int TextFade::start(const std::vector<CL_String> &args)
{
	quit = false;

	// Set the window
	CL_DisplayWindowDescription desc;
	desc.set_title("ClanLib TextFade Example");
	desc.set_size(CL_Size(640, 480), true);
	desc.set_allow_resize(true);

	CL_DisplayWindow window(desc);

	// Connect the Window close event
	CL_Slot slot_quit = window.sig_window_close().connect(this, &TextFade::on_window_close);

	// Connect a keyboard handler to on_key_up()
	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &TextFade::on_input_up);

	// Get the graphic context
	CL_GraphicContext gc = window.get_gc();

	CL_Image background = CL_Image(gc, "../../Game/DiceWar/Resources/lobby_background2.png");

	CL_FontDescription font_description;
	font_description.set_typeface_name("arial");
	font_description.set_height(48);
	font_description.set_subpixel(false);	// Fading only works with sub pixel off
	CL_Font standard_font(gc, font_description);

	CL_ResourceManager app_resources("../Font/Resources/resources.xml");
	CL_Font_Sprite sprite_font(gc, "ClanFont", &app_resources);

	// Run until someone presses escape
	while (!quit)
	{
		background.draw(gc, gc.get_size());

		draw_text(gc, standard_font, 100, "ABCDEFGHIJKLMNOPQRSTUVWXZ");

		draw_text(gc, sprite_font, 300, "ABCDEFGHIJKLMNOPQRSTUVWXZ");

		window.flip(1);

		// This call processes user input and other events
		CL_KeepAlive::process(0);
	}

	return 0;
}

void TextFade::draw_text(CL_GraphicContext &gc, CL_Font &font, int ypos, const char *text)
{
	// For this example, to keep things simple, we only handle ASCII characters

	int gc_width = gc.get_width();

	CL_Size text_size = font.get_text_size(gc, text);

	int xpos = (gc_width - text_size.width) / 2;

	while(*text)
	{
		char let = *(text++);
		char buffer[2];
		buffer[0] = let;
		buffer[1] = 0;

		int position_off_centre = (gc_width/2) - xpos;
		if (position_off_centre < 0)
			position_off_centre = -position_off_centre;

		float alpha = 1.0f - ( (float) position_off_centre * 2.0f / gc_width );
		if (alpha > 0.0f)
			font.draw_text(gc, xpos, ypos,  buffer, CL_Colorf(0.4f, 0.4f, 1.0f, alpha));
		xpos += font.get_glyph_size(gc, let).width;
	}

}


// A key was pressed
void TextFade::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
	{
		quit = true;
	}
}

// The window was closed
void TextFade::on_window_close()
{
	quit = true;
}


