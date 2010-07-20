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

#pragma once

class CL_CSSFloat;
class CL_CSSLayoutTreeNode;
class CL_CSSLayoutCursor;

class CL_CSSBlockFormattingContext
{
public:
	CL_CSSBlockFormattingContext(CL_CSSBlockFormattingContext *parent);

	void clear();
	void set_position(int x, int y);
	CL_Rect place_left(CL_Rect box, int width);
	CL_Rect place_right(CL_Rect box, int width);
	CL_Rect float_left(CL_Rect box, int width, CL_CSSLayoutTreeNode *layout);
	CL_Rect float_right(CL_Rect box, int width, CL_CSSLayoutTreeNode *layout);
	CL_Rect find_line_box(int left, int right, int y, int height, int minimum_width) const;
	int find_left_clearance() const;
	int find_right_clearance() const;
	//int find_available_width(const CSSLayoutCursor &cursor) const;

	int get_x() const;
	int get_y() const;
	int get_local_x() const { return x; }
	int get_local_y() const { return y; }
	CL_CSSBlockFormattingContext *get_parent() { return parent; }

private:
	static bool is_null(const CL_Rect &box);
	CL_Rect find_relevant_left_box(const CL_Rect &box) const;
	CL_Rect find_relevant_right_box(const CL_Rect &box, int width) const;

	CL_CSSBlockFormattingContext *parent;
	int x, y;
	std::vector<CL_CSSFloat> left_floats;
	std::vector<CL_CSSFloat> right_floats;
};

class CL_CSSFloat
{
public:
	CL_CSSFloat(const CL_Rect &box, CL_CSSLayoutTreeNode *layout) : box(box), layout(layout) { }

	CL_Rect box;
	CL_CSSLayoutTreeNode *layout;
};
