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
#include "css_box_tree.h"
#include "css_box_element.h"
#include "css_box_text.h"
#include "css_box_node_walker.h"
#include "css_whitespace_eraser.h"
#include "API/CSSLayout/css_property2.h"
#include "API/CSSLayout/css_property_list2.h"

CL_CSSBoxTree::CL_CSSBoxTree()
: root_element(0), html_body_element(0), selection_start(0), selection_end(0), selection_start_text_offset(0), selection_end_text_offset(0)
{
}

CL_CSSBoxTree::~CL_CSSBoxTree()
{
	clear();
}

void CL_CSSBoxTree::set_root_element(CL_CSSBoxElement *new_root_element)
{
	clear();
	root_element = new_root_element;
}

void CL_CSSBoxTree::set_html_body_element(CL_CSSBoxElement *new_html_body_element)
{
	html_body_element = new_html_body_element;
}

void CL_CSSBoxTree::create(const CL_DomNode &node)
{
	clear();
	CL_CSSBoxNode *root_node = create_node(node);
	root_element = dynamic_cast<CL_CSSBoxElement*>(root_node);
	if (!root_element)
	{
		delete root_node;
		throw CL_Exception("XML contained no document element!");
	}
}

void CL_CSSBoxTree::prepare(CL_CSSResourceCache *resource_cache)
{
	clean();
	compute_element(root_element, resource_cache);
	propagate_html_body();
	convert_run_in_blocks(root_element);
	CL_CSSWhitespaceEraser::remove_whitespace(root_element);
	filter_table(resource_cache);
}

void CL_CSSBoxTree::clean(CL_CSSBoxNode *node)
{
	if (node == 0)
		node = root_element;

	CL_CSSBoxText *text = dynamic_cast<CL_CSSBoxText*>(node);
	if (text)
		text->processed_text = text->text;
	
	CL_CSSBoxNode *child = node->get_first_child();
	while (child)
	{
		clean(child);
		child = child->get_next_sibling();
	}
}

void CL_CSSBoxTree::clear()
{
	delete root_element;
	root_element = 0;
}

CL_CSSBoxNode *CL_CSSBoxTree::create_node(const CL_DomNode &node)
{
	if (node.is_element())
	{
		CL_CSSBoxElement *box_element = new CL_CSSBoxElement();
		box_element->name = node.get_node_name();
		box_element->properties = get_css_properties(node.to_element());
		create_pseudo_element(box_element, node.to_element(), "before");
		CL_DomNode cur = node.get_first_child();
		while (!cur.is_null())
		{
			CL_CSSBoxNode *box_child = create_node(cur);
			if (box_child)
				box_element->push_back(box_child);
			cur = cur.get_next_sibling();
		}
		create_pseudo_element(box_element, node.to_element(), "after");
		return box_element;
	}
	else if (node.is_text())
	{
		CL_CSSBoxText *box_text = new CL_CSSBoxText();
		box_text->set_text(node.get_node_value());
		return box_text;
	}
	else
	{
		return 0;
	}
}

void CL_CSSBoxTree::create_pseudo_element(CL_CSSBoxElement *box_element, const CL_DomElement &dom_element, const CL_String &pseudo_element)
{
	CL_CSSBoxProperties properties = get_css_properties(dom_element, pseudo_element);
	if (properties.content.type != CL_CSSBoxContent::type_none && properties.content.type != CL_CSSBoxContent::type_normal)
	{
		CL_CSSBoxElement *before_element = new CL_CSSBoxElement();
		before_element->name = cl_format("%1:%2", dom_element.get_node_name(), pseudo_element);
		before_element->properties = properties;

		CL_CSSBoxText *box_text = new CL_CSSBoxText();
		box_text->set_text(properties.content.str);

		before_element->push_back(box_text);
		box_element->push_back(before_element);
	}
}

void CL_CSSBoxTree::apply_properties(CL_CSSBoxElement *node, const CL_CSSPropertyList2 &css_properties)
{
	for (size_t i = css_properties.size(); i > 0; i--)
		property_parsers.parse(node->properties, css_properties[i-1]);
}

CL_CSSBoxProperties CL_CSSBoxTree::get_css_properties(const CL_DomElement &element, const CL_String &pseudo_element)
{
	CL_CSSBoxProperties properties;
	CL_CSSPropertyList2 css_properties = css.select(element, pseudo_element);
	for (size_t i = css_properties.size(); i > 0; i--)
		property_parsers.parse(properties, css_properties[i-1]);
	return properties;
}

void CL_CSSBoxTree::compute_element(CL_CSSBoxElement *element, CL_CSSResourceCache *resource_cache)
{
	CL_CSSBoxProperties *parent_properties = 0;
	CL_CSSBoxNode *parent_node = element->get_parent();
	if (parent_node)
		parent_properties = &dynamic_cast<CL_CSSBoxElement*>(parent_node)->computed_properties;

	element->computed_properties = element->properties;
	element->computed_properties.compute(parent_properties, resource_cache);

	CL_CSSBoxNode *cur = element->get_first_child();
	while (cur)
	{
		CL_CSSBoxElement *cur_element = dynamic_cast<CL_CSSBoxElement*>(cur);
		if (cur_element)
			compute_element(cur_element, resource_cache);
		cur = cur->get_next_sibling();
	}
}

void CL_CSSBoxTree::propagate_html_body()
{
	if (html_body_element && root_element != html_body_element)
	{
		if (root_element->computed_properties.background_image.images[0].type == CL_CSSBoxBackgroundImage::image_type_none &&
			root_element->computed_properties.background_color.color.a == 0.0f)
		{
			root_element->computed_properties.background_color = html_body_element->computed_properties.background_color;
			root_element->computed_properties.background_image = html_body_element->computed_properties.background_image;
			root_element->computed_properties.background_repeat = html_body_element->computed_properties.background_repeat;
			root_element->computed_properties.background_attachment = html_body_element->computed_properties.background_attachment;
			root_element->computed_properties.background_position = html_body_element->computed_properties.background_position;
			root_element->computed_properties.background_origin = html_body_element->computed_properties.background_origin;
			root_element->computed_properties.background_clip = html_body_element->computed_properties.background_clip;
			root_element->computed_properties.background_size = html_body_element->computed_properties.background_size;
			html_body_element->computed_properties.background_color = CL_CSSBoxBackgroundColor();
			html_body_element->computed_properties.background_image = CL_CSSBoxBackgroundImage();
		}
	}
}

