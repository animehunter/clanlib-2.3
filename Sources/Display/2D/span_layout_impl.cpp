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

#include "Display/precomp.h"
#include "API/Core/Math/cl_math.h"
#include "API/Display/2D/draw.h"
#include "span_layout_impl.h"

CL_SpanLayout_Impl::CL_SpanLayout_Impl()
: cursor_visible(false), cursor_pos(0), cursor_overwrite_mode(false), cursor_color(CL_Colorf::black),
  sel_start(0), sel_end(0), sel_foreground(CL_Colorf::white), sel_background(CL_Colorf::darkslateblue),
  alignment(cl_left), is_ellipsis_draw(false)
{
}

CL_SpanLayout_Impl::~CL_SpanLayout_Impl()
{
	clear();
}

void CL_SpanLayout_Impl::clear()
{
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i].type == object_component)
			delete objects[i].component;
	}

	objects.clear();
	text.clear();
	lines.clear();
}

std::vector<CL_Rect> CL_SpanLayout_Impl::get_rect_by_id(int id) const
{
	std::vector<CL_Rect> segment_rects;

	int x = position.x;
	int y = position.y;
	for (std::vector<Line>::size_type line_index = 0; line_index < lines.size(); line_index++)
	{
		const Line &line = lines[line_index];
		for (std::vector<LineSegment>::size_type segment_index = 0; segment_index < line.segments.size(); segment_index++)
		{
			const LineSegment &segment = line.segments[segment_index];
			if (segment.id == id)
			{
				segment_rects.push_back(CL_Rect(x + segment.x_position, y, segment.width, y+line.height));
			}
		}
		y += line.height;
	}

	return segment_rects;
}

void CL_SpanLayout_Impl::draw_layout(CL_GraphicContext &gc)
{
	int x = position.x;
	int y = position.y;
	for (std::vector<Line>::size_type line_index = 0; line_index < lines.size(); line_index++)
	{
		Line &line = lines[line_index];
		for (std::vector<LineSegment>::size_type segment_index = 0; segment_index < line.segments.size(); segment_index++)
		{
			LineSegment &segment = line.segments[segment_index];
			switch(segment.type)
			{
			case object_text:
				draw_layout_text(gc, line, segment, x, y);
				break;
			case object_image:
				draw_layout_image(gc, line, segment, x, y);
				break;
			case object_component:
				break;
			}

		}

		if (line_index + 1 == lines.size() && !line.segments.empty())
		{
			LineSegment &segment = line.segments.back();
			if (cursor_visible && segment.end <= cursor_pos)
			{
				switch(segment.type)
				{
				case object_text:
					{
						int cursor_x = x + segment.x_position + segment.font.get_text_size(gc, text.substr(segment.start, segment.end - segment.start)).width;
						int cursor_width = 1;
						CL_Draw::fill(gc, cursor_x, y + line.ascender-segment.ascender, cursor_x + cursor_width, y+line.ascender+segment.descender, cursor_color);
					}
					break;
				}
			}
		}

		y += line.height;
	}
}

void CL_SpanLayout_Impl::draw_layout_ellipsis(CL_GraphicContext &gc, const CL_Rect &content_rect)
{
	is_ellipsis_draw = true;
	ellipsis_content_rect = content_rect;
	try
	{
		is_ellipsis_draw = false;
		draw_layout(gc);
	}
	catch (...)
	{
		is_ellipsis_draw = false;
		throw;
	}
}

void CL_SpanLayout_Impl::draw_layout_image(CL_GraphicContext &gc, Line &line, LineSegment &segment, int x, int y)
{
	segment.image.draw(gc, x+segment.x_position, y + line.ascender - segment.ascender);
}

void CL_SpanLayout_Impl::draw_layout_text(CL_GraphicContext &gc, Line &line, LineSegment &segment, int x, int y)
{
	CL_StringRef segment_text = text.substr(segment.start, segment.end - segment.start);

	int length = (int)segment_text.length();
	int s1 = cl_clamp((int)sel_start - (int)segment.start, 0, length);
	int s2 = cl_clamp((int)sel_end - (int)segment.start, 0, length);

	if (s1 != s2)
	{
		int xx = x + segment.x_position;
		int xx0 = xx + segment.font.get_text_size(gc, segment_text.substr(0, s1)).width;
		int xx1 = xx0 + segment.font.get_text_size(gc, segment_text.substr(s1, s2 - s1)).width;
		int sel_width = segment.font.get_text_size(gc, segment_text.substr(s1, s2 - s1)).width;

		CL_Draw::fill(gc, xx0, y + line.ascender-segment.ascender, xx1, y+line.ascender+segment.descender, sel_background);

		if (cursor_visible && cursor_pos >= segment.start && cursor_pos < segment.end)
		{
			int cursor_x = x + segment.x_position + segment.font.get_text_size(gc, text.substr(segment.start, cursor_pos - segment.start)).width;
			int cursor_width = cursor_overwrite_mode ? segment.font.get_text_size(gc, text.substr(cursor_pos, 1)).width : 1;
			CL_Draw::fill(gc, cursor_x, y + line.ascender-segment.ascender, cursor_x + cursor_width, y+line.ascender+segment.descender, cursor_color);
		}

		if (s1 > 0)
		{
			if (is_ellipsis_draw)
				segment.font.draw_text_ellipsis(gc, xx, y + line.ascender, ellipsis_content_rect, segment_text.substr(0, s1), segment.color);
			else
				segment.font.draw_text(gc, xx, y + line.ascender, segment_text.substr(0, s1), segment.color);
		}
		if (is_ellipsis_draw)
			segment.font.draw_text_ellipsis(gc, xx0, y+line.ascender, ellipsis_content_rect, segment_text.substr(s1, s2 - s1), sel_foreground);
		else
			segment.font.draw_text(gc, xx0, y+line.ascender, segment_text.substr(s1, s2 - s1), sel_foreground);
		xx += sel_width;
		if (s2 < length)
		{
			if (is_ellipsis_draw)
				segment.font.draw_text_ellipsis(gc, xx1, y + line.ascender, ellipsis_content_rect, segment_text.substr(s2), segment.color);
			else
				segment.font.draw_text(gc, xx1, y + line.ascender, segment_text.substr(s2), segment.color);
		}
	}
	else
	{
		if (cursor_visible && cursor_pos >= segment.start && cursor_pos < segment.end)
		{
			int cursor_x = x + segment.x_position + segment.font.get_text_size(gc, text.substr(segment.start, cursor_pos - segment.start)).width;
			int cursor_width = cursor_overwrite_mode ? segment.font.get_text_size(gc, text.substr(cursor_pos, 1)).width : 1;
			CL_Draw::fill(gc, cursor_x, y + line.ascender-segment.ascender, cursor_x + cursor_width, y+line.ascender+segment.descender, cursor_color);
		}

		if (is_ellipsis_draw)
			segment.font.draw_text_ellipsis(gc, x + segment.x_position, y + line.ascender, ellipsis_content_rect, segment_text, segment.color);
		else
			segment.font.draw_text(gc, x + segment.x_position, y + line.ascender, segment_text, segment.color);
	}
}

