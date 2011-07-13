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
**  Trigve Siver
**  Mark Page
*/

#pragma once


#include "../opengl_window_provider.h"
#include "Display/X11/x11_window.h"
#include "API/Display/Image/pixel_buffer.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

//#include <X11/extensions/XInput.h>

#include <GL/glx.h>

#include "API/GL/opengl_wrap.h"
#include "API/Display/TargetProviders/render_window_provider.h"

typedef int (*ptr_glXSwapIntervalSGI)(int interval);
typedef int (*ptr_glXSwapIntervalMESA)(int interval);
typedef GLXContext (*ptr_glXCreateContextAttribs)(Display *dpy, GLXFBConfig config, GLXContext share_list, Bool direct, const int *attrib_list);

class CL_OpenGLWindowProvider_GLX;

#define CL_USE_DLOPEN		// Using dlopen for linux by default

class CL_GL_GLXFunctions
{
public:
	typedef XVisualInfo* (CL_GLFUNC *ptr_glXChooseVisual)(Display *dpy, int screen, int *attrib_list);
	typedef void (CL_GLFUNC *ptr_glXCopyContext)(Display *dpy, GLXContext src, GLXContext dst, unsigned long mask);
	typedef GLXContext (CL_GLFUNC *ptr_glXCreateContext)(Display *dpy, XVisualInfo *vis, GLXContext share_list, Bool direct);
	typedef GLXPixmap (CL_GLFUNC *ptr_glXCreateGLXPixmap)(Display *dpy, XVisualInfo *vis, Pixmap pixmap);
	typedef void (CL_GLFUNC *ptr_glXDestroyContext)(Display *dpy, GLXContext ctx);
	typedef void (CL_GLFUNC *ptr_glXDestroyGLXPixmap)(Display *dpy, GLXPixmap pix);
	typedef int (CL_GLFUNC *ptr_glXGetConfig)(Display *dpy, XVisualInfo *vis, int attrib, int *value);
	typedef GLXContext (CL_GLFUNC *ptr_glXGetCurrentContext)(void);
	typedef GLXDrawable (CL_GLFUNC *ptr_glXGetCurrentDrawable)(void);
	typedef Bool (CL_GLFUNC *ptr_glXIsDirect)(Display *dpy, GLXContext ctx);
	typedef Bool (CL_GLFUNC *ptr_glXMakeCurrent)(Display *dpy, GLXDrawable drawable, GLXContext ctx);
	typedef Bool (CL_GLFUNC *ptr_glXQueryExtension)(Display *dpy, int *error_base, int *event_base);
	typedef Bool (CL_GLFUNC *ptr_glXQueryVersion)(Display *dpy, int *major, int *minor);
	typedef void (CL_GLFUNC *ptr_glXSwapBuffers)(Display *dpy, GLXDrawable drawable);
	typedef void (CL_GLFUNC *ptr_glXUseXFont)(Font font, int first, int count, int list_base);
	typedef void (CL_GLFUNC *ptr_glXWaitGL)(void);
	typedef void (CL_GLFUNC *ptr_glXWaitX)(void);
	typedef const char *(CL_GLFUNC *ptr_glXGetClientString)(Display *dpy, int name);
	typedef const char *(CL_GLFUNC *ptr_glXQueryServerString)(Display *dpy, int screen, int name);
	typedef const char *(CL_GLFUNC *ptr_glXQueryExtensionsString)(Display *dpy, int screen);
	typedef Display *(CL_GLFUNC *ptr_glXGetCurrentDisplay)(void);
	typedef GLXFBConfig *(CL_GLFUNC *ptr_glXChooseFBConfig)(Display *dpy, int screen, const int *attrib_list, int *nelements);
	typedef GLXContext (CL_GLFUNC *ptr_glXCreateNewContext)(Display *dpy, GLXFBConfig config, int render_type, GLXContext share_list, Bool direct);
	typedef GLXPbuffer (CL_GLFUNC *ptr_glXCreatePbuffer)(Display *dpy, GLXFBConfig config, const int *attrib_list);
	typedef GLXPixmap (CL_GLFUNC *ptr_glXCreatePixmap)(Display *dpy, GLXFBConfig config, Pixmap pixmap, const int *attrib_list);
	typedef GLXWindow (CL_GLFUNC *ptr_glXCreateWindow)(Display *dpy, GLXFBConfig config, Window win, const int *attrib_list);
	typedef void (CL_GLFUNC *ptr_glXDestroyPbuffer)(Display *dpy, GLXPbuffer pbuf);
	typedef void (CL_GLFUNC *ptr_glXDestroyPixmap)(Display *dpy, GLXPixmap pixmap);
	typedef void (CL_GLFUNC *ptr_glXDestroyWindow)(Display *dpy, GLXWindow win);
	typedef GLXDrawable (CL_GLFUNC *ptr_glXGetCurrentReadDrawable)(void);
	typedef int (CL_GLFUNC *ptr_glXGetFBConfigAttrib)(Display *dpy, GLXFBConfig config, int attribute, int *value);
	typedef GLXFBConfig *(CL_GLFUNC *ptr_glXGetFBConfigs)(Display *dpy, int screen, int *nelements);
	typedef void (CL_GLFUNC *ptr_glXGetSelectedEvent)(Display *dpy, GLXDrawable draw, unsigned long *event_mask);
	typedef XVisualInfo *(CL_GLFUNC *ptr_glXGetVisualFromFBConfig)(Display *dpy, GLXFBConfig config);
	typedef Bool (CL_GLFUNC *ptr_glXMakeContextCurrent)(Display *display, GLXDrawable draw, GLXDrawable read, GLXContext ctx);
	typedef int (CL_GLFUNC *ptr_glXQueryContext)(Display *dpy, GLXContext ctx, int attribute, int *value);
	typedef void (CL_GLFUNC *ptr_glXQueryDrawable)(Display *dpy, GLXDrawable draw, int attribute, unsigned int *value);
	typedef void (CL_GLFUNC *ptr_glXSelectEvent)(Display *dpy, GLXDrawable draw, unsigned long event_mask);

	typedef __GLXextFuncPtr (CL_GLFUNC *ptr_glXGetProcAddress) (const GLubyte *);
	typedef void (*(CL_GLFUNC *ptr_glXGetProcAddressARB)(const GLubyte *procName))(void);


public:
	ptr_glXChooseVisual glXChooseVisual;
	ptr_glXCopyContext glXCopyContext;
	ptr_glXCreateContext glXCreateContext;
	ptr_glXCreateGLXPixmap glXCreateGLXPixmap;
	ptr_glXDestroyContext glXDestroyContext;
	ptr_glXDestroyGLXPixmap glXDestroyGLXPixmap;
	ptr_glXGetConfig glXGetConfig;
	ptr_glXGetCurrentContext glXGetCurrentContext;
	ptr_glXGetCurrentDrawable glXGetCurrentDrawable;
	ptr_glXIsDirect glXIsDirect;
	ptr_glXMakeCurrent glXMakeCurrent;
	ptr_glXQueryExtension glXQueryExtension;
	ptr_glXQueryVersion glXQueryVersion;
	ptr_glXSwapBuffers glXSwapBuffers;
	ptr_glXUseXFont glXUseXFont;
	ptr_glXWaitGL glXWaitGL;
	ptr_glXWaitX glXWaitX;
	ptr_glXGetClientString glXGetClientString;
	ptr_glXQueryServerString glXQueryServerString;
	ptr_glXQueryExtensionsString glXQueryExtensionsString;
	ptr_glXGetCurrentDisplay glXGetCurrentDisplay;
	ptr_glXChooseFBConfig glXChooseFBConfig;
	ptr_glXCreateNewContext glXCreateNewContext;
	ptr_glXCreatePbuffer glXCreatePbuffer;
	ptr_glXCreatePixmap glXCreatePixmap;
	ptr_glXCreateWindow glXCreateWindow;
	ptr_glXDestroyPbuffer glXDestroyPbuffer;
	ptr_glXDestroyPixmap glXDestroyPixmap;
	ptr_glXDestroyWindow glXDestroyWindow;
	ptr_glXGetCurrentReadDrawable glXGetCurrentReadDrawable;
	ptr_glXGetFBConfigAttrib glXGetFBConfigAttrib;
	ptr_glXGetFBConfigs glXGetFBConfigs;
	ptr_glXGetSelectedEvent glXGetSelectedEvent;
	ptr_glXGetVisualFromFBConfig glXGetVisualFromFBConfig;
	ptr_glXMakeContextCurrent glXMakeContextCurrent;
	ptr_glXQueryContext glXQueryContext;
	ptr_glXQueryDrawable glXQueryDrawable;
	ptr_glXSelectEvent glXSelectEvent;
	ptr_glXGetProcAddress glXGetProcAddress;
	ptr_glXGetProcAddressARB glXGetProcAddressARB;

};

class CL_GL_RenderWindowProvider_GLX: public CL_RenderWindowProvider
{
public:
	CL_GL_RenderWindowProvider_GLX(CL_OpenGLWindowProvider_GLX & window, GLXContext glx_context, bool own_context);
	virtual ~CL_GL_RenderWindowProvider_GLX();
	virtual int get_viewport_width() const;
	virtual int get_viewport_height() const;
	virtual void flip_buffers(int interval) const;
	virtual void make_current() const;
	CL_ProcAddress *get_proc_address(const CL_String8& function_name) const;

	GLXContext get_context() const {return glx_context;}

private:
	CL_OpenGLWindowProvider_GLX & window;
	GLXContext glx_context;
	bool own_context;
};


class CL_OpenGLWindowProvider_GLX : public CL_OpenGLWindowProvider
{
/// \name Construction
/// \{

public:
	CL_OpenGLWindowProvider_GLX();

	~CL_OpenGLWindowProvider_GLX();


/// \}
/// \name Attributes
/// \{

public:
	CL_Rect get_geometry() const {return x11_window.get_geometry();}

	CL_Rect get_viewport() const {return x11_window.get_viewport();}

	bool is_fullscreen() const {return x11_window.is_fullscreen();}

	bool has_focus() const {return x11_window.has_focus();}

	bool is_minimized() const {return x11_window.is_minimized();}

	bool is_maximized() const {return x11_window.is_maximized();}

	bool is_visible() const {return x11_window.is_visible();}

	bool is_clipboard_text_available() const { return x11_window.is_clipboard_text_available(); }

	bool is_clipboard_image_available() const { return x11_window.is_clipboard_image_available(); }

	CL_String get_title() const { return x11_window.get_title(); }
	CL_Size get_minimum_size(bool client_area) const { return x11_window.get_minimum_size(client_area); }
	CL_Size get_maximum_size(bool client_area) const { return x11_window.get_maximum_size(client_area); }

	CL_String get_clipboard_text() const { return x11_window.get_clipboard_text(); }

	CL_PixelBuffer get_clipboard_image() const { return x11_window.get_clipboard_image(); }

	/// \brief Returns the X11 display handle.
	Display *get_display() const { return x11_window.get_display(); }

	/// \brief Handle to X11 window handle.
	Window get_window() const { return x11_window.get_window(); }

	/// \brief Returns the GLX rendering context for this window.
	GLXContext get_opengl_context() { return opengl_context; }

	CL_GraphicContext& get_gc() { return gc; }

	CL_InputContext& get_ic() { return x11_window.get_ic(); }

	CL_GraphicContext gc;

	CL_GL_GLXFunctions glx;

/// \}
/// \name Operations
/// \{

public:
	CL_Point client_to_screen(const CL_Point &client) { return x11_window.client_to_screen(client); }

	CL_Point screen_to_client(const CL_Point &screen) { return x11_window.screen_to_client(screen); }

	void create(CL_DisplayWindowSite *site, const CL_DisplayWindowDescription &description);

	void show_system_cursor() { x11_window.show_system_cursor(); }

	CL_CursorProvider *create_cursor(const CL_SpriteDescription &sprite_description, const CL_Point &hotspot);

	void set_cursor(CL_CursorProvider *cursor);

	void set_cursor(CL_StandardCursor type) { x11_window.set_cursor(type); }

	void hide_system_cursor()  { x11_window.hide_system_cursor(); }

	void set_title(const CL_StringRef &new_title) { x11_window.set_title(new_title); }

	void set_position(const CL_Rect &pos, bool client_area) { return x11_window.set_position(pos, client_area); };

	void set_size(int width, int height, bool client_area)  { return x11_window.set_size(width, height, client_area); }

	void set_minimum_size(int width, int height, bool client_area) { return x11_window.set_minimum_size(width, height, client_area); }

	void set_maximum_size( int width, int height, bool client_area) { return x11_window.set_maximum_size(width, height, client_area); }

	void set_enabled(bool enable) { return x11_window.set_enabled(enable); }

	void minimize() { x11_window.minimize(); }

	void restore() { x11_window.restore(); }

	void maximize() { x11_window.maximize(); }

	void show(bool activate)  { x11_window.show(activate); }

	void hide() { x11_window.hide(); }

	void bring_to_front() { x11_window.bring_to_front(); }

	/// \brief Flip opengl buffers.
	void flip(int interval);

	/// \brief Copy a region of the backbuffer to the frontbuffer.
	void update(const CL_Rect &rect);

	/// \brief Capture/Release the mouse.
	void capture_mouse(bool capture) { x11_window.capture_mouse(capture); }

	void process_messages();

	GLXContext create_context(CL_OpenGLWindowDescription &gl_desc);

	/// \brief Check for window messages
	/** \return true when there is a message*/
	bool has_messages() { return x11_window.has_messages(); }

	void set_clipboard_text(const CL_StringRef &text) { x11_window.set_clipboard_text(text); }

	void set_clipboard_image(const CL_PixelBuffer &buf) { x11_window.set_clipboard_image(buf); }

	void request_repaint(const CL_Rect &rect) { x11_window.request_repaint(rect); }

	void set_large_icon(const CL_PixelBuffer &image);
	void set_small_icon(const CL_PixelBuffer &image);

/// \}
/// \name Implementation
/// \{

private:

	GLXContext create_context_glx_1_3(CL_OpenGLWindowDescription &gl_desc, GLXContext shared_context);
	GLXContext create_context_glx_1_2(CL_OpenGLWindowDescription &gl_desc, GLXContext shared_context);
	void create_glx_1_3(CL_DisplayWindowSite *new_site, const CL_DisplayWindowDescription &desc, Display *disp);
	void create_glx_1_2(CL_DisplayWindowSite *new_site, const CL_DisplayWindowDescription &desc, Display *disp);

	void on_window_resized();

	bool is_glx_extension_supported(const char *ext_name);

	void setup_swap_interval_pointers();

	CL_X11Window x11_window;

	/// \brief GLX rendering context handle.
	GLXContext opengl_context;

	CL_DisplayWindowSite *site;

	XVisualInfo *opengl_visual_info;

	ptr_glXSwapIntervalSGI glXSwapIntervalSGI;
	ptr_glXSwapIntervalMESA glXSwapIntervalMESA;
	int swap_interval;

	GLXFBConfig fbconfig;

#ifdef CL_USE_DLOPEN
	void *opengl_lib_handle;
#endif
	bool glx_1_3;

/// \}
};



