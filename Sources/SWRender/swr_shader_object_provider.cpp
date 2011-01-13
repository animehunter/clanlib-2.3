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

#include "SWRender/precomp.h"
#include "swr_shader_object_provider.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SWRenderShaderObjectProvider Construction:

CL_SWRenderShaderObjectProvider::CL_SWRenderShaderObjectProvider()
{
}

CL_SWRenderShaderObjectProvider::~CL_SWRenderShaderObjectProvider()
{
}

void CL_SWRenderShaderObjectProvider::create(CL_ShaderType type, const CL_StringRef &source)
{
}

void CL_SWRenderShaderObjectProvider::create(CL_ShaderType type, const std::vector<CL_StringRef> &sources)
{
}

void CL_SWRenderShaderObjectProvider::destroy()
{
	delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SWRenderShaderObjectProvider Attributes:

unsigned int CL_SWRenderShaderObjectProvider::get_handle() const
{
	return 0;
}

bool CL_SWRenderShaderObjectProvider::get_compile_status() const
{
	return false;
}

CL_ShaderType CL_SWRenderShaderObjectProvider::get_shader_type() const
{
	return cl_shadertype_vertex;
}

CL_String CL_SWRenderShaderObjectProvider::get_info_log() const
{
	return CL_String();
}

CL_String CL_SWRenderShaderObjectProvider::get_shader_source() const
{
	return CL_String();
}

/////////////////////////////////////////////////////////////////////////////
// CL_SWRenderShaderObjectProvider Operations:

void CL_SWRenderShaderObjectProvider::compile()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_SWRenderShaderObjectProvider Implementation:
