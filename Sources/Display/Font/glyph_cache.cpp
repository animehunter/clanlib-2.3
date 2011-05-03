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
#include "glyph_cache.h"
#include "FontEngine/font_engine.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/Text/string_format.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/Image/pixel_buffer_help.h"
#include "API/Display/2D/color.h"
#include "API/Display/2D/subtexture.h"
#include "API/Display/2D/texture_group.h"
#include "API/Display/Render/texture.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Font/font_system.h"
#include "API/Display/2D/draw.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/utf8_reader.h"
#include "../2D/render_batch2d.h"
#include "../Render/graphic_context_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GlyphCache Construction:

CL_GlyphCache::CL_GlyphCache()
{
	glyph_list.reserve(256);

	// Note, the user can specify a different texture group size using set_texture_group()
	texture_group = CL_TextureGroup(CL_Size(256,256));

	// Set default font metrics
	font_metrics = CL_FontMetrics(
		0,0, 0, 0,0,0,0,0, 0,0,
		false, false, false, false);

	anti_alias = false;
}

CL_GlyphCache::~CL_GlyphCache()
{
	for (int cnt = 0; cnt < glyph_list.size(); cnt++)
		delete glyph_list[cnt];
}

/////////////////////////////////////////////////////////////////////////////
// CL_GlyphCache Attributes:

CL_FontMetrics CL_GlyphCache::get_font_metrics()
{
	return font_metrics;
}

CL_Size CL_GlyphCache::get_text_size(CL_FontEngine *font_engine, CL_GraphicContext &gc, const CL_StringRef &text)
{
	int width = 0;

	CL_UTF8_Reader reader(text);
	while(!reader.is_end())
	{
		unsigned int glyph = reader.get_char();
		reader.next();
		CL_Font_TextureGlyph *gptr = get_glyph(font_engine, gc, glyph);
		if (gptr == NULL) continue;
		width += gptr->increment.x;
	}
	int height;
	if (width == 0)
	{
		height = 0;
	}
	else
	{
		height = font_metrics.get_ascent() + font_metrics.get_descent();
	}

	return (CL_Size(width, height));
}

