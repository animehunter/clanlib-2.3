/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "hsv_sprite.h"
#include "hsv_sprite_batch.h"

HSVSprite::HSVSprite(CL_GraphicContext &gc, HSVSpriteBatch *batcher, const CL_String &image_filename)
: batcher(batcher)
{
	CL_PixelBuffer image = CL_ImageProviderFactory::load(image_filename);
	CL_Subtexture subtexture = batcher->alloc_sprite(gc, image.get_size());
	geometry = subtexture.get_geometry();
	texture = subtexture.get_texture();
	texture.set_subimage(geometry.get_top_left(), image, image.get_size());
}

HSVSprite::~HSVSprite()
{
}

void HSVSprite::draw(CL_GraphicContext &gc, int x, int y, float hue_offset)
{
	batcher->draw_sprite(gc, CL_Rect(CL_Point(x, y), geometry.get_size()), geometry, texture, hue_offset);
}
