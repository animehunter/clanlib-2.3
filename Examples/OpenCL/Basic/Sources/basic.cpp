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

#include "precomp.h"
#include "basic.h"

#undef clFlush
#undef clFinish
#include <CL/opencl.h>
//#include <CL/cl.hpp>

#pragma comment(lib, "C:\\Program Files (x86)\\AMD APP\\lib\\x86\\OpenCL.lib")

// The start of the Application
int Basic::start(const std::vector<CL_String> &args)
{
	quit = false;

	// Set the window
	CL_DisplayWindowDescription desc;
	desc.set_title("ClanLib OpenCL Basic Example");
	desc.set_size(CL_Size(640, 480), true);
	desc.set_allow_resize(true);

	CL_DisplayWindow window(desc);

	// Connect the Window close event
	CL_Slot slot_quit = window.sig_window_close().connect(this, &Basic::on_window_close);

	// Connect a keyboard handler to on_key_up()
	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &Basic::on_input_up);

	// Get the graphic context
	CL_GraphicContext gc = window.get_gc();


	cl_int error;
	cl_platform_id platform;
	cl_uint platforms;
	
	error=clGetPlatformIDs(1, &platform, &platforms);
	if (error != CL_SUCCESS)
		throw CL_Exception("clGetPlatformIDs failed");

	cl_device_id device;
	cl_uint devices;
	error=clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, &devices);
	if (error != CL_SUCCESS)
		throw CL_Exception("clGetDeviceIDs failed");


	cl_context_properties properties[]={CL_CONTEXT_PLATFORM, (cl_context_properties)platform, 0};
	cl_context context;
	context=clCreateContext(properties, 1, &device, NULL, NULL, &error);
	if (error != CL_SUCCESS)
		throw CL_Exception("clCreateContext failed");

	// Clean up
	clReleaseContext(context);

	CL_Font font(gc, "tahoma", 24);

	// Run until someone presses escape
	while (!quit)
	{
		gc.clear(CL_Colorf(0.0f,0.0f,0.2f));
		font.draw_text(gc, 32, 32, "Success");
		window.flip(1);
		CL_KeepAlive::process(0);
	}

	return 0;
}

// A key was pressed
void Basic::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
	{
		quit = true;
	}
}

// The window was closed
void Basic::on_window_close()
{
	quit = true;
}


