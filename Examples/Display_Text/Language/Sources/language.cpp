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
#include "language.h"

// The start of the Application
int Language::start(const std::vector<CL_String> &args)
{
	quit = false;

	// Set the window
	CL_DisplayWindowDescription desc;
	desc.set_title("ClanLib Language Example");
	desc.set_size(CL_Size(640, 480), true);
	desc.set_allow_resize(true);

	CL_DisplayWindow window(desc);

	// Connect the Window close event
	CL_Slot slot_quit = window.sig_window_close().connect(this, &Language::on_window_close);

	// Connect a keyboard handler to on_key_up()
	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &Language::on_input_up);

	// Get the graphic context
	CL_GraphicContext gc = window.get_gc();

	CL_File file("Resources/test.xml");
	CL_DomDocument document(file);
	CL_DomElement document_element = document.get_document_element();
	if (document_element.is_null())
		throw CL_Exception("Cannot obtain the document element");

	CL_Font font_english(gc, "arial", 32);

	CL_FontDescription desc_chinese;
	desc_chinese.set_typeface_name("simsun");
	desc_chinese.set_height(48);
	desc_chinese.set_charset(CL_FontDescription::charset_chinesebig5);
	CL_Font font_chinese(gc, desc_chinese);

	CL_FontDescription desc_arabic;
	desc_arabic.set_typeface_name("arial");
	desc_arabic.set_height(48);
	desc_arabic.set_charset(CL_FontDescription::charset_arabic);
	CL_Font font_arabic(gc, desc_arabic);

	// Run until someone presses escape
	while (!quit)
	{

		gc.clear(CL_Colorf(0.0f,0.0f,0.2f));
		
		CL_String text;
		
		text = document_element.get_child_string("ENGLISH");
		font_english.draw_text(gc, 10, 30, text);

		text = document_element.get_child_string("CHINESE");
		font_chinese.draw_text(gc, 10, 130, text);

		text = document_element.get_child_string("ARABIC");
		font_arabic.draw_text(gc, 10, 230, text);		// Incorrect, does not use Right to left drawing for complex scripts

		 // This does not work
		/*
		// Reverse the arabic glyphs
		CL_UTF8_Reader reader(text);
		std::vector<char> buffer;
		buffer.reserve(text.length() + 1);
		while(!reader.is_end())	// Go to the end
		{
			reader.next();
		}
		const char *in_text = text.c_str();

		while(true)		// Work backwards to the start
		{
			int glyph_end = reader.get_position();
			if (glyph_end == 0)
				break;

			reader.prev();
			for (int glyph_start = reader.get_position(); glyph_start < glyph_end; glyph_start++)
			{
				buffer.push_back(in_text[glyph_start]);
			}
		}
		buffer.push_back(0);
		CL_String out_text(&buffer[0]);

		font_arabic.draw_text(gc, 10, 330, out_text);
		*/

		window.flip(1);

		CL_KeepAlive::process(0);
	}

	return 0;
}

// A key was pressed
void Language::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
	{
		quit = true;
	}
}

// The window was closed
void Language::on_window_close()
{
	quit = true;
}


