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

#include "GL1/precomp.h"
#include "API/GL1/opengl1_target.h"
#include "gl1_target_provider.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GL1Target Construction:

CL_GL1Target::CL_GL1Target()
: CL_DisplayTarget(new CL_GL1TargetProvider)
{
}

CL_GL1Target::~CL_GL1Target()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GL1Target Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_GL1Target Operations:

/////////////////////////////////////////////////////////////////////////////
// CL_GL1Target Implementation:
