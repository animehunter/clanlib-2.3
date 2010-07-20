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

#include "Display/precomp.h"
#include "API/Display/display.h"
#include "API/Display/display_target.h"
#include "API/Display/Window/display_window.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Display Implementation:

static CL_WeakPtr<CL_DisplayTarget_Impl> _cl_current_target;

/////////////////////////////////////////////////////////////////////////////
// CL_Display Attributes:

CL_DisplayTarget CL_Display::get_current_target()
{
	return CL_DisplayTarget(_cl_current_target);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Display Operations:

void CL_Display::set_current_target(const CL_DisplayTarget &target, bool only_set_if_null)
{
	if (only_set_if_null == false || _cl_current_target.is_null())
		_cl_current_target = target.impl;
}
