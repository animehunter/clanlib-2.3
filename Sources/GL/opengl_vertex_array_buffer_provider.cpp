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
**    Mark Page
*/

#include "GL/precomp.h"
#include "opengl_vertex_array_buffer_provider.h"
#include "opengl_graphic_context_provider.h"
#include "API/GL/opengl_wrap.h"
#include "API/Display/Render/shared_gc_data.h"

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLVertexArrayBufferProvider Construction:

CL_OpenGLVertexArrayBufferProvider::CL_OpenGLVertexArrayBufferProvider()
{
}

CL_OpenGLVertexArrayBufferProvider::~CL_OpenGLVertexArrayBufferProvider()
{
}

void CL_OpenGLVertexArrayBufferProvider::create(int size, CL_BufferUsage usage)
{
	create(0, size, usage);
}

void CL_OpenGLVertexArrayBufferProvider::create(void *data, int size, CL_BufferUsage usage)
{
	buffer.create(data, size, usage, GL_ARRAY_BUFFER_BINDING, GL_ARRAY_BUFFER);
}

