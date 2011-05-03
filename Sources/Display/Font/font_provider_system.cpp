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

#include "Display/precomp.h"
#include "font_provider_system.h"

#ifdef USE_MS_FONT_RENDERER
#include "FontEngine/font_engine_win32.h"
#endif
#ifdef __APPLE__
#include "FontEngine/font_engine_cocoa.h"
#endif
//#else
#include "FontEngine/font_engine_freetype.h"
#include "font_provider_freetype.h"
//#endif

#include "API/Core/IOData/file.h"
#include "API/Core/IOData/virtual_directory.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/IOData/iodevice_memory.h"
#include "API/Core/Text/string_format.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/2D/color.h"
#include "API/Display/2D/subtexture.h"
#include "API/Display/2D/texture_group.h"
#include "API/Display/Render/texture.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/2D/draw.h"
#include "API/Core/Text/string_help.h"

#include "../2D/render_batch2d.h"
#include "../Render/graphic_context_impl.h"

#ifndef WIN32
#include "../X11/font_config.h"
#endif

#ifndef WIN32
std::map<CL_String, CL_String > CL_FontProvider_System::font_register_cache;
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_System Construction:

CL_FontProvider_System::CL_FontProvider_System() : glyph_cache(), font_engine(0)
{
}

CL_FontProvider_System::~CL_FontProvider_System()
{
	free_font();
}

/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_System Attributes:

CL_FontMetrics CL_FontProvider_System::get_font_metrics()
{
	return glyph_cache.get_font_metrics();
}

CL_Font_TextureGlyph *CL_FontProvider_System::get_glyph(CL_GraphicContext &gc, unsigned int glyph)
{
	return glyph_cache.get_glyph(font_engine, gc, glyph);
}

/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_System Operations:

void CL_FontProvider_System::insert_glyph(CL_GraphicContext &gc, CL_Font_System_Position &position, CL_PixelBuffer &pixel_buffer)
{
	glyph_cache.insert_glyph(gc, position, pixel_buffer);
}

void CL_FontProvider_System::insert_glyph(CL_GraphicContext &gc, const CL_StringRef &text)
{
	glyph_cache.insert_glyph(font_engine, gc, text);
}

void CL_FontProvider_System::draw_text(CL_GraphicContext &gc, float xpos, float ypos, const CL_StringRef &text, const CL_Colorf &color)
{
	glyph_cache.draw_text(font_engine, gc, xpos, ypos, text, color);
}

CL_Size CL_FontProvider_System::get_text_size(CL_GraphicContext &gc, const CL_StringRef &text)
{
	return glyph_cache.get_text_size(font_engine, gc, text);
}

void CL_FontProvider_System::set_font_metrics(const CL_FontMetrics &metrics)
{
	glyph_cache.set_font_metrics(metrics);
}

void CL_FontProvider_System::set_texture_group(CL_TextureGroup &new_texture_group)
{
	glyph_cache.set_texture_group(new_texture_group);
}

int CL_FontProvider_System::get_character_index(CL_GraphicContext &gc, const CL_String &text, const CL_Point &point)
{
	return glyph_cache.get_character_index(font_engine, gc, text, point);
}

void CL_FontProvider_System::destroy()
{
	delete this;
}

void CL_FontProvider_System::free_font()
{
	if (font_engine)
	{
		delete(font_engine);
		font_engine = NULL;
	}
}

