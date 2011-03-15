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

#include "CSSLayout/precomp.h"
#include "API/CSSLayout/dom_select_node.h"
#include "API/Core/XML/dom_named_node_map.h"
#include "API/Core/XML/dom_attr.h"

CL_DomSelectNode::CL_DomSelectNode(const CL_DomElement &element)
: dom_element(element), pos(element)
{
}

bool CL_DomSelectNode::parent()
{
	CL_DomNode parent_node = pos.get_parent_node();
	if (parent_node.is_element())
	{
		pos = parent_node.to_element();
		return true;
	}
	else
	{
		return false;
	}
}

bool CL_DomSelectNode::prev_sibling()
{
	CL_DomNode prev_node = pos.get_previous_sibling();
	while (!prev_node.is_null() && !prev_node.is_element())
		prev_node = prev_node.get_previous_sibling();

	if (prev_node.is_element())
	{
		pos = prev_node.to_element();
		return true;
	}
	else
	{
		return false;
	}
}

CL_String CL_DomSelectNode::name()
{
	return pos.get_tag_name();
}

CL_String CL_DomSelectNode::lang()
{
	return pos.get_attribute("lang");
}

CL_String CL_DomSelectNode::id()
{
	return pos.get_attribute("id");
}

std::vector<CL_String> CL_DomSelectNode::element_classes()
{
	return CL_StringHelp::split_text(pos.get_attribute("class"), " ");
}

std::vector<CL_String> CL_DomSelectNode::pseudo_classes()
{
	std::vector<CL_String> classes;
	if (pos.get_previous_sibling().is_null())
		classes.push_back("first-child");
	if (pos.get_tag_name() == "a" || pos.get_tag_name() == "A")
		classes.push_back("link");
	return classes;
}

CL_String CL_DomSelectNode::get_attribute_value(const CL_String &name, bool &out_found)
{
	if (pos.has_attribute(name)) // To do: need a case insensitive version of this for HTML
	{
		out_found = true;
		return pos.get_attribute(name);
	}
	else
	{
		out_found = false;
		return CL_String();
	}
}

int CL_DomSelectNode::child_index()
{
	int index = 0;
	CL_DomNode cur = pos;
	while (!cur.is_null())
	{
		index++;
		cur = cur.get_previous_sibling();
	}
	return index;
}

void CL_DomSelectNode::push()
{
	saved_elements.push_back(pos);
}

void CL_DomSelectNode::pop()
{
	pos = saved_elements.back();
	saved_elements.pop_back();
}
