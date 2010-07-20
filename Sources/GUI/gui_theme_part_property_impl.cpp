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

#include "GUI/precomp.h"
#include "gui_theme_part_property_impl.h"
#include "gui_theme_part_impl.h"
#include "API/Core/Math/hash_functions.h"
#include "API/Core/Text/string_help.h"

CL_GUIThemePartProperty_Impl::CL_GUIThemePartProperty_Impl(const CL_StringRef &property_name, const CL_StringRef &default_value)
: property_name(property_name), default_value(default_value)
{
	property_name_hash = CL_HashFunctions::sha1(CL_StringHelp::text_to_utf8(property_name));
}

CL_StringRef CL_GUIThemePartProperty_Impl::get_value(const CL_GUIThemePart &part) const
{
	return part.impl->get_property(part, property_name, property_name_hash, default_value);
}
