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
#include "css_parser_padding.h"
#include "API/CSSLayout/css_box_properties.h"

std::vector<CL_String> CL_CSSParserPadding::get_names()
{
	std::vector<CL_String> names;
	names.push_back("padding");
	return names;
}

void CL_CSSParserPadding::parse(CL_CSSBoxProperties &properties, const CL_String &name, const std::vector<CL_CSSToken> &tokens, std::map<CL_String, CL_CSSBoxProperties *> *out_change_set)
{
	CL_CSSBoxPaddingWidth padding_widths[4];
	int count;
	size_t pos = 0;
	for (count = 0; count < 4; count++)
	{
		CL_CSSToken token = next_token(pos, tokens);
		if (token.type == CL_CSSToken::type_ident && equals(token.value, "inherit") && count == 0 && pos == tokens.size())
		{
			properties.padding_width_left.type = CL_CSSBoxPaddingWidth::type_inherit;
			properties.padding_width_top.type = CL_CSSBoxPaddingWidth::type_inherit;
			properties.padding_width_right.type = CL_CSSBoxPaddingWidth::type_inherit;
			properties.padding_width_bottom.type = CL_CSSBoxPaddingWidth::type_inherit;
			return;
		}
		else if (is_length(token))
		{
			CL_CSSBoxLength length;
			if (parse_length(token, length))
			{
				padding_widths[count].type = CL_CSSBoxPaddingWidth::type_length;
				padding_widths[count].length = length;
			}
			else
			{
				debug_parse_error(name, tokens);
				return;
			}
		}
		else if (token.type == CL_CSSToken::type_percentage)
		{
			padding_widths[count].type = CL_CSSBoxPaddingWidth::type_percentage;
			padding_widths[count].percentage = CL_StringHelp::text_to_float(token.value);
		}
		else if (token.type == CL_CSSToken::type_delim && token.value == "-")
		{
			token = next_token(pos, tokens);
			if (is_length(token))
			{
				CL_CSSBoxLength length;
				if (parse_length(token, length))
				{
					length.value = -length.value;
					padding_widths[count].type = CL_CSSBoxPaddingWidth::type_length;
					padding_widths[count].length = length;
				}
				else
				{
					debug_parse_error(name, tokens);
					return;
				}
			}
			else if (token.type == CL_CSSToken::type_percentage)
			{
				padding_widths[count].type = CL_CSSBoxPaddingWidth::type_percentage;
				padding_widths[count].percentage = -CL_StringHelp::text_to_float(token.value);
			}
			else
			{
				debug_parse_error(name, tokens);
				return;
			}
		}
		else if (token.type == CL_CSSToken::type_null)
		{
			break;
		}
		else
		{
			debug_parse_error(name, tokens);
			return;
		}
	}

	if (pos == tokens.size())
	{
		switch (count)
		{
		case 1:
			properties.padding_width_left = padding_widths[0];
			properties.padding_width_top = padding_widths[0];
			properties.padding_width_right = padding_widths[0];
			properties.padding_width_bottom = padding_widths[0];
			break;
		case 2:
			properties.padding_width_top = padding_widths[0];
			properties.padding_width_bottom = padding_widths[0];
			properties.padding_width_left = padding_widths[1];
			properties.padding_width_right = padding_widths[1];
			break;
		case 3:
			properties.padding_width_top = padding_widths[0];
			properties.padding_width_left = padding_widths[1];
			properties.padding_width_right = padding_widths[1];
			properties.padding_width_bottom = padding_widths[2];
			break;
		case 4:
			properties.padding_width_top = padding_widths[0];
			properties.padding_width_right = padding_widths[1];
			properties.padding_width_bottom = padding_widths[2];
			properties.padding_width_left = padding_widths[3];
			break;
		default:
			break;
		}
	}
}