void CL_CSSBoxTree::convert_run_in_blocks(CL_CSSBoxElement *element)
{
	// This code is pretty broken.  But then again, who cares?  There's not
	// a single browser out there yet that managed to implement run-in boxes properly,
	// so I doubt they are being used much..

	if (element->computed_properties.display.type == CL_CSSBoxDisplay::type_run_in)
	{
		if (element->has_block_level_children())
		{
			element->properties.display.type = CL_CSSBoxDisplay::type_block;
			element->computed_properties.display.type = CL_CSSBoxDisplay::type_block;
		}
		else
		{
			element->properties.display.type = CL_CSSBoxDisplay::type_block;
			element->computed_properties.display.type = CL_CSSBoxDisplay::type_block;

			CL_CSSBoxNode *next = element->get_next_sibling();
			while (next)
			{
				CL_CSSBoxText *next_text = dynamic_cast<CL_CSSBoxText*>(next);
				CL_CSSBoxElement *next_element = dynamic_cast<CL_CSSBoxElement*>(next);
				if (next_text)
				{
					if (next_text->processed_text.find_first_not_of(" \t\r\n") != CL_String::npos)
						break;
				}
				else if (next_element)
				{
					if (next_element->computed_properties.float_box.type != CL_CSSBoxFloat::type_none ||
						next_element->computed_properties.position.type == CL_CSSBoxPosition::type_absolute ||
						next_element->computed_properties.position.type == CL_CSSBoxPosition::type_fixed)
					{
						// Skip floating and absolute positioned boxes
					}
					else if (next_element->computed_properties.display.type == CL_CSSBoxDisplay::type_block)
					{
						element->properties.display.type = CL_CSSBoxDisplay::type_inline;
						element->computed_properties.display.type = CL_CSSBoxDisplay::type_inline;
						element->remove();
						next_element->push_front(element);
						break;
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}

				next = next->get_next_sibling();
			}
		}
	}
	else
	{
		CL_CSSBoxNode *cur = element->get_first_child();
		while (cur)
		{
			CL_CSSBoxElement *cur_element = dynamic_cast<CL_CSSBoxElement*>(cur);
			if (cur_element)
				convert_run_in_blocks(cur_element);
			cur = cur->get_next_sibling();
		}
	}
}

void CL_CSSBoxTree::filter_table(CL_CSSResourceCache *resource_cache)
{
/*
	// Remove irrelevant boxes:
	if (parent.display.type == CL_CSSBoxDisplay::type_table_column)
	{
		cur.display.type = CL_CSSBoxDisplay::type_none;
	}
	else if (parent.display.type == CL_CSSBoxDisplay::type_table_column_group &&
		cur.display.type != CL_CSSBoxDisplay::type_table_column)
	{
		cur.display.type = CL_CSSBoxDisplay::type_none;
	}
	else if (cur.is_text() &&
		cur.only_contains_whitespace() &&
		cur.prev_sibling().is_null_or_proper_table_descendant() &&
		cur.next_sibling().is_null_or_proper_table_descendant())
	{
		cur.display.type = CL_CSSBoxDisplay::type_none;
	}
	// To do: If a box B is an anonymous inline containing only white space, and is between two immediate
	// siblings each of which is either an internal table box or a 'table-caption' box then B is treated
	// as if it had 'display: none'. 
*/

	// Generate missing child wrappers:

	CL_CSSBoxNodeWalker walker(root_element->get_first_child(), true);
	while (walker.is_node())
	{
		if (walker.is_element())
		{
			CL_CSSBoxElement *cur = walker.get_element();
			CL_CSSBoxElement *parent = static_cast<CL_CSSBoxElement *>(walker.get_element()->get_parent());

			if ((parent->computed_properties.display.type == CL_CSSBoxDisplay::type_table ||
				parent->computed_properties.display.type == CL_CSSBoxDisplay::type_inline_table) &&
				cur->computed_properties.display.type != CL_CSSBoxDisplay::type_table_caption &&
				cur->computed_properties.display.type != CL_CSSBoxDisplay::type_table_row_group &&
				cur->computed_properties.display.type != CL_CSSBoxDisplay::type_table_header_group &&
				cur->computed_properties.display.type != CL_CSSBoxDisplay::type_table_footer_group &&
				cur->computed_properties.display.type != CL_CSSBoxDisplay::type_table_row)
			{
				// generate an anonymous 'table-row' box around C and all consecutive siblings of C that are not proper table children

				CL_CSSBoxElement *anonymous = new CL_CSSBoxElement();
				anonymous->properties.display.type = CL_CSSBoxDisplay::type_table_row;
				anonymous->name = "anonymous";
				parent->insert(anonymous, cur);
				cur->remove();
				anonymous->push_back(cur);

				CL_CSSBoxNode *next = anonymous->get_next_sibling();
				while (next)
				{
					cur = dynamic_cast<CL_CSSBoxElement*>(next);
					if (cur &&
						cur->computed_properties.display.type != CL_CSSBoxDisplay::type_table_caption &&
						cur->computed_properties.display.type != CL_CSSBoxDisplay::type_table_row_group &&
						cur->computed_properties.display.type != CL_CSSBoxDisplay::type_table_header_group &&
						cur->computed_properties.display.type != CL_CSSBoxDisplay::type_table_footer_group &&
						cur->computed_properties.display.type != CL_CSSBoxDisplay::type_table_row)
					{
						next = next->get_next_sibling();
						cur->remove();
						anonymous->push_back(cur);
					}
					else
					{
						break;
					}
				}

				compute_element(anonymous, resource_cache);
			}
			if ((parent->computed_properties.display.type == CL_CSSBoxDisplay::type_table_row_group ||
				parent->computed_properties.display.type == CL_CSSBoxDisplay::type_table_header_group ||
				parent->computed_properties.display.type == CL_CSSBoxDisplay::type_table_footer_group) &&
				cur->computed_properties.display.type != CL_CSSBoxDisplay::type_table_row)
			{
				// generate an anonymous 'table-row' box around C and all consecutive siblings of C that are not 'table-row' boxes

				CL_CSSBoxElement *anonymous = new CL_CSSBoxElement();
				anonymous->properties.display.type = CL_CSSBoxDisplay::type_table_row;
				anonymous->name = "anonymous";
				parent->insert(anonymous, cur);
				cur->remove();
				anonymous->push_back(cur);

				CL_CSSBoxNode *next = anonymous->get_next_sibling();
				while (next)
				{
					cur = dynamic_cast<CL_CSSBoxElement*>(next);
					if (cur && cur->computed_properties.display.type != CL_CSSBoxDisplay::type_table_row)
					{
						next = next->get_next_sibling();
						cur->remove();
						anonymous->push_back(cur);
					}
					else
					{
						break;
					}
				}

				compute_element(anonymous, resource_cache);
			}
			if (parent->computed_properties.display.type == CL_CSSBoxDisplay::type_table_row &&
				cur->computed_properties.display.type != CL_CSSBoxDisplay::type_table_cell)
			{
				// generate an anonymous 'table-cell' box around C and all consecutive siblings of C that are not 'table-cell' boxes

				CL_CSSBoxElement *anonymous = new CL_CSSBoxElement();
				anonymous->properties.display.type = CL_CSSBoxDisplay::type_table_row;
				anonymous->name = "anonymous";
				parent->insert(anonymous, cur);
				cur->remove();
				anonymous->push_back(cur);

				CL_CSSBoxNode *next = anonymous->get_next_sibling();
				while (next)
				{
					cur = dynamic_cast<CL_CSSBoxElement*>(next);
					if (cur && cur->computed_properties.display.type != CL_CSSBoxDisplay::type_table_cell)
					{
						next = next->get_next_sibling();
						cur->remove();
						anonymous->push_back(cur);
					}
					else
					{
						break;
					}
				}

				compute_element(anonymous, resource_cache);
			}
		}
		walker.next(true);
	}

	// Generate missing parents:

	walker = CL_CSSBoxNodeWalker(root_element->get_first_child(), true);
	while (walker.is_node())
	{
		if (walker.is_element())
		{
			CL_CSSBoxElement *cur = walker.get_element();
			CL_CSSBoxElement *parent = static_cast<CL_CSSBoxElement *>(walker.get_element()->get_parent());

			if (cur->computed_properties.display.type == CL_CSSBoxDisplay::type_table_cell && parent->computed_properties.display.type != CL_CSSBoxDisplay::type_table_row)
			{
				// generate an anonymous 'table-row' box around C and all consecutive siblings of C that are 'table-cell' boxes

				CL_CSSBoxElement *anonymous = new CL_CSSBoxElement();
				anonymous->properties.display.type = CL_CSSBoxDisplay::type_table_row;
				anonymous->name = "anonymous";
				parent->insert(anonymous, cur);
				cur->remove();
				anonymous->push_back(cur);

				CL_CSSBoxNode *next = anonymous->get_next_sibling();
				while (next)
				{
					cur = dynamic_cast<CL_CSSBoxElement*>(next);
					if (cur && cur->computed_properties.display.type == CL_CSSBoxDisplay::type_table_cell)
					{
						next = next->get_next_sibling();
						cur->remove();
						anonymous->push_back(cur);
					}
					else
					{
						break;
					}
				}

				compute_element(anonymous, resource_cache);
			}

			bool table_row_misparented =
				cur->computed_properties.display.type == CL_CSSBoxDisplay::type_table_row && 
				parent->computed_properties.display.type != CL_CSSBoxDisplay::type_table &&
				parent->computed_properties.display.type != CL_CSSBoxDisplay::type_inline_table;

			bool table_column_misparented = 
				cur->computed_properties.display.type == CL_CSSBoxDisplay::type_table_column &&
				parent->computed_properties.display.type != CL_CSSBoxDisplay::type_table_column_group &&
				parent->computed_properties.display.type != CL_CSSBoxDisplay::type_table &&
				parent->computed_properties.display.type != CL_CSSBoxDisplay::type_inline_table;

			bool table_group_column_caption_misparented =
				(cur->computed_properties.display.type == CL_CSSBoxDisplay::type_table_row_group ||
				cur->computed_properties.display.type == CL_CSSBoxDisplay::type_table_header_group ||
				cur->computed_properties.display.type == CL_CSSBoxDisplay::type_table_footer_group ||
				cur->computed_properties.display.type == CL_CSSBoxDisplay::type_table_column_group ||
				cur->computed_properties.display.type == CL_CSSBoxDisplay::type_table_caption) &&
				parent->computed_properties.display.type != CL_CSSBoxDisplay::type_table &&
				parent->computed_properties.display.type != CL_CSSBoxDisplay::type_inline_table;

			if (table_row_misparented || table_column_misparented || table_group_column_caption_misparented)
			{
				// generate an anonymous 'table' or 'inline-table' box T around C and all consecutive siblings of C that are proper table children
				// If C's parent is an 'inline' box, then T must be an 'inline-table' box; otherwise it must be a 'table' box

				CL_CSSBoxElement *anonymous = new CL_CSSBoxElement();
				anonymous->properties.display.type = parent->is_block_level() ? CL_CSSBoxDisplay::type_table : CL_CSSBoxDisplay::type_inline_table;
				anonymous->name = "anonymous";
				parent->insert(anonymous, cur);
				cur->remove();
				anonymous->push_back(cur);

				CL_CSSBoxNode *next = anonymous->get_next_sibling();
				while (next)
				{
					cur = dynamic_cast<CL_CSSBoxElement*>(next);
					if (cur &&
						(cur->computed_properties.display.type == CL_CSSBoxDisplay::type_table_caption ||
						cur->computed_properties.display.type == CL_CSSBoxDisplay::type_table_row_group ||
						cur->computed_properties.display.type == CL_CSSBoxDisplay::type_table_header_group ||
						cur->computed_properties.display.type == CL_CSSBoxDisplay::type_table_footer_group ||
						cur->computed_properties.display.type == CL_CSSBoxDisplay::type_table_row ||
						cur->computed_properties.display.type == CL_CSSBoxDisplay::type_table_column ||
						cur->computed_properties.display.type == CL_CSSBoxDisplay::type_table_column_group))
					{
						next = next->get_next_sibling();
						cur->remove();
						anonymous->push_back(cur);
					}
					else
					{
						break;
					}
				}

				compute_element(anonymous, resource_cache);
			}
		}
		walker.next(true);
	}
}

void CL_CSSBoxTree::set_selection(CL_CSSBoxNode *start, size_t start_text_offset, CL_CSSBoxNode *end, size_t end_text_offset)
{
	apply_selection(selection_start, selection_start_text_offset, selection_end, selection_end_text_offset, true);

	selection_start = start;
	selection_start_text_offset = start_text_offset;
	selection_end = end;
	selection_end_text_offset = end_text_offset;

	apply_selection(selection_start, selection_start_text_offset, selection_end, selection_end_text_offset, false);
}

void CL_CSSBoxTree::apply_selection(CL_CSSBoxNode *start, size_t start_offset, CL_CSSBoxNode *end, size_t end_offset, bool clear)
{
	if (start && end)
	{
		CL_CSSBoxNodeWalker walker(start, true);
		do
		{
			if (walker.is_text())
			{
				CL_CSSBoxText *text = walker.get_text();
				if (clear)
				{
					text->selection_start = 0;
					text->processed_selection_start = 0;
					text->selection_end = 0;
					text->processed_selection_end = 0;
				}
				else
				{
					text->selection_start = 0;
					text->processed_selection_start = 0;
					text->selection_end = text->text.length();
					text->processed_selection_end = text->processed_text.length();

					if (text == start)
					{
						text->selection_start = start_offset;
						text->processed_selection_start = start_offset;
					}
					if (text == end)
					{
						text->selection_end = end_offset;
						text->processed_selection_end = end_offset;
					}
				}
			}

			if (walker.get() == end)
				break;
		} while (walker.next());
	}
}
