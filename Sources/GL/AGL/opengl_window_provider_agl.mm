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

#include "GL/precomp.h"
#include "opengl_window_provider_agl.h"
#include "API/Core/Math/rect.h"
#include "API/Display/Window/display_window_description.h"
#include "API/Display/display.h"
#include "API/Display/display_target.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Render/shared_gc_data.h"
#include "API/GL/opengl.h"
#include "API/GL/opengl_wrap.h"
#include "API/GL/opengl_window_description.h"
#include "API/Core/Text/logger.h"
//#include "Display/Cocoa/cursor_provider_cocoa.h"
#include "../opengl_window_description_impl.h"
#include "../opengl_graphic_context_provider.h"
#include "../opengl_target_provider.h"

void cl_agl_make_none_current()
{
    [EAGLContext setCurrentContext:nil];    
}

CL_OpenGLWindowProvider *cl_alloc_display_window_agl()
{
    return new CL_OpenGLWindowProvider_AGL();
}

CL_GL_RenderWindowProvider_AGL::CL_GL_RenderWindowProvider_AGL(CL_OpenGLWindowProvider_AGL & window, EAGLContext *agl_context, bool own_context)
	: window(window), agl_context(agl_context), own_context(own_context)
{
}

CL_GL_RenderWindowProvider_AGL::~CL_GL_RenderWindowProvider_AGL()
{
	if( agl_context && own_context)
	{
        [agl_context release];
        agl_context = 0;
	}
}

int CL_GL_RenderWindowProvider_AGL::get_viewport_width() const
{
	return window.get_viewport().get_width();
}

int CL_GL_RenderWindowProvider_AGL::get_viewport_height() const
{
	return window.get_viewport().get_height();
}

void CL_GL_RenderWindowProvider_AGL::flip_buffers(int interval) const
{
	window.flip(interval);
}

void CL_GL_RenderWindowProvider_AGL::make_current() const
{
    [EAGLContext setCurrentContext:agl_context];
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLWindowProvider_AGL Construction:

CL_OpenGLWindowProvider_AGL::CL_OpenGLWindowProvider_AGL()
: cocoa_window(), opengl_context(0), swap_interval(-1)
{
	cocoa_window.func_on_resized().set(this, &CL_OpenGLWindowProvider_AGL::on_window_resized);
}

CL_OpenGLWindowProvider_AGL::~CL_OpenGLWindowProvider_AGL()
{
	if (opengl_context)
	{
		// save gc provider pointer so we can delete it later from shared list
		CL_GraphicContextProvider *destroyed_gc_provider = gc.get_provider();

		// Delete context from list of gc's that share textures.
		if (CL_SharedGCData::get_instance()) // Check that the cache hasn't been destroyed yet
		{
			std::vector<CL_GraphicContextProvider*> &gc_providers = CL_SharedGCData::get_gc_providers();
			if (gc_providers.size() == 1)
				CL_SharedGCData::dispose_objects();
			for (std::vector<CL_GraphicContextProvider*>::iterator it=gc_providers.begin(); it != gc_providers.end(); ++it)
			{
				if (destroyed_gc_provider == (*it))
				{
					gc_providers.erase(it);
					break;
				}
			}
		}

		// Destroy graphic context before the window is destroyed
		gc = CL_GraphicContext();

		// Delete the context
        CL_OpenGL::set_active(NULL);
        [opengl_context release];
		opengl_context = 0;
	}

}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLWindowProvider_AGL Attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLWindowProvider_AGL Operations:

void CL_OpenGLWindowProvider_AGL::create(CL_DisplayWindowSite *new_site, const CL_DisplayWindowDescription &desc)
{
	site = new_site;

	cocoa_window.create(site, desc);

	if (!opengl_context)
	{
        opengl_context = create_context();
        
		gc = CL_GraphicContext(new CL_OpenGLGraphicContextProvider(new CL_GL_RenderWindowProvider_AGL(*this, opengl_context, false)));
		std::vector<CL_GraphicContextProvider*> &gc_providers = CL_SharedGCData::get_gc_providers();
		gc_providers.push_back(gc.get_provider());
	}

	swap_interval = desc.get_swap_interval();
	if (swap_interval != -1)
	{
/*		if (glXSwapIntervalSGI)
		{
			glXSwapIntervalSGI(swap_interval);
		}*/
	}
}

void CL_OpenGLWindowProvider_AGL::on_window_resized()
{
	if (gc.get_provider())
		((CL_OpenGLGraphicContextProvider *) gc.get_provider())->on_window_resized();
}

EAGLContext *CL_OpenGLWindowProvider_AGL::create_context()
{
	EAGLContext *shared_context = NULL;

	std::vector<CL_GraphicContextProvider*> &gc_providers = CL_SharedGCData::get_gc_providers();
	if (!gc_providers.empty())
	{
		CL_OpenGLGraphicContextProvider *gl_provider = dynamic_cast<CL_OpenGLGraphicContextProvider*>(gc_providers.back());
		if (gl_provider)
		{
			const CL_RenderWindowProvider *rwp = &gl_provider->get_render_window();
			const CL_GL_RenderWindowProvider_AGL *render_window_agl = dynamic_cast<const CL_GL_RenderWindowProvider_AGL*>(rwp);
			if (render_window_agl)
				shared_context = render_window_agl->get_context();
		}
	}
    
    EAGLContext *context;
    if (shared_context)
    {
        // Possible memory leak: Do we have to release sharegroup here?
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2 sharegroup:shared_context.sharegroup];
    }
    else
    {
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2 sharegroup:nil];
    }
    
	//CL_GraphicContext gc = get_gc();
	//CL_OpenGL::set_active(gc);
    [EAGLContext setCurrentContext:context];
        
    // to do: create renderbuffer object that renderbufferStorage binds to
    
    GLuint framebuffer_handle = 0;
    glGenFramebuffers(1, &framebuffer_handle);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_handle);
    
    GLuint colorbuffer_handle = 0;
    glGenRenderbuffers(1, &colorbuffer_handle);
    glBindRenderbuffer(GL_RENDERBUFFER, colorbuffer_handle);
    
    BOOL result = [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:cocoa_window.get_drawable()];
    if (!result)
    {
        [context release];
        throw CL_Exception("EAGLContext.renderbufferStorage failed");
    }
    
    GLint width = 0, height = 0;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorbuffer_handle);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        [context release];
        throw CL_Exception("glCheckFramebufferStatus failed");
    }
    
    glViewport(0, 0, width, height);
    
    return context;
}

void CL_OpenGLWindowProvider_AGL::flip(int interval)
{
	CL_GraphicContext gc = get_gc();
	CL_OpenGL::set_active(gc);
	clFlush();
/*
	if (interval != -1 && interval != swap_interval)
	{
		swap_interval = interval;
		if (glXSwapIntervalSGI)
		{
			glXSwapIntervalSGI(swap_interval);
		}
		else if (glXSwapIntervalMESA)
		{
			glXSwapIntervalMESA(swap_interval);
		}
	}
*/ 
    [opengl_context presentRenderbuffer:GL_RENDERBUFFER];
    
    GLint width = 0, height = 0;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
    glViewport(0, 0, width, height);
    
}

void CL_OpenGLWindowProvider_AGL::update(const CL_Rect &_rect)
{
	int width = get_viewport().get_width();
	int height = get_viewport().get_height();

	CL_Rect rect = _rect;
	if (rect.left < 0)
		rect.left = 0;
	if (rect.top < 0)
		rect.top = 0;
	if (rect.right > width)
		rect.right = width;
	if (rect.bottom > height)
		rect.bottom = height;
	if (rect.right <= rect.left || rect.bottom <= rect.top)
		return;

	CL_OpenGL::set_active(gc);
	clFlush();

	CLboolean isDoubleBuffered = CL_TRUE;
	clGetBooleanv(CL_DOUBLEBUFFER, &isDoubleBuffered);
	if (isDoubleBuffered)
	{
		CLint read_last_bound;
		CLint draw_last_bound;

		clGetIntegerv(CL_READ_FRAMEBUFFER_BINDING, &read_last_bound);
		clGetIntegerv(CL_DRAW_FRAMEBUFFER_BINDING, &draw_last_bound);

		clBindFramebuffer(CL_READ_FRAMEBUFFER, 0);
	    clBindFramebuffer(CL_DRAW_FRAMEBUFFER, 0);

		clReadBuffer(CL_BACK);
		clDrawBuffer(CL_FRONT);

		clBlitFramebuffer( 
			rect.left, height - rect.bottom,
			rect.right, height - rect.top,
			rect.left, height - rect.bottom,
			rect.right, height - rect.top,
			CL_COLOR_BUFFER_BIT, CL_LINEAR);

		clDrawBuffer(CL_BACK);
		clReadBuffer(CL_FRONT);

		if (read_last_bound)
			clBindFramebuffer(CL_READ_FRAMEBUFFER, read_last_bound);

		if (draw_last_bound)
			clBindFramebuffer(CL_DRAW_FRAMEBUFFER, draw_last_bound);

		clFlush();
	}
}


CL_CursorProvider *CL_OpenGLWindowProvider_AGL::create_cursor(const CL_SpriteDescription &sprite_description, const CL_Point &hotspot)
{
//	return new CL_CursorProvider_Cocoa(sprite_description, hotspot);
    return 0;
}

void CL_OpenGLWindowProvider_AGL::set_cursor(CL_CursorProvider *cursor)
{
//	cocoa_window.set_cursor(static_cast<CL_CursorProvider_Cocoa *>(cursor));
}

void CL_OpenGLWindowProvider_AGL::set_large_icon(const CL_PixelBuffer &image)
{
	cocoa_window.set_large_icon(image);
}

void CL_OpenGLWindowProvider_AGL::set_small_icon(const CL_PixelBuffer &image)
{
	cocoa_window.set_small_icon(image);
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLWindowProvider_AGL Implementation:

