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
**    Harry Storbacka
*/

#include "Display/precomp.h"
#include "API/Display/Render/render_buffer.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Display/TargetProviders/frame_buffer_provider.h"
#include "API/Display/TargetProviders/render_buffer_provider.h"

class CL_RenderBuffer_Impl
{
public:
	CL_RenderBuffer_Impl() : provider(0)
	{
	}

	CL_RenderBufferProvider *provider;
	CL_Size size;
};

/////////////////////////////////////////////////////////////////////////////
// CL_RenderBuffer Construction:

CL_RenderBuffer::CL_RenderBuffer()
{
}

CL_RenderBuffer::CL_RenderBuffer(CL_GraphicContext &context, int width, int height, CL_TextureFormat internal_format, int multisample_samples)
: impl(new CL_RenderBuffer_Impl)
{
	CL_GraphicContextProvider *gc_provider = context.get_provider();
	impl->provider = gc_provider->alloc_render_buffer();
	impl->provider->create(width, height, internal_format, multisample_samples);
	impl->size.width = width;
	impl->size.height = height; 
}

/////////////////////////////////////////////////////////////////////////////
// CL_RenderBuffer Attributes:

void CL_RenderBuffer::throw_if_null() const
{
	if (impl.is_null())
		throw CL_Exception("CL_RenderBuffer is null");
}

CL_RenderBufferProvider *CL_RenderBuffer::get_provider() const
{
	return impl->provider;
}

const CL_Size &CL_RenderBuffer::get_size() const
{
	return impl->size;
}

/////////////////////////////////////////////////////////////////////////////
// CL_RenderBuffer Operations:

bool CL_RenderBuffer::operator==(const CL_RenderBuffer &other) const
{
	return impl == other.impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_RenderBuffer Implementation:


