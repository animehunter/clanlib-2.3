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
*/

#include "Core/precomp.h"
#include "API/Core/Text/string_help.h"
#include "css_selector.h"

bool CL_CSSSelector::select(const std::vector<CL_StringRef> &match_path, int &specificity) const
{
	specificity = 0;

	std::vector<CL_StringRef>::size_type selector_path_pos = path_elements.size();
	std::vector<CL_StringRef>::size_type match_path_pos = match_path.size();
	while (selector_path_pos > 0)
	{
		if (match_path_pos == 0)
			return false;

		const CL_String &selector_path_element = path_elements[selector_path_pos-1];
		const CL_String &match_path_element = match_path[match_path_pos-1];

		if (selector_path_element == "*")
		{
			// path element matched
			match_path_pos--;
			selector_path_pos--;
			continue;
		}

		CL_StringRef selector_path_type = get_type(selector_path_element);
		CL_StringRef selector_path_class = get_class(selector_path_element);
		CL_StringRef selector_path_id = get_id(selector_path_element);
		CL_StringRef selector_path_state = get_state(selector_path_element);

		CL_StringRef match_path_type = get_type(match_path_element);
		CL_StringRef match_path_class = get_class(match_path_element);
		CL_StringRef match_path_id = get_id(match_path_element);
		CL_StringRef match_path_state = get_state(match_path_element);

		if (selector_path_type != match_path_type)
			return false;
		if (!selector_path_class.empty() && selector_path_class != match_path_class)
			return false;
		if (!selector_path_id.empty() && selector_path_id != match_path_id)
			return false;
		if (selector_path_state.empty())
		{
			// path element matched
			match_path_pos--;
			selector_path_pos--;
			continue;
		}

		std::vector<CL_String> selector_states = CL_StringHelp::split_text(selector_path_state, ":");
		std::vector<CL_String> match_states = CL_StringHelp::split_text(match_path_state, ":");

		std::vector<CL_StringRef>::size_type selector_states_pos;
		for (selector_states_pos = 0; selector_states_pos < selector_states.size(); selector_states_pos++)
		{
			CL_String &selector_state = selector_states[selector_states_pos];
			
			std::vector<CL_StringRef>::size_type match_states_pos;
			bool state_matched = false;
			for (match_states_pos = 0; match_states_pos < match_states.size(); match_states_pos++)
			{
				CL_String &match_state = match_states[match_states_pos];
				if (selector_state == match_state)
				{
					state_matched = true;
					break;
				}
			}

			if (!state_matched)
				return false;
		}

		// path_element matched
		match_path_pos--;
		selector_path_pos--;
	}

	for (selector_path_pos = 0; selector_path_pos < path_elements.size(); selector_path_pos++)
	{
		const CL_String &selector_path_element = path_elements[selector_path_pos];

		CL_StringRef selector_path_type = get_type(selector_path_element);
		CL_StringRef selector_path_class = get_class(selector_path_element);
		CL_StringRef selector_path_id = get_id(selector_path_element);
		CL_StringRef selector_path_state = get_state(selector_path_element);

		if (!selector_path_id.empty())
			specificity += 0x010000;
		if (!selector_path_class.empty())
			specificity += 0x000100;
		if (!selector_path_type.empty())
			specificity += 0x000001;
		if (!selector_path_state.empty())
			specificity += 0x000001;
	}

	return true;
}

bool CL_CSSSelector::operator ==(const CL_CSSSelector &other) const
{
	return path_elements == other.path_elements;
}

CL_StringRef CL_CSSSelector::get_type(const CL_StringRef &path_element)
{
	CL_StringRef::size_type pos = path_element.find_first_of(".#:");
	if (pos == CL_StringRef::npos)
		return path_element;
	else
		return path_element.substr(0, pos);
}

CL_StringRef CL_CSSSelector::get_class(const CL_StringRef &path_element)
{
	CL_StringRef::size_type pos1 = path_element.find_first_of('.');
	if (pos1 == CL_StringRef::npos)
		return CL_StringRef();
	CL_StringRef::size_type pos2 = path_element.find_first_of("#:", pos1);
	if (pos2 == CL_StringRef::npos)
		return path_element.substr(pos1);
	else
		return path_element.substr(pos1, pos2 - pos1);
}

CL_StringRef CL_CSSSelector::get_id(const CL_StringRef &path_element)
{
	CL_StringRef::size_type pos1 = path_element.find_first_of('#');
	if (pos1 == CL_StringRef::npos)
		return CL_StringRef();
	CL_StringRef::size_type pos2 = path_element.find_first_of(".:", pos1);
	if (pos2 == CL_StringRef::npos)
		return path_element.substr(pos1);
	else
		return path_element.substr(pos1, pos2 - pos1);
}

CL_StringRef CL_CSSSelector::get_state(const CL_StringRef &path_element)
{
	CL_StringRef::size_type pos1 = path_element.find_first_of(':');
	if (pos1 == CL_StringRef::npos)
		return CL_StringRef();
	CL_StringRef::size_type pos2 = path_element.find_first_of(".#", pos1);
	if (pos2 == CL_StringRef::npos)
		return path_element.substr(pos1);
	else
		return path_element.substr(pos1, pos2 - pos1);
}
