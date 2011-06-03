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
#include "css_parser_background_repeat.h"
#include "API/CSSLayout/css_box_properties.h"

std::vector<CL_String> CL_CSSParserBackgroundRepeat::get_names()
{
	std::vector<CL_String> names;
	names.push_back("background-repeat");
	return names;
}

void CL_CSSParserBackgroundRepeat::parse(CL_CSSBoxProperties &properties, const CL_String &name, const std::vector<CL_CSSToken> &tokens, std::map<CL_String, CL_CSSBoxProperty *> *out_change_set)
{
	size_t pos = 0;
	CL_CSSToken token = next_token(pos, tokens);

	if (token.type == CL_CSSToken::type_ident && pos == tokens.size() && equals(token.value, "inherit"))
	{
		properties.background_repeat.type = CL_CSSBoxBackgroundRepeat::type_inherit;
	}
	else
	{
		CL_CSSBoxBackgroundRepeat background_repeat;
		background_repeat.type = CL_CSSBoxBackgroundRepeat::type_repeat_style;
		background_repeat.repeat_x.clear();
		background_repeat.repeat_y.clear();
		while (true)
		{
			if (token.type != CL_CSSToken::type_ident)
				return;

			CL_CSSBoxBackgroundRepeat::RepeatStyle repeat_x, repeat_y;
			bool single_style = false;

			if (equals(token.value, "repeat"))
			{
				repeat_x = CL_CSSBoxBackgroundRepeat::style_repeat;
			}
			else if (equals(token.value, "repeat-x"))
			{
				repeat_x = CL_CSSBoxBackgroundRepeat::style_repeat;
				repeat_y = CL_CSSBoxBackgroundRepeat::style_no_repeat;
				single_style = true;
			}
			else if (equals(token.value, "repeat-y"))
			{
				repeat_x = CL_CSSBoxBackgroundRepeat::style_no_repeat;
				repeat_y = CL_CSSBoxBackgroundRepeat::style_repeat;
				single_style = true;
			}
			else if (equals(token.value, "no-repeat"))
			{
				repeat_x = CL_CSSBoxBackgroundRepeat::style_no_repeat;
			}
			else if (equals(token.value, "space"))
			{
				repeat_x = CL_CSSBoxBackgroundRepeat::style_space;
			}
			else if (equals(token.value, "round"))
			{
				repeat_x = CL_CSSBoxBackgroundRepeat::style_round;
			}
			else
			{
				return;
			}

			if (pos == tokens.size())
			{
				if (!single_style)
					repeat_y = repeat_x;
				background_repeat.repeat_x.push_back(repeat_x);
				background_repeat.repeat_y.push_back(repeat_y);
				break;
			}

			token = next_token(pos, tokens);
			if (token.type == CL_CSSToken::type_delim && token.value == ",")
			{
				if (!single_style)
					repeat_y = repeat_x;
				background_repeat.repeat_x.push_back(repeat_x);
				background_repeat.repeat_y.push_back(repeat_y);
			}
			else if (token.type == CL_CSSToken::type_ident && !single_style)
			{
				if (equals(token.value, "repeat"))
				{
					repeat_y = CL_CSSBoxBackgroundRepeat::style_repeat;
				}
				else if (equals(token.value, "no-repeat"))
				{
					repeat_y = CL_CSSBoxBackgroundRepeat::style_no_repeat;
				}
				else if (equals(token.value, "space"))
				{
					repeat_y = CL_CSSBoxBackgroundRepeat::style_space;
				}
				else if (equals(token.value, "round"))
				{
					repeat_y = CL_CSSBoxBackgroundRepeat::style_round;
				}
				else
				{
					return;
				}

				background_repeat.repeat_x.push_back(repeat_x);
				background_repeat.repeat_y.push_back(repeat_y);

				if (pos == tokens.size())
					break;

				if (token.type != CL_CSSToken::type_delim || token.value != ",")
					return;
			}
			else
			{
				return;
			}

			token = next_token(pos, tokens);
		}

		properties.background_repeat = background_repeat;
	}
	if (out_change_set)
	{
		(*out_change_set)["background-repeat"] = &properties.background_repeat;
	}
}