CL_Font_TextureGlyph *CL_GlyphCache::get_glyph(CL_FontEngine *font_engine, CL_GraphicContext &gc, unsigned int glyph)
{
	std::vector< CL_Font_TextureGlyph * >::size_type size = glyph_list.size();
	for (int cnt=0; cnt<size; cnt++)
	{
		if (glyph_list[cnt]->glyph == glyph)
			return &(*glyph_list[cnt]);
	}

	// If glyph does not exist, create one automatically

	insert_glyph(font_engine, gc, glyph);

	// Search for the glyph again
	size = glyph_list.size();
	for (int cnt=0; cnt<size; cnt++)
	{
		if (glyph_list[cnt]->glyph == glyph)
			return &(*glyph_list[cnt]);
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GlyphCache Operations:

int CL_GlyphCache::get_character_index(CL_FontEngine *font_engine, CL_GraphicContext &gc, const CL_String &text, const CL_Point &point)
{
	int dest_x = 0;
	int dest_y = 0;

	int character_counter = 0;

	CL_FontMetrics fm = get_font_metrics();
	int font_height = fm.get_height();
	int font_ascent = fm.get_ascent();
	int font_external_leading = fm.get_external_leading();

	std::vector<CL_String> lines = CL_StringHelp::split_text(text, "\n", false);
	for (std::vector<CL_String>::size_type i=0; i<lines.size(); i++)
	{
		int xpos = dest_x;
		int ypos = dest_y;

		CL_String &textline = lines[i];
		CL_String::size_type string_length = textline.length();

		// Scan the string

		CL_UTF8_Reader reader(textline);
		while(!reader.is_end())
		{
			unsigned int glyph = reader.get_char();
			CL_String::size_type glyph_pos = reader.get_position();
			reader.next();

			CL_Font_TextureGlyph *gptr = get_glyph(font_engine, gc, glyph);
			if (gptr == NULL) continue;

			CL_Rect position(xpos, ypos - font_ascent, CL_Size(gptr->increment.x, gptr->increment.y + font_height + font_external_leading));
			if (position.contains(point))
			{
				return glyph_pos + character_counter;
			}
		
			xpos += gptr->increment.x;
			ypos += gptr->increment.y;
		}

		dest_y += font_height + font_external_leading;

		character_counter += string_length + 1;		// (Including the '\n')

	}
	return -1;	// Not found
}

void CL_GlyphCache::insert_glyph(CL_GraphicContext &gc, CL_FontPixelBuffer &pb)
{
	// Search for duplicated glyph's, if found silently ignore them
	std::vector< CL_Font_TextureGlyph * >::size_type size = glyph_list.size();
	for (int cnt=0; cnt<size; cnt++)
	{
		if (glyph_list[cnt]->glyph == pb.glyph)
			return ;
	}

	CL_Font_TextureGlyph *font_glyph = new CL_Font_TextureGlyph();
	
	glyph_list.push_back(font_glyph);
	font_glyph->glyph = pb.glyph;
	font_glyph->empty_buffer = pb.empty_buffer;
	font_glyph->offset = pb.offset;
	font_glyph->increment = pb.increment;

	if (!pb.empty_buffer)
	{
		CL_PixelBuffer buffer_with_border = CL_PixelBufferHelp::add_border(pb.buffer, glyph_border_size, pb.buffer_rect);

		font_glyph->empty_buffer = false;
		font_glyph->subtexture = texture_group.add(gc, CL_Size(buffer_with_border.get_width(), buffer_with_border.get_height() ));
		font_glyph->geometry = CL_Rect(font_glyph->subtexture.get_geometry().left + glyph_border_size, font_glyph->subtexture.get_geometry().top + glyph_border_size, pb.buffer_rect.get_size() );

		font_glyph->subtexture.get_texture().set_subimage(font_glyph->subtexture.get_geometry().left, font_glyph->subtexture.get_geometry().top, buffer_with_border, buffer_with_border.get_size());
	}
}

void CL_GlyphCache::insert_glyph(CL_FontEngine *font_engine, CL_GraphicContext &gc, const CL_StringRef &text)
{
	for (CL_String::size_type p = 0; p < text.length(); ++p)
	{
		insert_glyph(font_engine, gc, text[p]);
	}
}

void CL_GlyphCache::insert_glyph(CL_GraphicContext &gc, CL_Font_System_Position &position, CL_PixelBuffer &pixel_buffer)
{
	unsigned int glyph = position.glyph;

	// Search for duplicated glyph's, if found silently ignore them
	std::vector< CL_Font_TextureGlyph * >::size_type size = glyph_list.size();
	for (int cnt=0; cnt<size; cnt++)
	{
		if (glyph_list[cnt]->glyph == glyph)
			return;
	}

	CL_Font_TextureGlyph *font_glyph = new CL_Font_TextureGlyph();
	
	glyph_list.push_back(font_glyph);
	font_glyph->glyph = glyph;
	font_glyph->offset = CL_Point(position.x_offset, position.y_offset);
	font_glyph->increment = CL_Point(position.x_increment, position.y_increment);

	if ( (position.width > 0 ) && (position.height > 0) )
	{
		CL_Rect source_rect(position.x_pos, position.y_pos, position.width + position.x_pos, position.height + position.y_pos);
		CL_PixelBuffer buffer_with_border = CL_PixelBufferHelp::add_border(pixel_buffer, glyph_border_size, source_rect);

		font_glyph->empty_buffer = false;
		font_glyph->subtexture = texture_group.add(gc, CL_Size(buffer_with_border.get_width(), buffer_with_border.get_height() ));
		font_glyph->geometry = CL_Rect(font_glyph->subtexture.get_geometry().left + glyph_border_size, font_glyph->subtexture.get_geometry().top + glyph_border_size, source_rect.get_size() );

		font_glyph->subtexture.get_texture().set_subimage(font_glyph->subtexture.get_geometry().left, font_glyph->subtexture.get_geometry().top, buffer_with_border, buffer_with_border.get_size());
	}
	else
	{
		font_glyph->empty_buffer = true;
	}
}

void CL_GlyphCache::insert_glyph(CL_FontEngine *font_engine, CL_GraphicContext &gc, int glyph)
{
	CL_FontPixelBuffer pb = font_engine->get_font_glyph(glyph, anti_alias, CL_Colorf::white);

	if (pb.glyph)	// Ignore invalid glyphs
	{
		insert_glyph(gc, pb);
	}
}

void CL_GlyphCache::draw_text(CL_FontEngine *font_engine, CL_GraphicContext &gc, float xpos, float ypos, const CL_StringRef &text, const CL_Colorf &color) 
{
	CL_String::size_type string_length = text.length();
	if (string_length==0)
	{
		return;
	}

	CL_RenderBatcherSprite *batcher = gc.impl->current_internal_batcher;

	// Scan the string
	CL_UTF8_Reader reader(text);
	while(!reader.is_end())
	{
		unsigned int glyph = reader.get_char();
		reader.next();

		CL_Font_TextureGlyph *gptr = get_glyph(font_engine, gc, glyph);
		if (gptr == NULL) continue;

		if (!gptr->empty_buffer)
		{
			float xp = xpos + gptr->offset.x;
			float yp = ypos + gptr->offset.y;

			CL_Rectf dest_size(xp, yp, CL_Sizef(gptr->geometry.get_size()));
			batcher->draw_glyph(gc, gptr->geometry, dest_size, color, gptr->subtexture.get_texture());
		}
		xpos += gptr->increment.x;
		ypos += gptr->increment.y;
	}
}

void CL_GlyphCache::set_texture_group(CL_TextureGroup &new_texture_group)
{
	if (new_texture_group.is_null())
	{
		throw CL_Exception("Specified texture group is not valid");
	}

	if (!glyph_list.empty())
	{
		throw CL_Exception("Cannot specify a new texture group after the font has been used");
	}

	texture_group = new_texture_group;
}

void CL_GlyphCache::set_font_metrics(const CL_FontMetrics &metrics)
{
	font_metrics = metrics;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GlyphCache Implementation:

