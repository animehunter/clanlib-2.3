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
#include "line_edit_popup.h"

LineEditPopup::LineEditPopup(CL_GUIComponent *parent)
: PropertyItemPopup(parent), lineedit(0)
{
	func_resized().set(this, &LineEditPopup::on_resized);
	func_focus_gained().set(this, &LineEditPopup::on_focus_gained);
	lineedit = new CL_LineEdit(this);
}

LineEditPopup::~LineEditPopup()
{
}

void LineEditPopup::on_resized()
{
	lineedit->set_geometry(get_size());
}

bool LineEditPopup::on_focus_gained()
{
	lineedit->set_focus();
	return true;
}
