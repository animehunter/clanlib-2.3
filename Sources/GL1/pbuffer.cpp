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
**    Mark Page
*/

#include "GL1/precomp.h"

#ifdef WIN32
#include "WGL/pbuffer_impl.h"
#else
#include "GLX/pbuffer_impl.h"
#endif

#include "gl1_graphic_context_provider.h"

CL_PBuffer_GL1::CL_PBuffer_GL1()
{
}

CL_PBuffer_GL1::CL_PBuffer_GL1(CL_GL1GraphicContextProvider *gc_provider) : impl(new CL_PBuffer_GL1_Impl(gc_provider))
{
}

CL_PBuffer_GL1::~CL_PBuffer_GL1()
{
}

#ifdef WIN32
void CL_PBuffer_GL1::create(CL_GL1WindowProvider_WGL &window_provider, CL_Size &size)
{
	impl->create(window_provider, size);
}
#else
void CL_PBuffer_GL1::create(CL_GL1WindowProvider_GLX &window_provider, CL_Size &size)
{
	impl->create(window_provider, size);
}
#endif

void CL_PBuffer_GL1::set_active() const
{
	impl->set_active();
}

void CL_PBuffer_GL1::throw_if_null() const
{
	if (impl.is_null())
		throw CL_Exception("is null");
}
