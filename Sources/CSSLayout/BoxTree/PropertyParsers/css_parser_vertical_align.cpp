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
**    Magnus Norddahl
*/

#include "CSSLayout/precomp.h"
#include "css_parser_vertical_align.h"
#include "API/CSSLayout/css_box_properties.h"

std::vector<CL_String> CL_CSSParserVerticalAlign::get_names()
{
	std::vector<CL_String> names;
	names.push_back("vertical-align");
	return names;
}

void CL_CSSParserVerticalAlign::parse(CL_CSSBoxProperties &properties, const CL_String &name, const std::vector<CL_CSSToken> &tokens)
{
	size_t pos = 0;
	CL_CSSToken token = next_token(pos, tokens);
	if (token.type == CL_CSSToken::type_ident && pos == tokens.size())
	{
		if (equals(token.value, "baseline"))
			properties.vertical_align.type = CL_CSSBoxVerticalAlign::type_baseline;
		else if (equals(token.value, "sub"))
			properties.vertical_align.type = CL_CSSBoxVerticalAlign::type_sub;
		else if (equals(token.value, "super"))
			properties.vertical_align.type = CL_CSSBoxVerticalAlign::type_super;
		else if (equals(token.value, "top"))
			properties.vertical_align.type = CL_CSSBoxVerticalAlign::type_top;
		else if (equals(token.value, "text-top"))
			properties.vertical_align.type = CL_CSSBoxVerticalAlign::type_text_top;
		else if (equals(token.value, "middle"))
			properties.vertical_align.type = CL_CSSBoxVerticalAlign::type_middle;
		else if (equals(token.value, "bottom"))
			properties.vertical_align.type = CL_CSSBoxVerticalAlign::type_bottom;
		else if (equals(token.value, "text-bottom"))
			properties.vertical_align.type = CL_CSSBoxVerticalAlign::type_text_bottom;
		else if (equals(token.value, "inherit"))
			properties.vertical_align.type = CL_CSSBoxVerticalAlign::type_inherit;
	}
	else if (is_length(token) && pos == tokens.size())
	{
		CL_CSSBoxLength length;
		if (parse_length(token, length))
		{
			properties.vertical_align.type = CL_CSSBoxVerticalAlign::type_length;
			properties.vertical_align.length = length;
		}
	}
	else if (token.type == CL_CSSToken::type_percentage && pos == tokens.size())
	{
		properties.vertical_align.type = CL_CSSBoxVerticalAlign::type_percentage;
		properties.vertical_align.percentage = CL_StringHelp::text_to_float(token.value);
	}
	else if (token.type == CL_CSSToken::type_delim && token.value == "-")
	{
		token = next_token(pos, tokens);
		if (is_length(token) && pos == tokens.size())
		{
			CL_CSSBoxLength length;
			if (parse_length(token, length))
			{
				length.value = -length.value;
				properties.vertical_align.type = CL_CSSBoxVerticalAlign::type_length;
				properties.vertical_align.length = length;
			}
		}
		else if (token.type == CL_CSSToken::type_percentage && pos == tokens.size())
		{
			properties.vertical_align.type = CL_CSSBoxVerticalAlign::type_percentage;
			properties.vertical_align.percentage = -CL_StringHelp::text_to_float(token.value);
		}
	}
}
