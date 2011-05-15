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
*/

#pragma once

#include "API/SWRender/pixel_buffer_data.h"
#include "API/Core/Math/vec3.h"
#include "API/Core/Math/mat4.h"
#include "API/Core/Signals/slot.h"
#include "API/Core/System/uniqueptr.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/Render/frame_buffer.h"
#include "API/Display/Render/blend_mode.h"
#include "API/Display/2D/color.h"

class CL_PixelPipeline;
class CL_PixelCommand;
class CL_ProgramObject_SWRender;

class CL_PixelCanvas
{
public:
	CL_PixelCanvas(const CL_Size &size);
	~CL_PixelCanvas();

	void resize(const CL_Size &size);

	int get_width() const { return colorbuffer0.size.width; }
	int get_height() const { return colorbuffer0.size.height; }

	void set_clip_rect(const CL_Rect &new_clip_rect);
	void reset_clip_rect();

	void set_blend_function(CL_BlendFunc src, CL_BlendFunc dest, CL_BlendFunc src_alpha, CL_BlendFunc dest_alpha, const CL_Colorf &const_color);

	void set_framebuffer(const CL_FrameBuffer &buffer);
	void reset_framebuffer();

	void clear(const CL_Colorf &color);
	void draw_pixels(const CL_Rect &dest, const CL_PixelBuffer &image, const CL_Rect &src_rect, const CL_Colorf &primary_color);
	void draw_pixels_bicubic(int x, int y, int zoom_number, int zoom_denominator, const CL_PixelBuffer &pixels);
	void queue_command(CL_UniquePtr<CL_PixelCommand> &command);

	void set_sampler(int index, const CL_PixelBuffer &new_sampler);
	void reset_sampler(int index);

	CL_PixelBuffer &to_pixelbuffer();
	CL_PixelPipeline *get_pipeline() const { return pipeline.get(); }

private:
	void modified_framebuffer();

	CL_PixelBuffer primary_colorbuffer0;
	CL_PixelBufferData colorbuffer0;
	bool framebuffer_set;
	CL_FrameBuffer framebuffer;
	CL_Slot slot_framebuffer_modified;

	CL_Rect clip_rect;
	bool cliprect_set;

	CL_BlendFunc cur_blend_src;
	CL_BlendFunc cur_blend_dest;
	CL_BlendFunc cur_blend_src_alpha;
	CL_BlendFunc cur_blend_dest_alpha;
	CL_Colorf cur_blend_color;

	CL_UniquePtr<CL_PixelPipeline> pipeline;
};
