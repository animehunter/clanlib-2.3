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

#pragma once


#include "API/Display/api_display.h"
#include "API/Display/TargetProviders/cursor_provider.h"
#include "API/Core/System/databuffer.h"
#include <vector>

class CL_PixelBufferRef;
class CL_Point;
class CL_DataBuffer;
class CL_SpriteDescription;

class CL_API_DISPLAY CL_CursorProvider_X11 : public CL_CursorProvider
{
/// \name Construction
/// \{

public:
	CL_CursorProvider_X11(const CL_SpriteDescription &sprite_description, const CL_Point &hotspot);

	~CL_CursorProvider_X11();


/// \}
/// \name Attributes
/// \{

public:
	int handle;


/// \}
/// \name Operations
/// \{

public:
	/// \brief Destroys the cursor provider.
	void destroy() { delete this; }


/// \}
/// \name Implementation
/// \{

private:
	int create_cursor(const CL_SpriteDescription &sprite_description, const CL_Point &hotspot) const;

/// \}
};