CL_SpanLayout::HitTestResult CL_SpanLayout_Impl::hit_test(CL_GraphicContext &gc, const CL_Point &pos)
{
	CL_SpanLayout::HitTestResult result;

	if(lines.empty())
	{
		result.type = CL_SpanLayout::HitTestResult::no_objects_available;
		return result;
	}

	int x = position.x;
	int y = position.y;

	// Check if we are outside to the top
	if(pos.y < y)
	{
		result.type = CL_SpanLayout::HitTestResult::outside_top;
		result.object_id = lines[0].segments[0].id;
		result.offset = 0;
		return result;
	}

	for (std::vector<Line>::size_type line_index = 0; line_index < lines.size(); line_index++)
	{
		Line &line = lines[line_index];

		// Check if we found current line
		if(pos.y >= y && pos.y <= y + line.height)
		{
			for (std::vector<LineSegment>::size_type segment_index = 0; segment_index < line.segments.size(); segment_index++)
			{
				LineSegment &segment = line.segments[segment_index];

				// Check if we are outside to the left
				if(segment_index == 0 && pos.x < x)
				{
					result.type = CL_SpanLayout::HitTestResult::outside_left;
					result.object_id = segment.id;
					result.offset = segment.start;
					return result;
				}

				// Check if we are inside a segment
				if(pos.x >= x + segment.x_position && pos.x <= x + segment.x_position + segment.width)
				{
					CL_StringRef segment_text = text.substr(segment.start, segment.end-segment.start);
					CL_Point hit_point(pos.x - x - segment.x_position, 0);
					int offset = segment.start + segment.font.get_character_index(gc, segment_text, hit_point);

					result.type = CL_SpanLayout::HitTestResult::inside;
					result.object_id = segment.id;
					result.offset = offset;
					return result;
				}

				// Check if we are outside to the right
				if(segment_index == line.segments.size() - 1 && pos.x > x + segment.x_position + segment.width)
				{
					result.type = CL_SpanLayout::HitTestResult::outside_right;
					result.object_id = segment.id;
					result.offset = segment.end;
					return result;
				}
			}
		}

		y += line.height;
	}

	// We are outside to the bottom
	const Line &last_line = lines[lines.size() - 1];
	const LineSegment &last_segment = last_line.segments[last_line.segments.size() - 1];

	result.type = CL_SpanLayout::HitTestResult::outside_bottom;
	result.object_id = last_segment.id;
	result.offset = last_segment.end;
	return result;
}

CL_Size CL_SpanLayout_Impl::get_size() const
{
	CL_Size size(0,0);
	int x = 0;
	int y = 0;
	for (std::vector<Line>::size_type line_index = 0; line_index < lines.size(); line_index++)
	{
		const Line &line = lines[line_index];
		for (std::vector<LineSegment>::size_type segment_index = 0; segment_index < line.segments.size(); segment_index++)
		{
			const LineSegment &segment = line.segments[segment_index];
			CL_Rect area(CL_Point(x + segment.x_position, y - line.height), CL_Size(segment.width, line.height));
			size.width = cl_max(size.width, area.right);
			size.height = cl_max(size.height, area.bottom);
		}
		y += line.height;
	}
	size.height = cl_max(size.height, y);
	return size;
}

void CL_SpanLayout_Impl::add_text(const CL_String &more_text, const CL_Font &font, const CL_Colorf &color, int id)
{
	SpanObject object;
	object.type = object_text;
	object.start = text.length();
	object.end = object.start + more_text.length();
	object.font = font;
	object.color = color;
	object.id = id;
	objects.push_back(object);
	text += more_text;
}

void CL_SpanLayout_Impl::add_image(const CL_Image &image, int baseline_offset, int id)
{
	SpanObject object;
	object.type = object_image;
	object.image = image;
	object.baseline_offset = baseline_offset;
	object.id = id;
	object.start = text.length();
	object.end = object.start + 1;
	objects.push_back(object);
	text += "*";
}

void CL_SpanLayout_Impl::add_component(CL_SpanComponent *component, int baseline_offset, int id)
{
	SpanObject object;
	object.type = object_component;
	object.component = component;
	object.baseline_offset = baseline_offset;
	object.id = id;
	object.start = text.length();
	object.end = object.start + 1;
	objects.push_back(object);
	text += "*";
}

void CL_SpanLayout_Impl::layout(CL_GraphicContext &gc, int max_width)
{
	layout_lines(gc, max_width);

	switch (alignment)
	{
	case cl_right: align_right(max_width); break;
	case cl_center: align_center(max_width); break;
	case cl_justify: align_justify(max_width); break;
	case cl_left:
	default: break;
	}
}

CL_SpanLayout_Impl::TextSizeResult CL_SpanLayout_Impl::find_text_size(CL_GraphicContext &gc, const TextBlock &block, unsigned int object_index)
{
	CL_Font font = objects[object_index].font;
	if (layout_cache.object_index != object_index)
	{
		layout_cache.object_index = object_index;
		layout_cache.metrics = font.get_font_metrics();
	}

	TextSizeResult result;
	result.start = block.start;
	int pos = block.start;
	int x_position = 0;
	while (pos != block.end)
	{
		int end = cl_min(objects[object_index].end, block.end);
		CL_StringRef subtext = text.substr(pos, end-pos);

		CL_Size text_size = font.get_text_size(gc, subtext);

		result.width += text_size.width;
		result.height = cl_max(result.height, (int)(layout_cache.metrics.get_height()+layout_cache.metrics.get_external_leading())/*text_size.height*/);
		result.ascender = cl_max(result.ascender, (int)layout_cache.metrics.get_ascent());
		result.descender = cl_max(result.descender, (int)layout_cache.metrics.get_descent());

		LineSegment segment;
		segment.type = object_text;
		segment.start = pos;
		segment.end = end;
		segment.font = objects[object_index].font;
		segment.color = objects[object_index].color;
		segment.id = objects[object_index].id;
		segment.x_position = x_position;
		segment.width = text_size.width;
		segment.ascender = (int)layout_cache.metrics.get_ascent();
		segment.descender = (int)layout_cache.metrics.get_descent();
		x_position += text_size.width;
		result.segments.push_back(segment);

		pos = end;
		if (pos == objects[object_index].end)
		{
			object_index++;
			result.objects_traversed++;

			if (object_index < objects.size())
			{
				layout_cache.object_index = object_index;
				font = objects[object_index].font;
				layout_cache.metrics = font.get_font_metrics();
			}
		}
	}
	result.end = pos;
	return result;
}

std::vector<CL_SpanLayout_Impl::TextBlock> CL_SpanLayout_Impl::find_text_blocks()
{
	std::vector<TextBlock> blocks;
	std::vector<SpanObject>::iterator block_object_it;

	// Find first object that is not text:
	for (block_object_it = objects.begin(); block_object_it != objects.end() && (*block_object_it).type == object_text; ++block_object_it);

	CL_String::size_type pos = 0;
	while (pos < text.size())
	{
		// Find end of text block:
		CL_String::size_type end_pos;
		switch(text[pos])
		{
			case ' ':
			case '\t':
			case '\n':
				end_pos = text.find_first_not_of(text[pos], pos);
				break;
			default:
				end_pos = text.find_first_of(" \t\n", pos);
				break;
		}

		if (end_pos == CL_String::npos)
			end_pos = text.length();

		// If we traversed past an object that is not text:
		if (block_object_it != objects.end() && (*block_object_it).start < end_pos)
		{
			// End text block
			end_pos = (*block_object_it).start;
			if (end_pos > pos)
			{
				TextBlock block;
				block.start = pos;
				block.end = end_pos;
				blocks.push_back(block);
			}

			// Create object block:
			pos = end_pos;
			end_pos = pos+1;

			TextBlock block;
			block.start = pos;
			block.end = end_pos;
			blocks.push_back(block);

			// Find next object that is not text:
			for (++block_object_it; block_object_it != objects.end() && (*block_object_it).type == object_text; ++block_object_it);
		}
		else
		{
			if (end_pos > pos)
			{
				TextBlock block;
				block.start = pos;
				block.end = end_pos;
				blocks.push_back(block);
			}
		}

		pos = end_pos;
	}

	return blocks;
}

void CL_SpanLayout_Impl::set_align(CL_SpanAlign align)
{
	alignment = align;
}

void CL_SpanLayout_Impl::layout_lines(CL_GraphicContext & gc, int max_width)
{
	lines.clear();
	if (objects.empty())
		return;

	layout_cache.metrics = -1;
	layout_cache.object_index = -1;

	CurrentLine current_line;
	std::vector<TextBlock> blocks = find_text_blocks();
	for (std::vector<TextBlock>::size_type block_index = 0; block_index < blocks.size(); block_index++)
	{
		if (objects[current_line.object_index].type == object_text)
			layout_text(gc, blocks, block_index, current_line, max_width);
		else
			layout_block(current_line, max_width, blocks, block_index);
	}
	next_line(current_line);
}

void CL_SpanLayout_Impl::layout_block(CurrentLine &current_line, int max_width, std::vector<TextBlock> &blocks, std::vector<TextBlock>::size_type block_index)
{
	if (objects[current_line.object_index].float_type == float_none)
		layout_inline_block(current_line, max_width, blocks, block_index);
	else
		layout_float_block(current_line, max_width);

	current_line.object_index++;
}

void CL_SpanLayout_Impl::layout_inline_block(CurrentLine &current_line, int max_width, std::vector<TextBlock> &blocks, std::vector<TextBlock>::size_type block_index)
{
	CL_Size size;
	LineSegment segment;
	if (objects[current_line.object_index].type == object_image)
	{
		size = objects[current_line.object_index].image.get_size();
		segment.type = object_image;
		segment.image = objects[current_line.object_index].image;
	}
	else if (objects[current_line.object_index].type == object_component)
	{
		size = objects[current_line.object_index].component->get_size();
		segment.type = object_component;
		segment.component = objects[current_line.object_index].component;
	}

	if (current_line.x_position + size.width > max_width)
		next_line(current_line);

	segment.x_position = current_line.x_position;
	segment.width = size.width;
	segment.start = blocks[block_index].start;
	segment.end = blocks[block_index].end;
	segment.id = objects[current_line.object_index].id;
	segment.ascender = size.height - objects[current_line.object_index].baseline_offset;
	current_line.cur_line.segments.push_back(segment);
	current_line.cur_line.height = cl_max(current_line.cur_line.height, size.height+objects[current_line.object_index].baseline_offset);
	current_line.cur_line.ascender = cl_max(current_line.cur_line.ascender, segment.ascender);
	current_line.x_position += size.width;
}

void CL_SpanLayout_Impl::layout_float_block(CurrentLine &current_line, int max_width)
{
	FloatBox floatbox;
	floatbox.type = objects[current_line.object_index].type;
	floatbox.image = objects[current_line.object_index].image;
	floatbox.component = objects[current_line.object_index].component;
	floatbox.id = objects[current_line.object_index].id;
	if (objects[current_line.object_index].type == object_image)
		floatbox.rect = CL_Rect(CL_Point(0, current_line.y_position), floatbox.image.get_size());
	else if (objects[current_line.object_index].type == object_component)
		floatbox.rect = CL_Rect(CL_Point(0, current_line.y_position), floatbox.component->get_size());

	if (objects[current_line.object_index].float_type == float_left)
		floats_left.push_back(float_box_left(floatbox, max_width));
	else
		floats_right.push_back(float_box_right(floatbox, max_width));

	reflow_line(current_line, max_width);
}

void CL_SpanLayout_Impl::reflow_line(CurrentLine &step, int max_width)
{
}

CL_SpanLayout_Impl::FloatBox CL_SpanLayout_Impl::float_box_left(FloatBox box, int max_width)
{
	return float_box_any(box, max_width, floats_left);
}

CL_SpanLayout_Impl::FloatBox CL_SpanLayout_Impl::float_box_right(FloatBox box, int max_width)
{
	return float_box_any(box, max_width, floats_right);
}

CL_SpanLayout_Impl::FloatBox CL_SpanLayout_Impl::float_box_any(FloatBox box, int max_width, const std::vector<FloatBox> &floats1)
{
	bool restart;
	do
	{
		restart = false;
		for (size_t i=0; i<floats1.size(); i++)
		{
			int top = cl_max(floats1[i].rect.top, box.rect.top);
			int bottom = cl_min(floats1[i].rect.bottom, box.rect.bottom);
			if (bottom > top && box.rect.left < floats1[i].rect.right)
			{
				CL_Size s = box.rect.get_size();
				box.rect.left = floats1[i].rect.left;
				box.rect.right = box.rect.left+s.width;

				if (!box_fits_on_line(box, max_width))
				{
					box.rect.left = 0;
					box.rect.right = s.width;
					box.rect.top = floats1[i].rect.bottom;
					box.rect.bottom = box.rect.top + s.height;
					restart = true;
					break;
				}
			}
		}
	} while(restart);
	return box;
}

bool CL_SpanLayout_Impl::box_fits_on_line(const FloatBox &box, int max_width)
{
	for (size_t i=0; i<floats_right.size(); i++)
	{
		int top = cl_max(floats_right[i].rect.top, box.rect.top);
		int bottom = cl_min(floats_right[i].rect.bottom, box.rect.bottom);
		if (bottom > top)
		{
			if (box.rect.right + floats_right[i].rect.right > max_width)
				return false;
		}
	}
	return true;
}

void CL_SpanLayout_Impl::layout_text(CL_GraphicContext & gc, std::vector<TextBlock> blocks, std::vector<TextBlock>::size_type block_index, CurrentLine &current_line, int max_width)
{
	TextSizeResult text_size_result = find_text_size(gc, blocks[block_index], current_line.object_index);
	current_line.object_index += text_size_result.objects_traversed;

	current_line.cur_line.width = current_line.x_position;

	if (is_newline(blocks[block_index]))
	{
		current_line.cur_line.height = cl_max(current_line.cur_line.height, text_size_result.height);
		current_line.cur_line.ascender = cl_max(current_line.cur_line.ascender, text_size_result.ascender);
		next_line(current_line);
	}
	else
	{
		if (!fits_on_line(current_line.x_position, text_size_result, max_width) && !is_whitespace(blocks[block_index]))
		{
			if (larger_than_line(text_size_result, max_width))
			{
				// force line breaks to make it fit
				force_place_line_segments(current_line, text_size_result, max_width);
			}
			else
			{
				next_line(current_line);
				place_line_segments(current_line, text_size_result);
			}
		}
		else
		{
			place_line_segments(current_line, text_size_result);
		}
	}
}

void CL_SpanLayout_Impl::next_line(CurrentLine &current_line)
{
	current_line.cur_line.width = current_line.x_position;
	for (std::vector<LineSegment>::reverse_iterator it = current_line.cur_line.segments.rbegin(); it != current_line.cur_line.segments.rend(); ++it)
	{
		LineSegment &segment = *it;
		if (segment.type == object_text)
		{
			CL_StringRef s = text.substr(segment.start, segment.end-segment.start);
			if (s.find_first_not_of(" \t\r\n") != CL_StringRef::npos)
			{
				current_line.cur_line.width = segment.x_position + segment.width;
				break;
			}
		}
		else
		{
			current_line.cur_line.width = segment.x_position + segment.width;
			break;
		}
	}

	int height = current_line.cur_line.height;
	lines.push_back(current_line.cur_line);
	current_line.cur_line = Line();
	current_line.x_position = 0;
	current_line.y_position += height;
}

void CL_SpanLayout_Impl::place_line_segments(CurrentLine &current_line, TextSizeResult &text_size_result)
{
	for (std::vector<LineSegment>::iterator it = text_size_result.segments.begin(); it != text_size_result.segments.end(); ++it)
	{
		LineSegment segment = *it;
		segment.x_position += current_line.x_position;
		current_line.cur_line.segments.push_back(segment);
	}
	current_line.x_position += text_size_result.width;
	current_line.cur_line.height = cl_max(current_line.cur_line.height, text_size_result.height);
	current_line.cur_line.ascender = cl_max(current_line.cur_line.ascender, text_size_result.ascender);
}

void CL_SpanLayout_Impl::force_place_line_segments(CurrentLine &current_line, TextSizeResult &text_size_result, int max_width)
{
	if (current_line.x_position != 0)
		next_line(current_line);

	// to do: do this properly - for now we just place the entire block on one line
	place_line_segments(current_line, text_size_result);
}

bool CL_SpanLayout_Impl::is_newline(const TextBlock &block)
{
	return block.start != block.end && text[block.start] == '\n';
}

