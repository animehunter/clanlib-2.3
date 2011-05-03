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
#include "API/CSSLayout/PropertyTypes/css_box_shadow.h"
#include "../../css_resource_cache.h"

CL_CSSBoxShadow::CL_CSSBoxShadow()
: type(type_none)
{
}

void CL_CSSBoxShadow::compute(const CL_CSSBoxShadow *parent, CL_CSSResourceCache *layout, float em_size, float ex_size)
{
	if (type == type_inherit)
	{
		if (parent)
		{
			type = parent->type;
			shadows = parent->shadows;
		}
		else
		{
			type = type_none;
			shadows.clear();
		}
	}

	for (size_t i = 0; i < shadows.size(); i++)
	{
		shadows[i].horizontal_offset = layout->compute_length(shadows[i].horizontal_offset, em_size, ex_size);
		shadows[i].vertical_offset = layout->compute_length(shadows[i].vertical_offset, em_size, ex_size);
		shadows[i].blur_radius = layout->compute_length(shadows[i].blur_radius, em_size, ex_size);
		shadows[i].spread_distance = layout->compute_length(shadows[i].spread_distance, em_size, ex_size);
	}
}
