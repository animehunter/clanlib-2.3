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

#include "CSSLayout/precomp.h"
#include "css_border_renderer.h"
#include "css_layout_graphics.h"
#include "../css_resource_cache.h"
#include "../BoxTree/css_box_element.h"

CL_CSSBorderRenderer::CL_CSSBorderRenderer(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resource_cache, CL_CSSBoxElement *element_node)
: graphics(graphics), resource_cache(resource_cache), element_node(element_node), border_left(0), border_top(0), border_right(0), border_bottom(0)
{
}

void CL_CSSBorderRenderer::set_border_values(CL_CSSUsedValue new_border_left, CL_CSSUsedValue new_border_top, CL_CSSUsedValue new_border_right, CL_CSSUsedValue new_border_bottom)
{
	border_left = new_border_left;
	border_top = new_border_top;
	border_right = new_border_right;
	border_bottom = new_border_bottom;
}

void CL_CSSBorderRenderer::set_border_box(CL_Rect new_border_box)
{
	border_box = new_border_box;
}

void CL_CSSBorderRenderer::render()
{
	CL_CSSUsedValue outer_radius_top_left_x = get_horizontal_radius(element_node->computed_properties.border_radius_top_left);
	CL_CSSUsedValue outer_radius_top_left_y = get_vertical_radius(element_node->computed_properties.border_radius_top_left);
	CL_CSSUsedValue outer_radius_top_right_x = get_horizontal_radius(element_node->computed_properties.border_radius_top_right);
	CL_CSSUsedValue outer_radius_top_right_y = get_vertical_radius(element_node->computed_properties.border_radius_top_right);
	CL_CSSUsedValue outer_radius_bottom_left_x = get_horizontal_radius(element_node->computed_properties.border_radius_bottom_left);
	CL_CSSUsedValue outer_radius_bottom_left_y = get_vertical_radius(element_node->computed_properties.border_radius_bottom_left);
	CL_CSSUsedValue outer_radius_bottom_right_x = get_horizontal_radius(element_node->computed_properties.border_radius_bottom_right);
	CL_CSSUsedValue outer_radius_bottom_right_y = get_vertical_radius(element_node->computed_properties.border_radius_bottom_right);

	CL_CSSUsedValue inner_radius_top_left_x = cl_max(0.0f, outer_radius_top_left_x - border_left);
	CL_CSSUsedValue inner_radius_top_left_y = cl_max(0.0f, outer_radius_top_left_y - border_top);
	CL_CSSUsedValue inner_radius_top_right_x = cl_max(0.0f, outer_radius_top_right_x - border_right);
	CL_CSSUsedValue inner_radius_top_right_y = cl_max(0.0f, outer_radius_top_right_y - border_top);
	CL_CSSUsedValue inner_radius_bottom_left_x = cl_max(0.0f, outer_radius_bottom_left_x - border_left);
	CL_CSSUsedValue inner_radius_bottom_left_y = cl_max(0.0f, outer_radius_bottom_left_y - border_bottom);
	CL_CSSUsedValue inner_radius_bottom_right_x = cl_max(0.0f, outer_radius_bottom_right_x - border_right);
	CL_CSSUsedValue inner_radius_bottom_right_y = cl_max(0.0f, outer_radius_bottom_right_y - border_bottom);

	CL_Point center_top_left(border_box.left + cl_used_to_actual(outer_radius_top_left_x), border_box.top + cl_used_to_actual(outer_radius_top_left_y));
	CL_Point center_top_right(border_box.right - cl_used_to_actual(outer_radius_top_right_x), border_box.top + cl_used_to_actual(outer_radius_top_right_y));
	CL_Point center_bottom_left(border_box.left + cl_used_to_actual(outer_radius_bottom_left_x), border_box.bottom - cl_used_to_actual(outer_radius_bottom_left_y));
	CL_Point center_bottom_right(border_box.right - cl_used_to_actual(outer_radius_bottom_right_x), border_box.bottom - cl_used_to_actual(outer_radius_bottom_right_y));

	if (element_node->computed_properties.border_style_top.type == CL_CSSBoxBorderStyle::type_solid)
	{
		graphics->fill(CL_Rect(center_top_left.x, border_box.top, center_top_right.x, border_box.top+border_top), element_node->computed_properties.border_color_top.color);
	}
	else if (element_node->computed_properties.border_style_top.type == CL_CSSBoxBorderStyle::type_dotted)
	{
		graphics->dot_horizontal(CL_Rect(center_top_left.x, border_box.top, center_top_right.x, border_box.top+border_top), element_node->computed_properties.border_color_top.color);
	}
	else if (element_node->computed_properties.border_style_top.type == CL_CSSBoxBorderStyle::type_dashed)
	{
		graphics->dash_horizontal(CL_Rect(center_top_left.x, border_box.top, center_top_right.x, border_box.top+border_top), element_node->computed_properties.border_color_top.color);
	}
	else if (element_node->computed_properties.border_style_top.type == CL_CSSBoxBorderStyle::type_double)
	{
		graphics->fill(CL_Rect(center_top_left.x, border_box.top, center_top_right.x, border_box.top+border_top/3), element_node->computed_properties.border_color_top.color);
		graphics->fill(CL_Rect(center_top_left.x, border_box.top+border_top*2/3, center_top_right.x, border_box.top+border_top), element_node->computed_properties.border_color_top.color);
	}
	else if (element_node->computed_properties.border_style_top.type == CL_CSSBoxBorderStyle::type_groove)
	{
		graphics->fill(CL_Rect(center_top_left.x, border_box.top, center_top_right.x, border_box.top+border_top/2), get_dark_color(element_node->computed_properties.border_color_top));
		graphics->fill(CL_Rect(center_top_left.x, border_box.top+border_top/2, center_top_right.x, border_box.top+border_top), get_light_color(element_node->computed_properties.border_color_top));
	}
	else if (element_node->computed_properties.border_style_top.type == CL_CSSBoxBorderStyle::type_ridge)
	{
		graphics->fill(CL_Rect(center_top_left.x, border_box.top, center_top_right.x, border_box.top+border_top/2), get_light_color(element_node->computed_properties.border_color_top));
		graphics->fill(CL_Rect(center_top_left.x, border_box.top+border_top/2, center_top_right.x, border_box.top+border_top), get_dark_color(element_node->computed_properties.border_color_top));
	}
	else if (element_node->computed_properties.border_style_top.type == CL_CSSBoxBorderStyle::type_inset)
	{
		graphics->fill(CL_Rect(center_top_left.x, border_box.top, center_top_right.x, border_box.top+border_top), get_dark_color(element_node->computed_properties.border_color_top));
	}
	else if (element_node->computed_properties.border_style_top.type == CL_CSSBoxBorderStyle::type_outset)
	{
		graphics->fill(CL_Rect(center_top_left.x, border_box.top, center_top_right.x, border_box.top+border_top), get_light_color(element_node->computed_properties.border_color_top));
	}

	if (element_node->computed_properties.border_style_bottom.type == CL_CSSBoxBorderStyle::type_solid)
	{
		graphics->fill(CL_Rect(center_bottom_left.x, border_box.bottom-border_bottom, center_bottom_right.x, border_box.bottom), element_node->computed_properties.border_color_bottom.color);
	}
	else if (element_node->computed_properties.border_style_bottom.type == CL_CSSBoxBorderStyle::type_dotted)
	{
		graphics->dot_horizontal(CL_Rect(center_bottom_left.x, border_box.bottom-border_bottom, center_bottom_right.x, border_box.bottom), element_node->computed_properties.border_color_bottom.color);
	}
	else if (element_node->computed_properties.border_style_bottom.type == CL_CSSBoxBorderStyle::type_dashed)
	{
		graphics->dash_horizontal(CL_Rect(center_bottom_left.x, border_box.bottom-border_bottom, center_bottom_right.x, border_box.bottom), element_node->computed_properties.border_color_bottom.color);
	}
	else if (element_node->computed_properties.border_style_bottom.type == CL_CSSBoxBorderStyle::type_double)
	{
		graphics->fill(CL_Rect(center_bottom_left.x, border_box.bottom-border_bottom, center_bottom_right.x, border_box.bottom-border_bottom*2/3), element_node->computed_properties.border_color_bottom.color);
		graphics->fill(CL_Rect(center_bottom_left.x, border_box.bottom-border_bottom*1/3, center_bottom_right.x, border_box.bottom), element_node->computed_properties.border_color_bottom.color);
	}
	else if (element_node->computed_properties.border_style_bottom.type == CL_CSSBoxBorderStyle::type_groove)
	{
		graphics->fill(CL_Rect(center_bottom_left.x, border_box.bottom-border_bottom, center_bottom_right.x, border_box.bottom-border_bottom/2), get_dark_color(element_node->computed_properties.border_color_bottom));
		graphics->fill(CL_Rect(center_bottom_left.x, border_box.bottom-border_bottom/2, center_bottom_right.x, border_box.bottom), get_light_color(element_node->computed_properties.border_color_bottom));
	}
	else if (element_node->computed_properties.border_style_bottom.type == CL_CSSBoxBorderStyle::type_ridge)
	{
		graphics->fill(CL_Rect(center_bottom_left.x, border_box.bottom-border_bottom, center_bottom_right.x, border_box.bottom-border_bottom/2), get_light_color(element_node->computed_properties.border_color_bottom));
		graphics->fill(CL_Rect(center_bottom_left.x, border_box.bottom-border_bottom/2, center_bottom_right.x, border_box.bottom), get_dark_color(element_node->computed_properties.border_color_bottom));
	}
	else if (element_node->computed_properties.border_style_bottom.type == CL_CSSBoxBorderStyle::type_inset)
	{
		graphics->fill(CL_Rect(center_bottom_left.x, border_box.bottom-border_bottom, center_bottom_right.x, border_box.bottom), get_light_color(element_node->computed_properties.border_color_bottom));
	}
	else if (element_node->computed_properties.border_style_bottom.type == CL_CSSBoxBorderStyle::type_outset)
	{
		graphics->fill(CL_Rect(center_bottom_left.x, border_box.bottom-border_bottom, center_bottom_right.x, border_box.bottom), get_dark_color(element_node->computed_properties.border_color_bottom));
	}

	if (element_node->computed_properties.border_style_left.type == CL_CSSBoxBorderStyle::type_solid)
	{
		graphics->fill(CL_Rect(border_box.left, center_top_left.y, border_box.left+border_left, center_bottom_left.y), element_node->computed_properties.border_color_left.color);
	}
	else if (element_node->computed_properties.border_style_left.type == CL_CSSBoxBorderStyle::type_dotted)
	{
		graphics->dot_vertical(CL_Rect(border_box.left, center_top_left.y, border_box.left+border_left, center_bottom_left.y), element_node->computed_properties.border_color_left.color);
	}
	else if (element_node->computed_properties.border_style_left.type == CL_CSSBoxBorderStyle::type_dashed)
	{
		graphics->dash_vertical(CL_Rect(border_box.left, center_top_left.y, border_box.left+border_left, center_bottom_left.y), element_node->computed_properties.border_color_left.color);
	}
	else if (element_node->computed_properties.border_style_left.type == CL_CSSBoxBorderStyle::type_double)
	{
		graphics->fill(CL_Rect(border_box.left, center_top_left.y, border_box.left+border_left/3, center_bottom_left.y), element_node->computed_properties.border_color_left.color);
		graphics->fill(CL_Rect(border_box.left+border_left*2/3, center_top_left.y, border_box.left+border_left, center_bottom_left.y), element_node->computed_properties.border_color_left.color);
	}
	else if (element_node->computed_properties.border_style_left.type == CL_CSSBoxBorderStyle::type_groove)
	{
		graphics->fill(CL_Rect(border_box.left, center_top_left.y, border_box.left+border_left/2, center_bottom_left.y), get_dark_color(element_node->computed_properties.border_color_left));
		graphics->fill(CL_Rect(border_box.left+border_left/2, center_top_left.y, border_box.left+border_left, center_bottom_left.y), get_light_color(element_node->computed_properties.border_color_left));
	}
	else if (element_node->computed_properties.border_style_left.type == CL_CSSBoxBorderStyle::type_ridge)
	{
		graphics->fill(CL_Rect(border_box.left, center_top_left.y, border_box.left+border_left/2, center_bottom_left.y), get_light_color(element_node->computed_properties.border_color_left));
		graphics->fill(CL_Rect(border_box.left+border_left/2, center_top_left.y, border_box.left+border_left, center_bottom_left.y), get_dark_color(element_node->computed_properties.border_color_left));
	}
	else if (element_node->computed_properties.border_style_left.type == CL_CSSBoxBorderStyle::type_inset)
	{
		graphics->fill(CL_Rect(border_box.left, center_top_left.y, border_box.left+border_left, center_bottom_left.y), get_dark_color(element_node->computed_properties.border_color_left));
	}
	else if (element_node->computed_properties.border_style_left.type == CL_CSSBoxBorderStyle::type_outset)
	{
		graphics->fill(CL_Rect(border_box.left, center_top_left.y, border_box.left+border_left, center_bottom_left.y), get_light_color(element_node->computed_properties.border_color_left));
	}

	if (element_node->computed_properties.border_style_right.type == CL_CSSBoxBorderStyle::type_solid)
	{
		graphics->fill(CL_Rect(border_box.right-border_right, center_top_right.y, border_box.right, center_bottom_right.y), element_node->computed_properties.border_color_right.color);
	}
	else if (element_node->computed_properties.border_style_right.type == CL_CSSBoxBorderStyle::type_dotted)
	{
		graphics->dot_vertical(CL_Rect(border_box.right-border_right, center_top_right.y, border_box.right, center_bottom_right.y), element_node->computed_properties.border_color_right.color);
	}
	else if (element_node->computed_properties.border_style_right.type == CL_CSSBoxBorderStyle::type_dashed)
	{
		graphics->dash_vertical(CL_Rect(border_box.right-border_right, center_top_right.y, border_box.right, center_bottom_right.y), element_node->computed_properties.border_color_right.color);
	}
	else if (element_node->computed_properties.border_style_right.type == CL_CSSBoxBorderStyle::type_double)
	{
		graphics->fill(CL_Rect(border_box.right-border_right, center_top_right.y, border_box.right-border_right*2/3, center_bottom_right.y), element_node->computed_properties.border_color_right.color);
		graphics->fill(CL_Rect(border_box.right-border_right/3, center_top_right.y, border_box.right, center_bottom_right.y), element_node->computed_properties.border_color_right.color);
	}
	else if (element_node->computed_properties.border_style_right.type == CL_CSSBoxBorderStyle::type_groove)
	{
		graphics->fill(CL_Rect(border_box.right-border_right, center_top_right.y, border_box.right-border_right/2, center_bottom_right.y), get_dark_color(element_node->computed_properties.border_color_right));
		graphics->fill(CL_Rect(border_box.right-border_right/2, center_top_right.y, border_box.right, center_bottom_right.y), get_light_color(element_node->computed_properties.border_color_right));
	}
	else if (element_node->computed_properties.border_style_right.type == CL_CSSBoxBorderStyle::type_ridge)
	{
		graphics->fill(CL_Rect(border_box.right-border_right, center_top_right.y, border_box.right-border_right/2, center_bottom_right.y), get_light_color(element_node->computed_properties.border_color_right));
		graphics->fill(CL_Rect(border_box.right-border_right/2, center_top_right.y, border_box.right, center_bottom_right.y), get_dark_color(element_node->computed_properties.border_color_right));
	}
	else if (element_node->computed_properties.border_style_right.type == CL_CSSBoxBorderStyle::type_inset)
	{
		graphics->fill(CL_Rect(border_box.right-border_right, center_top_right.y, border_box.right, center_bottom_right.y), get_light_color(element_node->computed_properties.border_color_right));
	}
	else if (element_node->computed_properties.border_style_right.type == CL_CSSBoxBorderStyle::type_outset)
	{
		graphics->fill(CL_Rect(border_box.right-border_right, center_top_right.y, border_box.right, center_bottom_right.y), get_dark_color(element_node->computed_properties.border_color_right));
	}
}

CL_CSSUsedValue CL_CSSBorderRenderer::get_horizontal_radius(const CL_CSSBoxBorderRadius &border_radius) const
{
	if (border_radius.value_type1 == CL_CSSBoxBorderRadius::value_type_length)
		return border_radius.length1.value;
	else if (border_radius.value_type1 == CL_CSSBoxBorderRadius::value_type_percentage)
		return border_radius.percentage1 * border_box.get_width() / 100.0f;
	else
		return 0.0f;
}

CL_CSSUsedValue CL_CSSBorderRenderer::get_vertical_radius(const CL_CSSBoxBorderRadius &border_radius) const
{
	if (border_radius.value_type2 == CL_CSSBoxBorderRadius::value_type_length)
		return border_radius.length2.value;
	else if (border_radius.value_type2 == CL_CSSBoxBorderRadius::value_type_percentage)
		return border_radius.percentage2 * border_box.get_height() / 100.0f;
	else
		return 0.0f;
}

CL_Colorf CL_CSSBorderRenderer::get_light_color(const CL_CSSBoxBorderColor &border_color) const
{
	CL_Colorf light = border_color.color;
	light.r = cl_min(1.0f, light.r * 1.2f);
	light.g = cl_min(1.0f, light.g * 1.2f);
	light.b = cl_min(1.0f, light.b * 1.2f);
	return light;
}

CL_Colorf CL_CSSBorderRenderer::get_dark_color(const CL_CSSBoxBorderColor &border_color) const
{
	CL_Colorf dark = border_color.color;
	dark.r *= 0.8f;
	dark.g *= 0.8f;
	dark.b *= 0.8f;
	return dark;
}