bool CL_SpanLayout_Impl::is_whitespace(const TextBlock &block)
{
	return block.start != block.end && text[block.start] == ' ';
}

bool CL_SpanLayout_Impl::fits_on_line(int x_position, const TextSizeResult &text_size_result, int max_width)
{
	return x_position + text_size_result.width <= max_width;
}

bool CL_SpanLayout_Impl::larger_than_line(const TextSizeResult &text_size_result, int max_width)
{
	return text_size_result.width > max_width;
}

void CL_SpanLayout_Impl::align_right(int max_width)
{
	for (std::vector<Line>::size_type line_index = 0; line_index < lines.size(); line_index++)
	{
		Line &line = lines[line_index];
		int offset = max_width - line.width;
		if (offset < 0) offset = 0;

		for (std::vector<LineSegment>::size_type segment_index = 0; segment_index < line.segments.size(); segment_index++)
		{
			LineSegment &segment = line.segments[segment_index];
			segment.x_position += offset;
		}
	}
}

void CL_SpanLayout_Impl::align_center(int max_width)
{
	for (std::vector<Line>::size_type line_index = 0; line_index < lines.size(); line_index++)
	{
		Line &line = lines[line_index];
		int offset = (max_width - line.width)/2;
		if (offset < 0) offset = 0;

		for (std::vector<LineSegment>::size_type segment_index = 0; segment_index < line.segments.size(); segment_index++)
		{
			LineSegment &segment = line.segments[segment_index];
			segment.x_position += offset;
		}
	}
}

void CL_SpanLayout_Impl::align_justify(int max_width)
{
	// Note, we do not justify the last line
	for (std::vector<Line>::size_type line_index = 0; line_index + 1 < lines.size(); line_index++)
	{
		Line &line = lines[line_index];
		int offset = max_width - line.width;
		if (offset < 0) offset = 0;

		if (line.segments.size() <= 1)	// Do not justify line if only one word exists
			continue;

		for (std::vector<LineSegment>::size_type segment_index = 0; segment_index < line.segments.size(); segment_index++)
		{
			LineSegment &segment = line.segments[segment_index];
			segment.x_position += (offset * segment_index) / (line.segments.size()-1);
		}
	}
}

CL_Size CL_SpanLayout_Impl::find_preferred_size(CL_GraphicContext &gc)
{
	layout_lines(gc, 0x70000000); // Feed it with a very long length so it ends up on one line
	return get_size();
}

void CL_SpanLayout_Impl::set_selection_range(CL_String::size_type start, CL_String::size_type end)
{
	sel_start = start;
	sel_end = end;
	if (sel_end < sel_start)
		sel_end = sel_start;
}

void CL_SpanLayout_Impl::set_selection_colors(const CL_Colorf &foreground, const CL_Colorf &background)
{
	sel_foreground = foreground;
	sel_background = background;
}

CL_String CL_SpanLayout_Impl::get_combined_text() const
{
	return text;
}

void CL_SpanLayout_Impl::set_component_geometry()
{
	int x = position.x;
	int y = position.y;
	for (size_t i = 0; i < lines.size(); i++)
	{
		for (size_t j = 0; j < lines[i].segments.size(); j++)
		{
			if (lines[i].segments[j].type == object_component)
			{
				CL_Point pos(x+lines[i].segments[j].x_position, y + lines[i].ascender - lines[i].segments[j].ascender);
				CL_Size size = lines[i].segments[j].component->get_size();
				CL_Rect rect(pos, size);
				lines[i].segments[j].component->set_geometry(rect);
			}
		}
		y += lines[i].height;
	}
}

int CL_SpanLayout_Impl::get_first_baseline_offset()
{
	if (!lines.empty())
	{
		return lines.front().ascender;
	}
	else
	{
		return 0;
	}
}

int CL_SpanLayout_Impl::get_last_baseline_offset()
{
	if (!lines.empty())
	{
		int y = 0;
		for (size_t i = 0; i + 1 < lines.size(); i++)
			y += lines[i].height;
		return y + lines.back().ascender;
	}
	else
	{
		return 0;
	}
}
