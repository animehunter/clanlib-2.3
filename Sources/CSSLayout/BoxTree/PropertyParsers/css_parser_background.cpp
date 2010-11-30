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
#include "css_parser_background.h"
#include "API/CSSLayout/css_box_properties.h"

std::vector<CL_String> CL_CSSParserBackground::get_names()
{
	std::vector<CL_String> names;
	names.push_back("background");
	return names;
}

void CL_CSSParserBackground::parse(CL_CSSBoxProperties &properties, const CL_String &name, const std::vector<CL_CSSToken> &tokens)
{
	bool color_specified = false;
	bool image_specified = false;
	bool repeat_specified = false;
	bool attachment_specified = false;

	CL_CSSBoxBackgroundColor bgcolor;
	CL_Colorf color;

	CL_CSSBoxBackgroundImage bgimage;
	CL_CSSBoxBackgroundRepeat bgrepeat;
	CL_CSSBoxBackgroundAttachment bgattachment;

	CL_CSSBoxBackgroundPosition bgposition;
	bool x_specified = false;
	bool y_specified = false;
	bool center_specified = false;

	size_t pos = 0;
	while (pos != tokens.size())
	{
		if (!color_specified && parse_color(tokens, pos, color))
		{
			bgcolor.type = CL_CSSBoxBackgroundColor::type_color;
			bgcolor.color = color;
			color_specified = true;
		}
		else
		{
			CL_CSSToken token = next_token(pos, tokens);
			if (token.type == CL_CSSToken::type_ident && token.value == "inherit" && tokens.size() == 1)
			{
				properties.background_color.type = CL_CSSBoxBackgroundColor::type_inherit;
				properties.background_image.type = CL_CSSBoxBackgroundImage::type_inherit;
				properties.background_repeat.type = CL_CSSBoxBackgroundRepeat::type_inherit;
				properties.background_attachment.type = CL_CSSBoxBackgroundAttachment::type_inherit;
				properties.background_position.type = CL_CSSBoxBackgroundPosition::type_inherit;
				return;
			}
			else if (token.type == CL_CSSToken::type_ident)
			{
				if (!color_specified && token.value == "transparent")
				{
					bgcolor.type = CL_CSSBoxBackgroundColor::type_transparent;
					color_specified = true;
				}
				else if (!image_specified && token.value == "none")
				{
					bgimage.type = CL_CSSBoxBackgroundImage::type_none;
					image_specified = true;
				}
				else if (!repeat_specified && token.value == "repeat")
				{
					bgrepeat.type = CL_CSSBoxBackgroundRepeat::type_repeat;
					repeat_specified = true;
				}
				else if (!repeat_specified && token.value == "repeat-x")
				{
					bgrepeat.type = CL_CSSBoxBackgroundRepeat::type_repeat_x;
					repeat_specified = true;
				}
				else if (!repeat_specified && token.value == "repeat-y")
				{
					bgrepeat.type = CL_CSSBoxBackgroundRepeat::type_repeat_y;
					repeat_specified = true;
				}
				else if (!repeat_specified && token.value == "no-repeat")
				{
					bgrepeat.type = CL_CSSBoxBackgroundRepeat::type_no_repeat;
					repeat_specified = true;
				}
				else if (!repeat_specified && token.value == "-clan-stretch")
				{
					bgrepeat.type = CL_CSSBoxBackgroundRepeat::type_clan_stretch;
					repeat_specified = true;
				}
				else if (!attachment_specified && token.value == "scroll")
				{
					bgattachment.type = CL_CSSBoxBackgroundAttachment::type_scroll;
					attachment_specified = true;
				}
				else if (!attachment_specified && token.value == "fixed")
				{
					bgattachment.type = CL_CSSBoxBackgroundAttachment::type_fixed;
					attachment_specified = true;
				}
				else if (!y_specified && token.value == "top")
				{
					bgposition.type_y = CL_CSSBoxBackgroundPosition::type2_top;
					y_specified = true;

					if (center_specified)
					{
						bgposition.type_x = CL_CSSBoxBackgroundPosition::type1_center;
						x_specified = true;
						center_specified = false;
					}
				}
				else if (!y_specified && token.value == "bottom")
				{
					bgposition.type_y = CL_CSSBoxBackgroundPosition::type2_bottom;
					y_specified = true;

					if (center_specified)
					{
						bgposition.type_x = CL_CSSBoxBackgroundPosition::type1_center;
						x_specified = true;
						center_specified = false;
					}
				}
				else if (!x_specified && token.value == "left")
				{
					bgposition.type_x = CL_CSSBoxBackgroundPosition::type1_left;
					x_specified = true;

					if (center_specified)
					{
						bgposition.type_y = CL_CSSBoxBackgroundPosition::type2_center;
						y_specified = true;
						center_specified = false;
					}
				}
				else if (!x_specified && token.value == "right")
				{
					bgposition.type_x = CL_CSSBoxBackgroundPosition::type1_right;
					x_specified = true;

					if (center_specified)
					{
						bgposition.type_y = CL_CSSBoxBackgroundPosition::type2_center;
						y_specified = true;
						center_specified = false;
					}
				}
				else if (token.value == "center")
				{
					if (center_specified)
					{
						bgposition.type_x = CL_CSSBoxBackgroundPosition::type1_center;
						x_specified = true;
						center_specified = false;
					}

					if (x_specified && !y_specified)
					{
						bgposition.type_y = CL_CSSBoxBackgroundPosition::type2_center;
						y_specified = true;
					}
					else if (y_specified && !x_specified)
					{
						bgposition.type_x = CL_CSSBoxBackgroundPosition::type1_center;
						x_specified = true;
					}
					else if (!x_specified && !y_specified)
					{
						center_specified = true;
					}
					else
					{
						return;
					}
				}
				else
				{
					return;
				}
			}
			else if (!image_specified && token.type == CL_CSSToken::type_uri)
			{
				bgimage.type = CL_CSSBoxBackgroundImage::type_uri;
				bgimage.url = token.value;
				image_specified = true;
			}
			else if (is_length(token))
			{
				CL_CSSBoxLength length;
				if (parse_length(token, length))
				{
					if (center_specified)
					{
						bgposition.type_x = CL_CSSBoxBackgroundPosition::type1_center;
						x_specified = true;
						center_specified = false;
					}

					if (!x_specified && !y_specified)
					{
						bgposition.type_x = CL_CSSBoxBackgroundPosition::type1_length;
						bgposition.length_x = length;
						x_specified = true;
					}
					else if (x_specified && !y_specified)
					{
						bgposition.type_y = CL_CSSBoxBackgroundPosition::type2_length;
						bgposition.length_y = length;
						y_specified = true;
					}
					else
					{
						return;
					}
				}
				else
				{
					return;
				}
			}
			else if (token.type == CL_CSSToken::type_percentage)
			{
				if (center_specified)
				{
					bgposition.type_x = CL_CSSBoxBackgroundPosition::type1_center;
					x_specified = true;
					center_specified = false;
				}

				if (!x_specified && !y_specified)
				{
					bgposition.type_x = CL_CSSBoxBackgroundPosition::type1_percentage;
					bgposition.percentage_x = CL_StringHelp::text_to_float(token.value);
					x_specified = true;
				}
				else if (x_specified && !y_specified)
				{
					bgposition.type_y = CL_CSSBoxBackgroundPosition::type2_percentage;
					bgposition.percentage_y = CL_StringHelp::text_to_float(token.value);
					y_specified = true;
				}
				else
				{
					return;
				}
			}
			else
			{
				return;
			}
		}
	}

	bgposition.type = CL_CSSBoxBackgroundPosition::type_value;
	if (!x_specified)
		bgposition.type_x = CL_CSSBoxBackgroundPosition::type1_center;
	else if (!y_specified)
		bgposition.type_y = CL_CSSBoxBackgroundPosition::type2_center;

	properties.background_color = bgcolor;
	properties.background_image = bgimage;
	properties.background_repeat = bgrepeat;
	properties.background_attachment = bgattachment;
	properties.background_position = bgposition;
}
