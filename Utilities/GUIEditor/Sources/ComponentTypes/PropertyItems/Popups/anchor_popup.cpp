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

#include "precomp.h"
#include "anchor_popup.h"

AnchorPopup::AnchorPopup(CL_ComponentAnchorPoint cap_tl, CL_ComponentAnchorPoint cap_br, CL_GUIComponent *parent)
: PropertyItemPopup(parent), cap_tl(cap_tl), cap_br(cap_br)
{
	func_resized().set(this, &AnchorPopup::on_resized);

	label_tl = new CL_Label(this);
	label_br = new CL_Label(this);
	tl = new CL_RadioButton(this);
	tr = new CL_RadioButton(this);
	bl = new CL_RadioButton(this);
	br = new CL_RadioButton(this);
	scale = new CL_RadioButton(this);
	br_tl = new CL_RadioButton(this);
	br_tr = new CL_RadioButton(this);
	br_bl = new CL_RadioButton(this);
	br_br = new CL_RadioButton(this);
	br_scale = new CL_RadioButton(this);
}

AnchorPopup::~AnchorPopup()
{
}

void AnchorPopup::on_resized()
{
	if (get_height() != 150)
	{
		set_geometry(CL_Rect(get_geometry().get_top_left(), CL_Size(get_width(), 150)));
	}
	else
	{
		int top = 0;
		int left = 20;

		label_tl->set_text("Top Left:");
		label_tl->set_geometry(CL_RectPS(left-10,top+6, 80,23));

		tl->set_id_name("tl"); tr->set_id_name("tr"); bl->set_id_name("bl"); br->set_id_name("br");
		scale->set_id_name("%");

		top += 20;

		CL_Size s = get_geometry().get_size();
		//top = rect.bottom+4+32;
		tl->set_geometry(CL_RectPS(        left, top,    20, 23));
 		tr->set_geometry(CL_RectPS(     50+left, top,    20, 23));
		bl->set_geometry(CL_RectPS(        left, top+32, 20, 23));
		br->set_geometry(CL_RectPS(     50+left, top+32, 20, 23));
		scale->set_geometry(CL_RectPS(  25+left, top+16, 20, 23));

		tl->set_group_name("TL");
		tr->set_group_name("TL");
		bl->set_group_name("TL");
		br->set_group_name("TL");
		scale->set_group_name("TL");

		br_tl->set_id_name("tl"); br_tr->set_id_name("tr"); br_bl->set_id_name("bl"); br_br->set_id_name("br");
		br_scale->set_id_name("%");

		top += 50;

		label_br->set_text("Bottom Right:");
		label_br->set_geometry(CL_RectPS(left-10,top+6, 80,23));

		top += 20;

		br_tl->set_geometry(CL_RectPS(        left, top,    20, 23));
		br_tr->set_geometry(CL_RectPS(     left+50, top,    20, 23));
		br_bl->set_geometry(CL_RectPS(        left, top+32, 20, 23));
		br_br->set_geometry(CL_RectPS(     left+50, top+32, 20, 23));
		br_scale->set_geometry(CL_RectPS(  left+25, top+16, 20, 23));

		br_tl->set_group_name("BR");
		br_tr->set_group_name("BR");
		br_bl->set_group_name("BR");
		br_br->set_group_name("BR");
		br_scale->set_group_name("BR");

		scale->func_group_selection_changed().set(this, &AnchorPopup::on_anchoring_changed);
		br_scale->func_group_selection_changed().set(this, &AnchorPopup::on_anchoring_changed);

//		CL_ComponentAnchorPoint cap_tl = selected_object->get_anchor_tl();
//		CL_ComponentAnchorPoint cap_br = selected_object->get_anchor_br();

		switch (cap_tl)
		{
		case cl_anchor_top_left: tl->set_selected(true); break;
		case cl_anchor_top_right: tr->set_selected(true); break;
		case cl_anchor_bottom_left: bl->set_selected(true); break;
		case cl_anchor_bottom_right: br->set_selected(true); break;
		case cl_anchor_relative: scale->set_selected(true); break;
		default:break;
		}

		switch (cap_br)
		{
		case cl_anchor_top_left: br_tl->set_selected(true); break;
		case cl_anchor_top_right: br_tr->set_selected(true); break;
		case cl_anchor_bottom_left: br_bl->set_selected(true); break;
		case cl_anchor_bottom_right: br_br->set_selected(true); break;
		case cl_anchor_relative: br_scale->set_selected(true); break;
		default:break;
		}
	}
}

void AnchorPopup::on_anchoring_changed(CL_RadioButton *rb)
{
	CL_String id = rb->get_id_name();

	if (rb->get_group_name() == "TL")
	{
		if (id == "tl")
		{
			cap_tl = cl_anchor_top_left;
		}
		else if (id == "tr")
		{
			cap_tl = cl_anchor_top_right;
		}
		else if (id == "bl")
		{
			cap_tl = cl_anchor_bottom_left;
		}
		else if (id == "br")
		{
			cap_tl = cl_anchor_bottom_right;
		}
		else if (id == "%")
		{
			cap_tl = cl_anchor_relative;
		}
	}
	else
	{
		if (id == "tl")
		{
			cap_br = cl_anchor_top_left;
		}
		else if (id == "tr")
		{
			cap_br = cl_anchor_top_right;
		}
		else if (id == "bl")
		{
			cap_br = cl_anchor_bottom_left;
		}
		else if (id == "br")
		{
			cap_br = cl_anchor_bottom_right;
		}
		else if (id == "%")
		{
			cap_br = cl_anchor_relative;
		}
	}
}
