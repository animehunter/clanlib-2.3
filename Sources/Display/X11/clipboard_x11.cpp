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
**    Mark Page
*/

#include "Display/precomp.h"
#include "input_device_provider_x11keyboard.h"
#include "x11_window.h"
#include <X11/Xatom.h>

/////////////////////////////////////////////////////////////////////////////
// CL_Clipboard_X11 construction:

CL_Clipboard_X11::CL_Clipboard_X11(CL_X11Window *x11_window)
:x11_window(x11_window), atom_CLIPBOARD(None), clipboard_available(false)
{
}

CL_Clipboard_X11::~CL_Clipboard_X11()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Clipboard_X11 attributes:

CL_String CL_Clipboard_X11::get_clipboard_text() const
{
	if ( ( atom_CLIPBOARD == None ) )
	{
		// X Server does not have clipboard support
		return CL_String();
	}

	XConvertSelection(disp, atom_CLIPBOARD, XA_STRING, atom_CLIPBOARD, window, CurrentTime);
	XFlush(disp);

	XEvent event;
	if (!x11_window->get_xevent( event, SelectionNotify ))
	{
		return CL_String();
	}

	Atom actual_type;
	int actual_format;
	unsigned long number_items;
	unsigned char *read_data = x11_window->get_property(window, atom_CLIPBOARD, &number_items, &actual_format, &actual_type);

	if ( (actual_format != 8) || (actual_type != XA_STRING) || (number_items <=0) || (read_data==NULL) )
	{
		if (read_data)
		{
			XFree(read_data);
		}
		return CL_String();
	}

	CL_String buffer( (char *) read_data);
	XFree(read_data);
	return buffer;
}

bool CL_Clipboard_X11::is_clipboard_text_available() const
{
	if (get_clipboard_text().size() > 0)
	{
		return true;
	}

	return false;
}

bool CL_Clipboard_X11::is_clipboard_image_available() const
{
	return false;
//	throw Exception("CL_Clipboard_X11::is_clipboard_image_available() not implemented");
}

/////////////////////////////////////////////////////////////////////////////
// CL_Clipboard_X11 operations:

void CL_Clipboard_X11::setup()
{
	clipboard_available = false;
	disp = x11_window->get_display();
	window = x11_window->get_window();
	atom_CLIPBOARD = XInternAtom(disp, "CLIPBOARD", False);

}

void CL_Clipboard_X11::event_selection_clear(XSelectionClearEvent &xselectionclear)
{
	clipboard_available = false;
	clipboard_current = CL_String();
}

void CL_Clipboard_X11::event_selection_notify()
{
	// Data was sent to use (this is handled in get_clipboard_text() )
}

void CL_Clipboard_X11::event_selection_request(XSelectionRequestEvent &xselectionrequest)
{
	XSelectionRequestEvent *rptr = &xselectionrequest;
	if (rptr->requestor == window) return;	// Ignore request if from self

	XEvent new_event;
	new_event.xselection.type = SelectionNotify;
	new_event.xselection.display = rptr->display;
	new_event.xselection.requestor = rptr->requestor;
	new_event.xselection.selection = rptr->selection;
	new_event.xselection.target = rptr->target;
	new_event.xselection.property = None;
	new_event.xselection.time = rptr->time;

	Atom xa_targets = XInternAtom(disp, "TARGETS", False);
	Atom xa_multiple = XInternAtom(disp, "MULTIPLE", False);

	struct AtomPair { Atom target; Atom property; } *multi_ptr = NULL;

	unsigned char *data_ptr = NULL;
	int num_multi = 0;
	int index_multi = -1;
	if (rptr->target == xa_multiple)
	{
		unsigned long number_items;
		int actual_format;
		Atom actual_type;
		data_ptr = x11_window->get_property( rptr->requestor, rptr->property, &number_items, &actual_format, &actual_type );
		if ( data_ptr )
		{
			num_multi = number_items / 2;
			multi_ptr = (struct AtomPair *) data_ptr;
		}
		index_multi = 0;
	}

	while( index_multi < num_multi)
	{
		Window xtarget;
		Atom xproperty;
		new_event.xselection.property = None;
		if (multi_ptr)
		{
			xtarget = multi_ptr[index_multi].target;
			xproperty = multi_ptr[index_multi].property;
			index_multi++;
		}else
		{
			xtarget = rptr->target;
			xproperty = rptr->property;
		}

		if (xtarget == xa_targets)
		{
			long new_targets[1];
			new_targets[0] = XA_STRING;
			XChangeProperty( disp, rptr->requestor, xproperty, xa_targets, 32, PropModeReplace, (unsigned char *) new_targets, 1);
			new_event.xselection.property = xproperty;
		}else
		{

			if (xtarget == XA_STRING)
			{
				XChangeProperty(disp, rptr->requestor, xproperty, xtarget, 8, PropModeReplace, (const unsigned char*) clipboard_current.c_str(), clipboard_current.size());
				new_event.xselection.property = xproperty;

			}
		}
		XSendEvent( disp, rptr->requestor, False, 0, &new_event );
		if (!num_multi) break;
	}
	if (data_ptr)
	{
		XFree(data_ptr);
	}
}

void CL_Clipboard_X11::set_clipboard_text(const CL_StringRef &text)
{

	// **** README ****
	// The clipboard only works while the message queue is being processed as get_message() processes clipboard (selection) events

	if ( ( atom_CLIPBOARD == None ) )
	{
		// X Server does not have clipboard support
		return;
	}
	clipboard_current = text;
	clipboard_available = true;

	XSetSelectionOwner(disp, XA_PRIMARY, window, CurrentTime );
	XSetSelectionOwner(disp, atom_CLIPBOARD, window, CurrentTime );
}

/////////////////////////////////////////////////////////////////////////////
// CL_Clipboard_X11 implementation:


