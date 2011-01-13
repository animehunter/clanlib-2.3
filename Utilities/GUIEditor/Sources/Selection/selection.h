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
**    Harry Storbacka
*/

#pragma once

class DialogComponent;
class GridObject;

class Selection
{
public:
	Selection();
	
	bool empty();
	std::vector<GridObject*> get_selection();
	bool is_selected(GridObject *object) const;

	void add_object(GridObject *object);
	void clear();
	void remove_object(GridObject *object);

	CL_Signal_v0 &sig_selection_changed();

private:
	std::vector<GridObject*> selected_objects;
	CL_Signal_v0 signal_selection_changed;
};
