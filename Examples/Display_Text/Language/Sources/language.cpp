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


#ifdef ENABLE_THIS_IF_YOU_WANT_TO_USE_FRIBIDI
#define DONT_HAVE_FRIBIDI_CONFIG_H 1
#define FRIBIDI_CHARSETS 0
#define FRIBIDI_USE_GLIB 0
#ifdef _M_X64
#define FRIBIDI_SIZEOF_INT 8
#else
#define FRIBIDI_SIZEOF_INT 4
#endif
#define FRIBIDI_NO_DEPRECATED 0
#define FRIBIDI_ENTRY

#pragma comment(lib, "fribidi-static-mt-debug.lib")

#include <fribidi/fribidi.h>
#endif

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

#ifdef ENABLE_THIS_IF_YOU_WANT_TO_USE_FRIBIDI
		text = document_element.get_child_string("ARABIC");

		/* input */
		CL_String16 text_16 = CL_StringHelp::utf8_to_ucs2(text);
		FriBidiChar *fri_str = (FriBidiChar *) text_16.c_str();
		FriBidiStrIndex fri_len = text_16.length();
		FriBidiCharType fri_base_dir = FRIBIDI_TYPE_ON;
		FriBidiCharType *fri_pbase_dir = &fri_base_dir;

		/* output */
		std::vector<wchar_t> output_buffer;
		output_buffer.resize(text_16.length() + 1);
		FriBidiChar *fri_visual_str = &output_buffer[0];
		FriBidiStrIndex *fri_position_L_to_V_list = NULL;
		FriBidiStrIndex *fri_position_V_to_L_list = NULL;
		FriBidiLevel    *fri_embedding_level_list = NULL;
		fribidi_boolean fri_result;
		fri_result = fribidi_log2vis(fri_str,  fri_len, fri_pbase_dir, fri_visual_str, fri_position_L_to_V_list, fri_position_V_to_L_list, fri_embedding_level_list);
		if (fri_result)
		{
			output_buffer[text_16.length()] = 0;
			CL_String new_text = CL_StringHelp::ucs2_to_utf8(&output_buffer[0]);
			font_arabic.draw_text(gc, 10, 230, new_text);
		}
#endif
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


