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
**    Kenneth Gangstoe
*/

#include "Display/precomp.h"
#include "sprite_impl.h"
#include "render_batch2d.h"
#include "API/Display/2D/sprite_description.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Core/Math/cl_math.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/XML/dom_element.h"
#include "Display/Render/graphic_context_impl.h"
#include <list>

/////////////////////////////////////////////////////////////////////////////
// CL_Sprite_Impl construction:

CL_Sprite_Impl::CL_Sprite_Impl() :
	angle(CL_Angle(0.0f, cl_radians)),
	angle_pitch(CL_Angle(0.0f, cl_radians)),
	angle_yaw(CL_Angle(0.0f, cl_radians)),
	base_angle(CL_Angle(0.0f, cl_radians)),
	scale_x(1.0f),
	scale_y(1.0f),
	color(1.0f, 1.0f, 1.0f, 1.0f),
	linear_filter(true),
	translation_hotspot(0,0),
	rotation_hotspot(0,0),
	translation_origin(origin_top_left),
	rotation_origin(origin_center),
	current_frame(0),
	delta_frame(1),
	update_time_ms(0),
	last_time_ms(0),
	id(0),
	finished(false),
	looping(false),
	play_loop(true),
	play_backward(false),
	play_pingpong(false),
	show_on_finish(CL_Sprite::show_blank),
	texture_group(CL_Size(1,1))
{
	for (int i=0; i<6; i++)
		prim_color[i] = CL_Vec4f(color.r,color.g,color.b,color.a);
}

CL_Sprite_Impl::~CL_Sprite_Impl()
{
}