void CL_FontProvider_System::load_font( CL_GraphicContext &context, const CL_FontDescription &desc)
{
	free_font();

	if (desc.get_anti_alias_set())	// Anti-alias was set
	{
		glyph_cache.anti_alias = desc.get_anti_alias();	// Override the default
	}
	else
	{
		glyph_cache.anti_alias = true;
/*
#ifdef USE_MS_FONT_RENDERER
		glyph_cache.anti_alias = abs(desc.get_height()) >= 16;
#else
		glyph_cache.anti_alias = true;	// Default, anti_alias enabled (may be modified by WIN32 later)
#endif
*/
	}

#ifdef USE_MS_FONT_RENDERER

	font_engine = new CL_FontEngine_Win32(desc);
	glyph_cache.font_metrics = font_engine->get_metrics();

#elif defined(WIN32)

	HFONT handle = CreateFont(
		desc.get_height(), desc.get_average_width(),
		(int) (desc.get_escapement() * 10 + 0.5),
		(int) (desc.get_orientation() * 10 + 0.5),
		desc.get_weight(),
		desc.get_italic() ? TRUE : FALSE,
		desc.get_underline() ? TRUE : FALSE,
		desc.get_strikeout() ? TRUE : FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		(desc.get_fixed_pitch() ? FIXED_PITCH : DEFAULT_PITCH) | FF_DONTCARE,
		CL_StringHelp::utf8_to_ucs2(desc.get_typeface_name()).c_str());
	if (handle == 0)
		throw CL_Exception("CreateFont failed");

	HDC dc = GetDC(0);
	HGDIOBJ old_font = SelectObject(dc, handle);
	DWORD font_file_size = GetFontData(dc, 0, 0, 0, 0);
	CL_DataBuffer font_file(font_file_size);
	DWORD result = GetFontData(dc, 0, 0, font_file.get_data(), font_file.get_size());
	SelectObject(dc, old_font);
	ReleaseDC(0, dc);
	DeleteObject(handle);
	if (result == GDI_ERROR)
		throw CL_Exception("GetFontData failed");

	CL_IODevice_Memory font_iodevice(font_file);
	font_engine = new CL_FontEngine_Freetype(font_iodevice, desc.get_height(), desc.get_average_width());
	glyph_cache.font_metrics = font_engine->get_metrics();
    
#elif defined(__APPLE__)
    
    font_engine = new CL_FontEngine_Cocoa(desc);
    glyph_cache.font_metrics = font_engine->get_metrics();

#else

	CL_FontDescription new_desc = get_registered_font(desc);

	CL_String font_file_path = new_desc.get_typeface_name();

	// Try opening the font file.
	CL_IODevice io_dev;
	try
	{
		io_dev = CL_File(font_file_path, CL_File::open_existing, CL_File::access_read);
	}
	catch(CL_Exception error)
	{
		throw CL_Exception(cl_format("Cannot open font file: \"%1\"", font_file_path));
	}

	float size_height =  new_desc.get_height();

	// Load font from the opened file.
	font_engine = new CL_FontEngine_Freetype(io_dev, size_height, new_desc.get_average_width());
	glyph_cache.font_metrics = font_engine->get_metrics();


#endif
}

void CL_FontProvider_System::register_font(const CL_StringRef &font_filename, const CL_StringRef &font_typeface)
{
#ifdef WIN32
	int fonts_added = AddFontResourceEx(CL_StringHelp::utf8_to_ucs2(font_filename).c_str(), FR_PRIVATE|FR_NOT_ENUM, 0);
	if(fonts_added == 0)
		throw CL_Exception("Unable to register font " + font_filename);
#else
	std::map<CL_String, CL_String >::iterator find_it;
	find_it = font_register_cache.find(font_typeface);
	if (find_it == font_register_cache.end())	// Ensure not already registered
	{
		font_register_cache[font_typeface] = font_filename;
	}
#endif
}


/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_System Implementation:

#ifndef WIN32
CL_FontDescription CL_FontProvider_System::get_registered_font(const CL_FontDescription &desc)
{
	int average_width = desc.get_average_width();
	int height = desc.get_height();

	// Attempt to convert the point sizes (to match WIN32 with FreeType)
	if (average_width==0)	// Unset width
	{
		if (height <0)
		{
			average_width = -height;
			average_width = (average_width * 80) / 96;
		}
		else
		{
			// I do not know why this formula works,  but it seems to obtain the best result
			average_width = ( height * 80 * 80 ) / (96 * 96);
		}
	}

	CL_FontDescription new_desc;
	new_desc.clone(desc);
	new_desc.set_average_width(average_width);
	new_desc.set_height(height);

	// Check for a registered font
	std::map<CL_String, CL_String >::iterator find_it;
	find_it = font_register_cache.find(desc.get_typeface_name());
	if (find_it != font_register_cache.end())	// Found the registered font
	{
		new_desc.set_typeface_name(find_it->second);
	}
	else
	{
#if !defined(__APPLE__)
        // Obtain the best matching font file from fontconfig.
		CL_FontConfig &fc = CL_FontConfig::instance();
		CL_String font_file_path = fc.match_font(new_desc);
		new_desc.set_typeface_name(font_file_path);
#endif
	}
	return new_desc;
}
#endif
