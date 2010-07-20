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
**    Mark Page
*/

#pragma once

#include "info.h"

class GUI;

class CheckBox : public CL_Window
{
public:
	CheckBox(GUI *gui);

private:
	void on_checked(CL_CheckBox *checkbox);
	void on_unchecked(CL_CheckBox *checkbox);
	void on_indeterminated(CL_CheckBox *checkbox);
	void on_state_changed(CL_CheckBox *checkbox);

	void on_checked_disable(CL_CheckBox *checkbox);
	void on_unchecked_disable(CL_CheckBox *checkbox);
	void on_checked_3state(CL_CheckBox *checkbox);
	void on_unchecked_3state(CL_CheckBox *checkbox);

private:
	GUI *gui;

	CL_CheckBox *checkbox1;

	CL_CheckBox *checkbox_disable;
	CL_CheckBox *checkbox_3state;

	Info *info_checked;
	Info *info_unchecked;
	Info *info_indeterminated;
	Info *info_state_changed;
};