void CL_Sprite_Impl::init(CL_GraphicContext &gc, const CL_StringRef &resource_id, CL_ResourceManager *resources, const CL_ImageImportDescription &import_desc)
{
//	bool pack_texture = resource.get_element().get_attribute("pack_texture", "yes") == "yes";
	CL_Resource resource = resources->get_resource(resource_id);

	// Create sprite from sprite description
	CL_SpriteDescription desc(gc, resource_id, resources, import_desc );
	create_textures(gc, desc);

	// Load base angle
	float work_angle = CL_StringHelp::text_to_float(resource.get_element().get_attribute("base_angle", "0"));
	base_angle= CL_Angle(work_angle, cl_degrees);

	// Load id
	id = CL_StringHelp::text_to_int(resource.get_element().get_attribute("id", "0"));

	// Load play options	
	CL_DomNode cur_node = resource.get_element().get_first_child();
	while (!cur_node.is_null())
	{
		if (!cur_node.is_element())
			continue;

		CL_DomElement cur_element = cur_node.to_element();

		CL_String tag_name = cur_element.get_tag_name();

		// <color red="float" green="float" blue="float" alpha="float" />
		if (tag_name == "color")
		{
			color.r = (float)CL_StringHelp::text_to_float(cur_element.get_attribute("red", "1.0"));
			color.g = (float)CL_StringHelp::text_to_float(cur_element.get_attribute("green", "1.0"));
			color.b = (float)CL_StringHelp::text_to_float(cur_element.get_attribute("blue", "1.0"));
			color.a = (float)CL_StringHelp::text_to_float(cur_element.get_attribute("alpha", "1.0"));
		}
		// <animation speed="integer" loop="[yes,no]" pingpong="[yes,no]" direction="[backward,forward]" on_finish="[blank,last_frame,first_frame]"/>
		else if (tag_name == "animation")
		{
			int delay_ms = CL_StringHelp::text_to_int(cur_element.get_attribute("speed", "60"));

			int frame_count = frames.size();
			for(int i=0; i<frame_count; ++i)
				get_frame(i)->delay_ms = delay_ms;

			play_loop = ((cur_element.get_attribute("loop", "yes")) == "yes");
			play_pingpong = ((cur_element.get_attribute("pingpong", "no")) == "yes");
			play_backward = ((cur_element.get_attribute("direction", "forward")) == "backward");

			CL_String on_finish = cur_element.get_attribute("on_finish", "blank");
			if (on_finish == "first_frame")
				show_on_finish = CL_Sprite::show_first_frame;
			else if(on_finish == "last_frame")
				show_on_finish = CL_Sprite::show_last_frame;
			else
				show_on_finish = CL_Sprite::show_blank;
		}
		// <scale x="float" y="float />
		else if (tag_name == "scale")
		{
			scale_x = CL_StringHelp::text_to_float(cur_element.get_attribute("x", "1.0"));
			scale_y = CL_StringHelp::text_to_float(cur_element.get_attribute("y", "1.0"));
		}
		// <translation origin="string" x="integer" y="integer" />
		else if (tag_name == "translation")
		{
			CL_String hotspot = cur_element.get_attribute("origin", "top_left");
			CL_Origin origin;

			if(hotspot == "center")
				origin = origin_center;
			else if(hotspot == "top_center")
				origin = origin_top_center;
			else if(hotspot == "top_right")
				origin = origin_top_right;
			else if(hotspot == "center_left")
				origin = origin_center_left;
			else if(hotspot == "center_right")
				origin = origin_center_right;
			else if(hotspot == "bottom_left")
				origin = origin_bottom_left;
			else if(hotspot == "bottom_center")
				origin = origin_bottom_center;
			else if(hotspot == "bottom_right")
				origin = origin_bottom_right;
			else
				origin = origin_top_left;

			int xoffset = CL_StringHelp::text_to_int(cur_element.get_attribute("x", "0"));
			int yoffset = CL_StringHelp::text_to_int(cur_element.get_attribute("y", "0"));

			translation_origin = origin;
			translation_hotspot.x = xoffset;
			translation_hotspot.y = yoffset;
		}
		// <rotation origin="string" x="integer" y="integer" />
		else if (tag_name == "rotation")
		{
			CL_String hotspot = cur_element.get_attribute("origin", "center");
			CL_Origin origin;

			if(hotspot == "top_left")
				origin = origin_top_left;
			else if(hotspot == "top_center")
				origin = origin_top_center;
			else if(hotspot == "top_right")
				origin = origin_top_right;
			else if(hotspot == "center_left")
				origin = origin_center_left;
			else if(hotspot == "center_right")
				origin = origin_center_right;
			else if(hotspot == "bottom_left")
				origin = origin_bottom_left;
			else if(hotspot == "bottom_center")
				origin = origin_bottom_center;
			else if(hotspot == "bottom_right")
				origin = origin_bottom_right;
			else
				origin = origin_center;

			int xoffset = CL_StringHelp::text_to_int(cur_element.get_attribute("x", "0"));
			int yoffset = CL_StringHelp::text_to_int(cur_element.get_attribute("y", "0"));

			rotation_origin = origin;
			rotation_hotspot.x = xoffset;
			rotation_hotspot.y = yoffset;
		}
		// <frame nr="integer" speed="integer" x="integer" y="integer" />
		else if (tag_name == "frame")
		{
			int nr = CL_StringHelp::text_to_int(cur_element.get_attribute("nr", "0"));

			int yoffset = CL_StringHelp::text_to_int(cur_element.get_attribute("y", "0"));
			int xoffset = CL_StringHelp::text_to_int(cur_element.get_attribute("x", "0"));

			SpriteFrame *sptr = get_frame(nr);
			if (sptr == NULL)
			{
				throw CL_Exception("Invalid sprite frame index specified");
			}

			if (cur_element.has_attribute("speed")) 
			{
				sptr->delay_ms = CL_StringHelp::text_to_int(cur_element.get_attribute("speed", "60"));
			}

			sptr->offset = CL_Point(xoffset, yoffset);
		}

		cur_node = cur_node.get_next_sibling();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_Sprite_Impl attributes:

const CL_Sprite_Impl::SpriteFrame *CL_Sprite_Impl::get_frame(unsigned int index) const
{
	if(index >= 0 && index < frames.size())
		return &frames[index];
	else
		return NULL;
}

CL_Sprite_Impl::SpriteFrame *CL_Sprite_Impl::get_frame(unsigned int index) 
{
	if(index >= 0 && index < frames.size())
		return &frames[index];
	else
		return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Sprite_Impl operations:

CL_Sprite_Impl &CL_Sprite_Impl::operator =(const CL_Sprite_Impl &copy)
{
	angle = copy.angle;
	angle_pitch = copy.angle_pitch;
	angle_yaw = copy.angle_yaw;
	base_angle = copy.base_angle;
	scale_x = copy.scale_x;
	scale_y = copy.scale_y;
	color = copy.color;
	translation_hotspot = copy.translation_hotspot;
	rotation_hotspot = copy.rotation_hotspot;
	translation_origin = copy.translation_origin;
	rotation_origin = copy.rotation_origin;
	current_frame = copy.current_frame;
	delta_frame = copy.delta_frame;
	update_time_ms = copy.update_time_ms;
	last_time_ms = copy.last_time_ms;
	id = copy.id;
	finished = copy.finished;
	play_loop = copy.play_loop;
	play_backward = copy.play_backward;
	play_pingpong = copy.play_pingpong;
	show_on_finish = copy.show_on_finish;

	frames = copy.frames;

	return *this;
}

void CL_Sprite_Impl::draw(CL_GraphicContext &gc, float x, float y)
{
	SpriteFrame &frame = frames[current_frame];

	CL_Surface_DrawParams2 params2;
	params2.srcX = frame.position.left;
	params2.srcY = frame.position.top;
	params2.srcWidth = frame.position.get_width();
	params2.srcHeight = frame.position.get_height();
	params2.destX = x;
	params2.destY = y;
	params2.destZ = 1.0;
	params2.color = color;
	params2.scale_x = scale_x;
	params2.scale_y = scale_y;
	params2.translate_origin = translation_origin;
	params2.translate_x = translation_hotspot.x + frame.offset.x;
	params2.translate_y = translation_hotspot.y + frame.offset.y;
	params2.rotate_angle = angle - base_angle;
	params2.rotate_pitch = angle_pitch;
	params2.rotate_yaw = angle_yaw;
	params2.rotate_origin = rotation_origin;
	params2.rotate_x = rotation_hotspot.x + frame.offset.x;
	params2.rotate_y = rotation_hotspot.y + frame.offset.y;
	params2.sub_pixel_accuracy = true;
	draw(gc, params2);
}

void CL_Sprite_Impl::draw(CL_GraphicContext &gc, const CL_Rectf &src, const CL_Rectf &dest)
{
	SpriteFrame &frame = frames[current_frame];

	CL_Surface_DrawParams1 params1;

	float texture_width = frame.texture.get_width();
	float texture_height = frame.texture.get_height();

	CL_Rectf new_src(
		(src.left) / texture_width,
		(src.top) / texture_height,
		(src.right) / texture_width,
		(src.bottom) / texture_height
		);

	params1.texture_position[0].x = new_src.left;
	params1.texture_position[1].x = new_src.right;
	params1.texture_position[2].x = new_src.left;
	params1.texture_position[3].x = new_src.right;
	params1.texture_position[0].y = new_src.top;
	params1.texture_position[1].y = new_src.top;
	params1.texture_position[2].y = new_src.bottom;
	params1.texture_position[3].y = new_src.bottom;
	params1.dest_position[0].x = dest.left;
	params1.dest_position[1].x = dest.right;
	params1.dest_position[2].x = dest.left;
	params1.dest_position[3].x = dest.right;
	params1.dest_position[0].y = dest.top;
	params1.dest_position[1].y = dest.top;
	params1.dest_position[2].y = dest.bottom;
	params1.dest_position[3].y = dest.bottom;
	for (int i = 0; i < 4; i++)
	{
		params1.color[i] = color;
	}
	params1.destZ = 1.0;
	draw(gc, params1);
}

void CL_Sprite_Impl::draw(CL_GraphicContext &gc, const CL_Rectf &dest)
{
	SpriteFrame &frame = frames[current_frame];

	CL_Surface_DrawParams2 params2;
	params2.srcX = frame.position.left;
	params2.srcY = frame.position.top;
	params2.srcWidth = frame.position.get_width();
	params2.srcHeight = frame.position.get_height();
	params2.destX = dest.left;
	params2.destY = dest.top;
	params2.destZ = 1.0;
	params2.color = color;
	params2.scale_x = dest.get_width()/float(frame.position.get_width());
	params2.scale_y = dest.get_height()/float(frame.position.get_height());
	params2.translate_origin = translation_origin;
	params2.translate_x = translation_hotspot.x + frame.offset.x;
	params2.translate_y = translation_hotspot.y + frame.offset.y;
	params2.rotate_angle = angle - base_angle;
	params2.rotate_pitch = angle_pitch;
	params2.rotate_yaw = angle_yaw;
	params2.rotate_origin = rotation_origin;
	params2.rotate_x = rotation_hotspot.x + frame.offset.x;
	params2.rotate_y = rotation_hotspot.y + frame.offset.y;
	params2.sub_pixel_accuracy = true;
	draw(gc, params2);
}

void CL_Sprite_Impl::draw(CL_GraphicContext &gc, const CL_Surface_DrawParams1 &params1)
{
	CL_RenderBatcherSprite *batcher = gc.impl->current_internal_batcher;
	batcher->draw_sprite(gc, &params1, frames[current_frame].texture);
}

void CL_Sprite_Impl::draw(CL_GraphicContext &gc, const CL_Surface_DrawParams2 &params2)
{
	CL_Surface_TargetDrawParams1 t_params1;
	CL_Surface_DrawParams1 params1;
	draw_calcs_step1(params2, t_params1);
	draw_calcs_step2(params2, &t_params1, params1);
	draw(gc, params1);
}

void CL_Sprite_Impl::draw_calcs_step1(
	const CL_Surface_DrawParams2 &params2,
	CL_Surface_TargetDrawParams1 &t_params1)
{
	// Find size of surface:
	float size_width  = (float) params2.srcWidth;
	float size_height = (float) params2.srcHeight;

	// Calculate translation hotspot
	CL_Pointf translation_hotspot = calc_hotspot(
		params2.translate_origin,
		(float) params2.translate_x,
		(float) params2.translate_y,
		size_width,
		size_height);

	// Calculate rotation hotspot:
	t_params1.rotation_hotspot = calc_hotspot(
		params2.rotate_origin,
		(float) params2.rotate_x,
		(float) params2.rotate_y,
		size_width,
		size_height);

	if (!params2.sub_pixel_accuracy)
	{
		// Avoid linear smoothing:
		translation_hotspot.x = float(int(translation_hotspot.x));
		translation_hotspot.y = float(int(translation_hotspot.y));
		t_params1.rotation_hotspot.x = float(int(t_params1.rotation_hotspot.x));
		t_params1.rotation_hotspot.y = float(int(t_params1.rotation_hotspot.y));
	}

	// Find top left point of destination rectangle and map rotation hotspot to screen coordinates:
	if (params2.scale_x == 1.0 && params2.scale_y == 1.0)
	{
		t_params1.destWidth = (float)params2.srcWidth;
		t_params1.destHeight = (float)params2.srcHeight;
		t_params1.pixDestX = params2.destX-translation_hotspot.x;
		t_params1.pixDestY = params2.destY-translation_hotspot.y;
		t_params1.rotation_hotspot.x += float(t_params1.pixDestX);
		t_params1.rotation_hotspot.y += float(t_params1.pixDestY);
	}
	else
	{
		t_params1.destWidth = params2.srcWidth * params2.scale_x;
		t_params1.destHeight = params2.srcHeight * params2.scale_y;
		t_params1.pixDestX = params2.destX-translation_hotspot.x * params2.scale_x;
		t_params1.pixDestY = params2.destY-translation_hotspot.y * params2.scale_y;
		t_params1.rotation_hotspot.x = float(t_params1.pixDestX + t_params1.rotation_hotspot.x * params2.scale_x);
		t_params1.rotation_hotspot.y = float(t_params1.pixDestY + t_params1.rotation_hotspot.y * params2.scale_y);
	}
}

void CL_Sprite_Impl::draw_calcs_step2(
	const CL_Surface_DrawParams2 & params2,
	const CL_Surface_TargetDrawParams1 *t_params1,
	CL_Surface_DrawParams1 &params1)
{
	// Calculate unit vectors for rotated surface:
	// (cached for speed reasons)
	static float vect_rotate_x[2] = { 1.0f, 0.0f };
	static float vect_rotate_y[2] = { 0.0f, 1.0f };
	static CL_Angle last_angle(0, cl_radians);

	if (last_angle != params2.rotate_angle)
	{
		if (params2.rotate_angle.to_degrees() == 0.0f)
		{
			vect_rotate_x[0] = 1.0;
			vect_rotate_x[1] = 0.0;
			vect_rotate_y[0] = 0.0;
			vect_rotate_y[1] = 1.0;
		}
		else if (params2.rotate_angle.to_degrees() == 90.0f)
		{
			vect_rotate_x[0] = 0.0;
			vect_rotate_x[1] = 1.0;
			vect_rotate_y[0] = -1.0;
			vect_rotate_y[1] = 0.0;
		}
		else if (params2.rotate_angle.to_degrees() == 180.0f)
		{
			vect_rotate_x[0] = -1.0;
			vect_rotate_x[1] = 0.0;
			vect_rotate_y[0] = 0.0;
			vect_rotate_y[1] = -1.0;
		}
		else if (params2.rotate_angle.to_degrees() == 270.0f)
		{
			vect_rotate_x[0] = 0.0;
			vect_rotate_x[1] = -1.0;
			vect_rotate_y[0] = 1.0;
			vect_rotate_y[1] = 0.0;
		}
		else
		{
			float angle_rad = params2.rotate_angle.to_radians();
			vect_rotate_x[0] = cos(angle_rad);
			vect_rotate_x[1] = sin(angle_rad);
			vect_rotate_y[0] = cos(CL_PI/2+angle_rad);
			vect_rotate_y[1] = sin(CL_PI/2+angle_rad);
		}
	}

	// Calculate final source rectangle points for render:
	const CL_Texture &texture = frames[current_frame].texture;
	float texture_width = texture.get_width();
	float texture_height = texture.get_height();

	params1.texture_position[0].x = (((float) params2.srcX) ) / texture_width;
	params1.texture_position[1].x = (((float) params2.srcX+params2.srcWidth) ) / texture_width;
	params1.texture_position[2].x = (((float) params2.srcX) ) / texture_width;
	params1.texture_position[3].x = (((float) params2.srcX+params2.srcWidth) ) / texture_width;

	params1.texture_position[0].y = (((float) params2.srcY) ) / texture_height;
	params1.texture_position[1].y = (((float) params2.srcY) ) / texture_height;
	params1.texture_position[2].y = (((float) params2.srcY+params2.srcHeight) ) / texture_height;
	params1.texture_position[3].y = (((float) params2.srcY+params2.srcHeight) ) / texture_height;

	// Calculate final destination rectangle points for surface rectangle:
	if (params2.rotate_angle.to_radians() == 0.0f)
	{
		params1.dest_position[0].x = t_params1->pixDestX;
		params1.dest_position[1].x = t_params1->pixDestX+t_params1->destWidth;
		params1.dest_position[2].x = t_params1->pixDestX;
		params1.dest_position[3].x = t_params1->pixDestX+t_params1->destWidth;

		params1.dest_position[0].y = t_params1->pixDestY;
		params1.dest_position[1].y = t_params1->pixDestY;
		params1.dest_position[2].y = t_params1->pixDestY+t_params1->destHeight;
		params1.dest_position[3].y = t_params1->pixDestY+t_params1->destHeight;
	}
	else
	{
		// Roll
		params1.dest_position[0].x = calc_rotate_x(t_params1->pixDestX, t_params1->pixDestY, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[0], vect_rotate_y[0]);
		params1.dest_position[1].x = calc_rotate_x(t_params1->pixDestX+t_params1->destWidth, t_params1->pixDestY, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[0], vect_rotate_y[0]);
		params1.dest_position[2].x = calc_rotate_x(t_params1->pixDestX, t_params1->pixDestY+t_params1->destHeight, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[0], vect_rotate_y[0]);
		params1.dest_position[3].x = calc_rotate_x(t_params1->pixDestX+t_params1->destWidth, t_params1->pixDestY+t_params1->destHeight, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[0], vect_rotate_y[0]);

		params1.dest_position[0].y = calc_rotate_y(t_params1->pixDestX, t_params1->pixDestY, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[1], vect_rotate_y[1]);
		params1.dest_position[1].y = calc_rotate_y(t_params1->pixDestX+t_params1->destWidth, t_params1->pixDestY, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[1], vect_rotate_y[1]);
		params1.dest_position[2].y = calc_rotate_y(t_params1->pixDestX, t_params1->pixDestY+t_params1->destHeight, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[1], vect_rotate_y[1]);
		params1.dest_position[3].y = calc_rotate_y(t_params1->pixDestX+t_params1->destWidth, t_params1->pixDestY+t_params1->destHeight, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[1], vect_rotate_y[1]);
	}

	// Pitch
	if (params2.rotate_pitch.to_radians() != 0.0f)
	{
		float pitch_rad = sin(CL_PI/2 + params2.rotate_pitch.to_radians());
		params1.dest_position[0].y = (params1.dest_position[0].y - t_params1->rotation_hotspot.y) * pitch_rad + t_params1->rotation_hotspot.y;
		params1.dest_position[1].y = (params1.dest_position[1].y - t_params1->rotation_hotspot.y) * pitch_rad + t_params1->rotation_hotspot.y;
		params1.dest_position[2].y = (params1.dest_position[2].y - t_params1->rotation_hotspot.y) * pitch_rad + t_params1->rotation_hotspot.y;
		params1.dest_position[3].y = (params1.dest_position[3].y - t_params1->rotation_hotspot.y) * pitch_rad + t_params1->rotation_hotspot.y;
	}
	// Yaw
	if (params2.rotate_yaw.to_radians() != 0.0f)
	{
		float yaw_rad = cos(params2.rotate_yaw.to_radians());
		params1.dest_position[0].x = (params1.dest_position[0].x - t_params1->rotation_hotspot.x) * yaw_rad + t_params1->rotation_hotspot.x;
		params1.dest_position[1].x = (params1.dest_position[1].x - t_params1->rotation_hotspot.x) * yaw_rad + t_params1->rotation_hotspot.x;
		params1.dest_position[2].x = (params1.dest_position[2].x - t_params1->rotation_hotspot.x) * yaw_rad + t_params1->rotation_hotspot.x;
		params1.dest_position[3].x = (params1.dest_position[3].x - t_params1->rotation_hotspot.x) * yaw_rad + t_params1->rotation_hotspot.x;
	}

	params1.color[0] = params2.color;
	params1.color[1] = params2.color;
	params1.color[2] = params2.color;
	params1.color[3] = params2.color;

	params1.destZ = params2.destZ;
}

int CL_Sprite_Impl::calc_time_elapsed()
{
	// Calculate amount of time since last frame
	unsigned int new_time = CL_System::get_time();
	if(last_time_ms == 0)
		last_time_ms = new_time;

	int delta_time = new_time - last_time_ms;
	last_time_ms = new_time;

	return delta_time;
}

void CL_Sprite_Impl::create_textures(CL_GraphicContext &gc, const CL_SpriteDescription &description)
{
	// Fetch frames
	const std::vector<CL_SpriteDescriptionFrame> &description_frames = description.get_frames();
	std::vector<CL_SpriteDescriptionFrame>::const_iterator it_frames;

	// Calculate estimated texture group size
	if (texture_group.get_texture_sizes().width <=1)
	{
		// *** This algorithm may not work! ***
		int max_width = 1;
		int max_height = 1;
		int num_objects = 0;
		int min_width = 9999999;
		int min_height = min_width;
		int total_width = 1;

		const int allowed_pixel_gap = 4;

		for (it_frames = description_frames.begin(); it_frames != description_frames.end(); ++it_frames)
		{
			CL_SpriteDescriptionFrame description_frame = (*it_frames);
			int width = description_frame.rect.get_width();
			int height = description_frame.rect.get_width();

			if (max_width < width) max_width = width;
			if (max_height < height) max_height = height;

			if (min_width > width) min_width = width;
			if (min_height > height) min_height = height;

			total_width += width + allowed_pixel_gap;

			num_objects++;
		}

		int group_width = total_width;
		int group_height = max_height;

		// Check all will fit into a single line
		const int max_group_size = 512;
		if (group_width > max_group_size)
		{
			group_width = max_group_size;
			group_height *= (total_width + max_group_size - 1) / max_group_size;
		}

		// Ensure power of 2 - width
		if (group_width > 256)
			group_width = 512;
		else if (group_width > 128)
			group_width = 256;
		else if (group_width > 64)
			group_width = 128;
		else if (group_width > 32)
			group_width = 64;
		else group_width = 32;

		// Ensure power of 2 - height
		if (group_height > 256)
			group_height = 512;
		else if (group_height > 128)
			group_height = 256;
		else if (group_height > 64)
			group_height = 128;
		else if (group_height > 32)
			group_height = 64;
		else group_height = 32;

		// Only create group if an object will definately fit into it
		if ( (group_width >= min_width) && (group_height >= min_height) && (num_objects > 1) )
		{
			texture_group = CL_TextureGroup(CL_Size(group_width, group_height));
		}
	}

	for (it_frames = description_frames.begin(); it_frames != description_frames.end(); ++it_frames)
	{
		CL_SpriteDescriptionFrame description_frame = (*it_frames);

		if(description_frame.type == CL_SpriteDescriptionFrame::type_pixelbuffer)
		{
			CL_PixelBuffer image = description_frame.pixelbuffer;
			if (description_frame.rect.get_width() <= texture_group.get_texture_sizes().width &&
				description_frame.rect.get_height() <= texture_group.get_texture_sizes().height)
			{
				CL_Subtexture subtexture = texture_group.add(gc, description_frame.rect.get_size());
				subtexture.get_texture().set_subimage(subtexture.get_geometry().get_top_left(), image, description_frame.rect);
				subtexture.get_texture().set_mag_filter(linear_filter ? cl_filter_linear : cl_filter_nearest);
				subtexture.get_texture().set_min_filter(linear_filter ? cl_filter_linear : cl_filter_nearest);

				SpriteFrame frame;
				frame.position = subtexture.get_geometry();
				frame.texture = subtexture.get_texture();
				frame.delay_ms = 60;
				frame.offset = CL_Point(0, 0);
				frames.push_back(frame);
			}
			else
			{
				int width = description_frame.rect.get_width();
				int height = description_frame.rect.get_height();

				// Note, forced power of 2 textures have clamping issues
				int texture_width = width;
				int texture_height = height;

				CL_Texture texture(gc, texture_width, texture_height);
				texture.set_subimage(CL_Point(0,0), image, description_frame.rect);
				texture.set_mag_filter(linear_filter ? cl_filter_linear : cl_filter_nearest);
				texture.set_min_filter(linear_filter ? cl_filter_linear : cl_filter_nearest);

				SpriteFrame frame;
				frame.position = CL_Rect(0, 0, width, height);
				frame.texture = texture;
				frame.delay_ms = 60;
				frame.offset = CL_Point(0, 0);
				frames.push_back(frame);
			}
		}
		else if(description_frame.type == CL_SpriteDescriptionFrame::type_texture)
		{
			SpriteFrame frame;
			frame.position = description_frame.rect;
			frame.texture = description_frame.texture;
			frame.delay_ms = 60;
			frame.offset = CL_Point(0, 0);
			frames.push_back(frame);
		}
	}
}
