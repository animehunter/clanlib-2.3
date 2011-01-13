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
#include "grid_edit_state_object_sizing.h"
#include "GridComponent/grid_component.h"
#include "MainWindow/main_window.h"
#include "Selection/selection.h"
#include "GridComponent/grid_object.h"

GridEditStateObjectSizing::GridEditStateObjectSizing()
{
}

bool GridEditStateObjectSizing::on_input_pressed(const CL_InputEvent &e)
{
	if(e.shift && (e.id == CL_KEY_LEFT || e.id == CL_KEY_RIGHT || e.id == CL_KEY_UP || e.id == CL_KEY_DOWN))
	{
		if(e.id == CL_KEY_LEFT)
			dir = CL_Vec2i(1, 0);
		else if(e.id == CL_KEY_RIGHT)
			dir = CL_Vec2i(-1, 0);
		else if(e.id == CL_KEY_UP)
			dir = CL_Vec2i(0, 1);
		else if(e.id == CL_KEY_DOWN)
			dir = CL_Vec2i(0, -1);

		std::vector<GridObject*> selection = grid->main_window->get_selection()->get_selection();
		if(selection.size())
		{
			start = CL_Point();
			primary_object_index = 0;
			start_geometry.clear();
			for (size_t j = 0; j < selection.size(); j++)
				start_geometry.push_back(selection[j]->get_geometry());

			resize_to(-dir, false);

			return true;
		}

		return false;
	}
	else if (e.id == CL_MOUSE_LEFT)
	{
		dir = CL_Vec2i(0,0);
		std::vector<GridObject*> selection = grid->main_window->get_selection()->get_selection();
		for (size_t i = 0; i < selection.size(); i++)
		{
			GridObject *h = selection[i];
			CL_Point h_mouse_pos = grid->grid_to_object_coords(h, e.mouse_pos);

			if (h->get_grabber_e().contains(h_mouse_pos))
				dir = CL_Vec2i(1,0);
			else if (h->get_grabber_se().contains(h_mouse_pos))
				dir = CL_Vec2i(1,1);
			else if (h->get_grabber_s().contains(h_mouse_pos))
				dir = CL_Vec2i(0,1);
			else if (h->get_grabber_sw().contains(h_mouse_pos))
				dir = CL_Vec2i(-1,1);
			else if (h->get_grabber_w().contains(h_mouse_pos))
				dir = CL_Vec2i(-1,0);
			else if (h->get_grabber_nw().contains(h_mouse_pos))
				dir = CL_Vec2i(-1,-1);
			else if (h->get_grabber_n().contains(h_mouse_pos))
				dir = CL_Vec2i(0,-1);
			else if (h->get_grabber_ne().contains(h_mouse_pos))
				dir = CL_Vec2i(1,-1);

			if (dir != CL_Vec2i(0,0))
			{
				start = e.mouse_pos;
				primary_object_index = i;
				start_geometry.clear();
				for (size_t j = 0; j < selection.size(); j++)
					start_geometry.push_back(selection[j]->get_geometry());
				grid->capture_mouse(true);
				return true;
			}
		}

		return false;
	}
	else
	{
		return false;
	}
}

bool GridEditStateObjectSizing::on_input_released(const CL_InputEvent &e)
{
	if (e.id == CL_MOUSE_LEFT)
	{
		bool perform_snap = e.alt == false;
		resize_to(e.mouse_pos, perform_snap);
		grid->capture_mouse(false);
		grid->edit_state.set_state(GridEditState::state_none);
		grid->main_window->get_selection()->sig_selection_changed().invoke();
		return true;
	}
	else
	{
		return false;
	}
}

bool GridEditStateObjectSizing::on_input_doubleclick(const CL_InputEvent &e)
{
	return false;
}

bool GridEditStateObjectSizing::on_input_pointer_moved(const CL_InputEvent &e)
{
	bool perform_snap = e.alt == false;
	resize_to(e.mouse_pos, perform_snap);
	return true;
}

void GridEditStateObjectSizing::resize_to(const CL_Point &mouse_pos, bool perform_snap)
{
	CL_Vec2i delta = mouse_pos - start;

	std::vector<GridObject*> selection = grid->main_window->get_selection()->get_selection();
	GridObject *object = selection[primary_object_index];

	CL_Rect source_rect = resize_rect(start_geometry[primary_object_index], delta);

	if(perform_snap)
	{
		object->set_geometry(source_rect);	// Needed for snaplines to use original position and not snapped
		std::vector<SnapLine> source_snaplines = get_filtered_snaplines(object->get_snaplines());

		delta += grid->snap(object, source_snaplines, resize_rect(start_geometry[primary_object_index], delta));
	}

	if (selection.size() == start_geometry.size())
	{
		for (size_t i = 0; i < selection.size(); i++)
		{
			CL_Rect geometry = resize_rect(start_geometry[i], delta);
			selection[i]->set_geometry(geometry);
		}
	}

	grid->request_repaint();
}

CL_Rect GridEditStateObjectSizing::resize_rect(CL_Rect geometry, CL_Vec2i delta) const
{
	if (dir.x < 0)
		geometry.left += delta.x;
	else if (dir.x > 0)
		geometry.right += delta.x;

	if (dir.y < 0)
		geometry.top += delta.y;
	else if (dir.y > 0)
		geometry.bottom += delta.y;

	return geometry;
}

std::vector<SnapLine> GridEditStateObjectSizing::get_filtered_snaplines(const std::vector<SnapLine> &original_snaplines)
{
	std::vector<SnapLine> snaplines;

	for(size_t i = 0; i < original_snaplines.size(); ++i)
	{
		if(original_snaplines[i].get_priority() == SnapLine::priority_medium)
		{
			if(original_snaplines[i].get_type() == SnapLine::type_left && dir.x < 0 ||
				original_snaplines[i].get_type() == SnapLine::type_right && dir.x > 0 ||
				original_snaplines[i].get_type() == SnapLine::type_top && dir.y < 0 ||
				original_snaplines[i].get_type() == SnapLine::type_bottom && dir.y > 0)
				snaplines.push_back(original_snaplines[i]);
		}
	}

	return snaplines;
}
