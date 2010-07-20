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
#include "css_parser_text_decoration.h"
#include "../css_box_properties.h"

std::vector<CL_String> CL_CSSParserTextDecoration::get_names()
{
	std::vector<CL_String> names;
	names.push_back("text-decoration");
	return names;
}

void CL_CSSParserTextDecoration::parse(CL_CSSBoxProperties &properties, const CL_String &name, const std::vector<CL_CSSToken> &tokens)
{
	size_t pos = 0;
	CL_CSSToken token = next_token(pos, tokens);
	if (token.type == CL_CSSToken::type_ident && pos == tokens.size())
	{
		if (token.value == "none")
		{
			properties.text_decoration.type = CL_CSSBoxTextDecoration::type_none;
		}
		else if (token.value == "inherit")
		{
			properties.text_decoration.type = CL_CSSBoxTextDecoration::type_inherit;
		}
		else
		{
			int underline = 0;
			int overline = 0;
			int line_through = 0;
			int blink = 0;
			do
			{
				if (token.type == CL_CSSToken::type_ident)
				{
					if (token.value == "underline")
						underline++;
					else if (token.value == "overline")
						overline++;
					else if (token.value == "line-through")
						line_through++;
					else if (token.value == "blink")
						blink++;
				}
				else
				{
					return;
				}

				token = next_token(pos, tokens);
			} while (token.type != CL_CSSToken::type_null);

			if (underline < 2 && overline < 2 && line_through < 2 && blink < 2)
			{
				properties.text_decoration.type = CL_CSSBoxTextDecoration::type_values;
				properties.text_decoration.underline = (underline == 1);
				properties.text_decoration.overline = (overline == 1);
				properties.text_decoration.line_through = (line_through == 1);
				properties.text_decoration.blink = (blink == 1);
			}
		}
	}
}
