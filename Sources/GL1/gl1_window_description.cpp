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

#include "GL1/precomp.h"
#include "API/GL1/opengl1_window_description.h"
#include "gl1_window_description_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GL1WindowDescription construction:

CL_GL1WindowDescription::CL_GL1WindowDescription()
{
	impl_gl = cl_dynamic_pointer_cast<CL_GL1WindowDescription_Impl>(get_data("OpenGL"));
	if (!impl_gl)
	{
		impl_gl = CL_SharedPtr<CL_GL1WindowDescription_Impl>(new CL_GL1WindowDescription_Impl);
		set_data("OpenGL", impl_gl);
	}
}

CL_GL1WindowDescription::~CL_GL1WindowDescription()
{
}

CL_GL1WindowDescription::CL_GL1WindowDescription(const CL_DisplayWindowDescription &desc)
{
	CL_DisplayWindowDescription::operator=(desc);
	impl_gl = cl_dynamic_pointer_cast<CL_GL1WindowDescription_Impl>(get_data("OpenGL"));
	if (!impl_gl)
	{
		impl_gl = CL_SharedPtr<CL_GL1WindowDescription_Impl>(new CL_GL1WindowDescription_Impl);
		set_data("OpenGL", impl_gl);
	}
}

CL_GL1WindowDescription &CL_GL1WindowDescription::operator=(CL_DisplayWindowDescription &desc)
{
	CL_DisplayWindowDescription::operator=(desc);
	impl_gl = cl_dynamic_pointer_cast<CL_GL1WindowDescription_Impl>(get_data("OpenGL"));
	if (!impl_gl)
	{
		impl_gl = CL_SharedPtr<CL_GL1WindowDescription_Impl>(new CL_GL1WindowDescription_Impl);
		set_data("OpenGL", impl_gl);
	}
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GL1WindowDescription attributes:

int CL_GL1WindowDescription::get_buffer_size() const
{
	return impl_gl->buffer_size;
}

bool CL_GL1WindowDescription::get_doublebuffer() const
{
	return impl_gl->doublebuffer;
}

bool CL_GL1WindowDescription::get_stereo() const
{
	return impl_gl->stereo;
}

int CL_GL1WindowDescription::get_red_size() const
{
	return impl_gl->red_size;
}

int CL_GL1WindowDescription::get_green_size() const
{
	return impl_gl->green_size;
}

int CL_GL1WindowDescription::get_blue_size() const
{
	return impl_gl->blue_size;
}

int CL_GL1WindowDescription::get_alpha_size() const
{
	return impl_gl->alpha_size;
}

int CL_GL1WindowDescription::get_multisampling() const
{
	return impl_gl->multisampling;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GL1WindowDescription operations:

void CL_GL1WindowDescription::set_doublebuffer(bool value)
{
	impl_gl->doublebuffer = value;
}

void CL_GL1WindowDescription::set_stereo(bool value)
{
	impl_gl->stereo = value;
}

void CL_GL1WindowDescription::set_buffer_size(int value)
{
	impl_gl->buffer_size = value;
}

void CL_GL1WindowDescription::set_red_size(int value)
{
	impl_gl->red_size = value;
}

void CL_GL1WindowDescription::set_green_size(int value)
{
	impl_gl->green_size = value;
}

void CL_GL1WindowDescription::set_blue_size(int value)
{
	impl_gl->blue_size = value;
}

void CL_GL1WindowDescription::set_alpha_size(int value)
{
	impl_gl->alpha_size = value;
}

void CL_GL1WindowDescription::set_multisampling(int value)
{
	impl_gl->multisampling = value;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GL1WindowDescription implementation:
