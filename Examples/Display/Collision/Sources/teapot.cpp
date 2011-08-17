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
#include "teapot.h"

Teapot::Teapot()
{
	dest_xpos = 0;
	dest_ypos = 0;
	x_delta = 0.0f;
	y_delta = 0.0f;
	float_xpos = 0.0f;
	float_ypos = 0.0f;
}

void Teapot::create(CL_GraphicContext &gc, CL_ResourceManager &resources)
{
	teapot_sprites = CL_Sprite(gc, "teapot", &resources);
	teapot_sprites.set_frame_delay(0, 100);

	teapot_collisions = CL_CollidableSprite::create_collision_outlines(gc, "teapot", &resources);
}

void Teapot::draw_collision_outline(CL_GraphicContext &gc)
{
	teapot_collisions[teapot_sprites.get_current_frame()].draw( dest_xpos, dest_ypos + gc.get_height()/2, CL_Colorf::limegreen, gc);

}

void Teapot::clone(const Teapot &source)
{
	dest_xpos = source.dest_xpos;
	dest_ypos = source.dest_ypos;
	x_delta = source.x_delta;
	y_delta = source.y_delta;
	float_xpos = source.float_xpos;
	float_ypos = source.float_ypos;

	teapot_sprites.clone(source.teapot_sprites);

	teapot_collisions.resize(source.teapot_collisions.size());
	for (size_t cnt = 0; cnt < source.teapot_collisions.size(); cnt++)
	{
		teapot_collisions[cnt] = source.teapot_collisions[cnt].clone();
	}
}


void Teapot::update(CL_GraphicContext &gc, int elapsed_ms)
{
	teapot_sprites.update(elapsed_ms);

	float_xpos += x_delta * (float) elapsed_ms;
	float_ypos += y_delta * (float) elapsed_ms;

	dest_xpos = (int) float_xpos;
	dest_ypos = (int) float_ypos;

	if (dest_xpos < 0)
	{
		dest_xpos = 0;
		if (x_delta < 0)
			x_delta = -x_delta;
		float_xpos = (float) dest_xpos;
	}

	if (dest_ypos < 0)
	{
		dest_ypos = 0;
		if (y_delta < 0)
			y_delta = -y_delta;
		float_ypos = (float) dest_ypos;
	}

	int limit_x = gc.get_width() - teapot_sprites.get_width();
	int limit_y = gc.get_height()/2 - teapot_sprites.get_height();

	if (dest_xpos >= limit_x)
	{
		dest_xpos = limit_x;
		if (x_delta > 0)
			x_delta = -x_delta;
		float_xpos = (float) dest_xpos;
	}

	if (dest_ypos >= limit_y)
	{
		dest_ypos = limit_y;
		if (y_delta > 0)
			y_delta = -y_delta;
		float_ypos = (float) dest_ypos;
	}

}

void Teapot::draw_teapot(CL_GraphicContext &gc)
{
	teapot_sprites.draw(gc, dest_xpos, dest_ypos);
}

void Teapot::set_frame(int frame_number)
{
	teapot_sprites.set_frame(frame_number);
}

void Teapot::set_position(int xpos, int ypos)
{
	dest_xpos = xpos;
	dest_ypos = ypos;
	float_xpos = (float) xpos;
	float_ypos = (float) ypos;
}

void Teapot::set_scale(float x_scale, float y_scale)
{
	teapot_sprites.set_scale(x_scale, y_scale);
	for (size_t cnt=0; cnt< teapot_collisions.size(); cnt++)
	{
		teapot_collisions[cnt].set_scale(x_scale, y_scale);
	}
	
}

void Teapot::set_movement_delta(float new_x_delta, float new_y_delta)
{
	x_delta = new_x_delta;
	y_delta = new_y_delta;
}
