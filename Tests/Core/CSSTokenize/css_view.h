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

#include "html_page.h"

class CL_CSSLayout;
class CL_CSSBoxElement;

class CSSView : public CL_GUIComponent
{
public:
	CSSView(CL_GUIComponent *parent);
	~CSSView();

private:
	void on_resized();
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_scroll();
	CL_Image on_layout_get_image(CL_GraphicContext &gc, const CL_String &uri);

	void load_html(const CL_String &html_filename, const CL_String &css_filename, const HTMLUrl &document_url);
	bool is_end_tag_forbidden(const CL_String &name);

	class Image
	{
	public:
		Image(CL_Image image) : image(image) { }
		int get_intrinsic_width() const { return image.get_width(); }
		int get_intrinsic_height() const { return image.get_height(); }
		float get_intrinsic_ratio() const { return image.get_height()/(float)image.get_width(); }
		void set_geometry(const CL_Rect &new_geometry) { geometry = new_geometry; }

		CL_Image image;
		CL_Rect geometry;
	};

	HTMLPage page;
	CL_CSSLayout layout;
	CL_Size last_layout_size;
	CL_CSSDocument2 css_document;
	CL_ScrollBar *scrollbar;

	CL_Rect client_box;
	CL_Rect scrollbar_box;
	CL_Rect view_box;
	std::map<CL_String, CL_Image> image_cache;
	std::vector<Image *> replaced_images;
};
