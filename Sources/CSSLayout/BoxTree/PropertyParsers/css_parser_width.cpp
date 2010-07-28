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
#include "css_parser_width.h"
#include "API/CSSLayout/css_box_properties.h"

std::vector<CL_String> CL_CSSParserWidth::get_names()
{
	std::vector<CL_String> names;
	names.push_back("width");
	return names;
}

void CL_CSSParserWidth::parse(CL_CSSBoxProperties &properties, const CL_String &name, const std::vector<CL_CSSToken> &tokens)
{
	size_t pos = 0;
	CL_CSSToken token = next_token(pos, tokens);
	if (token.type == CL_CSSToken::type_ident && pos == tokens.size())
	{
		if (token.value == "auto")
			properties.width.type = CL_CSSBoxWidth::type_auto;
		else if (token.value == "inherit")
			properties.width.type = CL_CSSBoxWidth::type_inherit;
		else if (token.value == "-clan-expanding")
			properties.width.type = CL_CSSBoxWidth::type_clan_expanding;
	}
	else if (is_length(token) && pos == tokens.size())
	{
		CL_CSSBoxLength length;
		if (parse_length(token, length) && length.value >= 0.0f)
		{
			properties.width.type = CL_CSSBoxWidth::type_length;
			properties.width.length = length;
		}
	}
	else if (token.type == CL_CSSToken::type_percentage && pos == tokens.size())
	{
		float v = CL_StringHelp::text_to_float(token.value);
		if (v >= 0.0f)
		{
			properties.width.type = CL_CSSBoxWidth::type_percentage;
			properties.width.percentage = v;
		}
	}
}
