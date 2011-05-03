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

#include "API/CSSLayout/css_box_properties.h"
#include "css_used_value.h"

class CL_CSSLayoutGraphics;
class CL_CSSResourceCache;
class CL_CSSBoxElement;

class CL_CSSBorderRenderer
{
public:
	CL_CSSBorderRenderer(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resource_cache, CL_CSSBoxElement *element_node);
	void set_border_values(CL_CSSUsedValue border_left, CL_CSSUsedValue border_top, CL_CSSUsedValue border_right, CL_CSSUsedValue border_bottom);
	void set_border_box(CL_Rect border_box);
	void render();

private:
	void draw_area(CL_Image &image, int x, int y, int w, int h, int sx, int sy, int sw, int sh);
	CL_Rect get_border_image_area() const;
	int get_left_grid(const CL_CSSBoxBorderImageWidth &border_image_width, int image_area_width, int auto_width) const;
	int get_right_grid(const CL_CSSBoxBorderImageWidth &border_image_width, int image_area_width, int auto_width) const;
	int get_top_grid(const CL_CSSBoxBorderImageWidth &border_image_width, int image_area_height, int auto_height) const;
	int get_bottom_grid(const CL_CSSBoxBorderImageWidth &border_image_width, int image_area_height, int auto_height) const;
	int get_left_slice_value(const CL_CSSBoxBorderImageSlice &border_image_slice, int image_width) const;
	int get_right_slice_value(const CL_CSSBoxBorderImageSlice &border_image_slice, int image_width) const;
	int get_top_slice_value(const CL_CSSBoxBorderImageSlice &border_image_slice, int image_height) const;
	int get_bottom_slice_value(const CL_CSSBoxBorderImageSlice &border_image_slice, int image_height) const;
	CL_CSSUsedValue get_horizontal_radius(const CL_CSSBoxBorderRadius &border_radius) const;
	CL_CSSUsedValue get_vertical_radius(const CL_CSSBoxBorderRadius &border_radius) const;
	CL_Colorf get_light_color(const CL_CSSBoxBorderColor &border_color) const;
	CL_Colorf get_dark_color(const CL_CSSBoxBorderColor &border_color) const;

	CL_CSSLayoutGraphics *graphics;
	CL_CSSResourceCache *resource_cache;
	CL_CSSBoxElement *element_node;
	CL_Rect border_box;
	CL_CSSUsedValue border_left;
	CL_CSSUsedValue border_top;
	CL_CSSUsedValue border_right;
	CL_CSSUsedValue border_bottom;
};
