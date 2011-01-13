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
**    Harry Storbacka
**    Kenneth Gangstoe
**    Mark Page
*/

#pragma once

class CL_ClanImageStretch
{
public:
	static void draw_image(CL_GraphicContext &gc, const CL_Rect &rect, CL_Image &image, int sizing_left, int sizing_top, int sizing_right, int sizing_bottom);

private:
	struct ImagePart
	{
		CL_Rect source_rect;
		CL_Rect dest_rect;
	};

	static std::vector<ImagePart> calc_stretch_image_parts(const CL_Rect &dest, CL_Image &sprite, int sizing_left, int sizing_top, int sizing_right, int sizing_bottom);
};
