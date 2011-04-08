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
#include "API/Core/Math/rect.h"
#include "API/Core/Math/point.h"
#include "API/Core/Text/logger.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/IOData/iodevice_memory.h"
#include "API/Display/Window/display_window_description.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/display.h"
#include "API/Display/2D/color.h"
#include "API/Display/display_target.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Window/keys.h"
#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/ImageProviders/png_provider.h"
#include "API/Display/screen_info.h"
#include "cocoa_window.h"
#include "cocoa_message_queue.h"
#include "Display/Window/input_context_impl.h"

CL_CocoaWindow::CL_CocoaWindow()
: site(0), window(0), controller(0)
{
    CL_CocoaMessageQueue *queue = CL_CocoaMessageQueue::get();
}

CL_CocoaWindow::~CL_CocoaWindow()
{
    if (controller)
        [controller release];
}

CL_Rect CL_CocoaWindow::get_geometry() const
{
    return CL_Rect();
}

CL_Rect CL_CocoaWindow::get_viewport() const
{
    return CL_Rect(0, 0, 768, 1024);
}

bool CL_CocoaWindow::has_focus() const
{
    return true;
}

bool CL_CocoaWindow::is_minimized() const
{
    return false;
}

bool CL_CocoaWindow::is_maximized() const
{
    return false;
}

bool CL_CocoaWindow::is_fullscreen() const
{
    return false;
}

bool CL_CocoaWindow::is_visible() const
{
    return true;
}

CL_Size CL_CocoaWindow::get_minimum_size(bool client_area) const
{
    return CL_Size();
}

CL_Size CL_CocoaWindow::get_maximum_size(bool client_area) const
{
    return CL_Size();
}

CL_String CL_CocoaWindow::get_title() const
{
    return CL_String();
}

bool CL_CocoaWindow::is_clipboard_text_available() const
{
    return false;
}

bool CL_CocoaWindow::is_clipboard_image_available() const
{
    return false;
}

CAEAGLLayer *CL_CocoaWindow::get_drawable()
{
    return (CAEAGLLayer*)[[controller view] layer];
}

void CL_CocoaWindow::create(CL_DisplayWindowSite *new_site, const CL_DisplayWindowDescription &description)
{
    site = new_site;
    if (!window)
    {
        window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
        controller = [[CL_CocoaController alloc] init];
        //UINavigationController *nav_controller = [[UINavigationController alloc] initWithRootViewController:controller];
        window.rootViewController = controller;
        //[window addSubview: nav_controller.view];
        [window makeKeyAndVisible];
    }
}

CL_Point CL_CocoaWindow::client_to_screen(const CL_Point &client)
{
    return client;
}

CL_Point CL_CocoaWindow::screen_to_client(const CL_Point &screen)
{
    return screen;
}

void CL_CocoaWindow::show_system_cursor()
{
    
}

void CL_CocoaWindow::set_cursor(CL_CursorProvider_Cocoa *cursor)
{
    
}

void CL_CocoaWindow::set_cursor(enum CL_StandardCursor type)
{
    
}

void CL_CocoaWindow::hide_system_cursor()
{
    
}

void CL_CocoaWindow::set_large_icon(const CL_PixelBuffer &image)
{
    
}

void CL_CocoaWindow::set_small_icon(const CL_PixelBuffer &image)
{
    
}

void CL_CocoaWindow::set_title(const CL_StringRef &new_title)
{
    
}

void CL_CocoaWindow::set_enabled(bool enable)
{
    
}

void CL_CocoaWindow::set_position(const CL_Rect &pos, bool client_area)
{
    
}

void CL_CocoaWindow::set_size(int width, int height, bool client_area)
{
    
}

void CL_CocoaWindow::set_minimum_size(int width, int height, bool client_area)
{
    
}

void CL_CocoaWindow::set_maximum_size(int width, int height, bool client_area)
{
    
}

void CL_CocoaWindow::minimize()
{
    
}

void CL_CocoaWindow::restore()
{
    
}

void CL_CocoaWindow::maximize()
{
    
}

void CL_CocoaWindow::show(bool activate)
{
    
}

void CL_CocoaWindow::hide()
{
    
}

void CL_CocoaWindow::bring_to_front()
{
    
}

void CL_CocoaWindow::capture_mouse(bool capture)
{
    
}

void CL_CocoaWindow::set_clipboard_text(const CL_StringRef &text)
{
    
}

CL_String CL_CocoaWindow::get_clipboard_text() const
{
    return CL_String();
}

CL_PixelBuffer CL_CocoaWindow::get_clipboard_image() const
{
    return CL_PixelBuffer();
}

void CL_CocoaWindow::set_clipboard_image(const CL_PixelBuffer &image)
{
    
}

void CL_CocoaWindow::request_repaint(const CL_Rect &rect)
{
    
}

void CL_CocoaWindow::set_modifier_keys(CL_InputEvent &key)
{
    
}
