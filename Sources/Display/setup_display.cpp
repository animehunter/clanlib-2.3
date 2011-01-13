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

#include "Display/precomp.h"
#include "API/Display/setup_display.h"
#include "API/Display/ImageProviders/provider_type_register.h"
#include "API/Display/ImageProviders/pcx_provider.h"
#include "API/Display/ImageProviders/targa_provider.h"
#include "API/Display/ImageProviders/jpeg_provider.h"
#include "API/Display/ImageProviders/png_provider.h"

#ifndef WIN32
#include <X11/Xlib.h>
#endif

static CL_ProviderType_Register<CL_JPEGProvider> *jpeg_provider = NULL;
static CL_ProviderType_Register<CL_JPEGProvider> *jpg_provider = NULL;
static CL_ProviderType_Register<CL_PCXProvider> *pcx_provider = NULL;
static CL_ProviderType_Register<CL_PNGProvider> *png_provider = NULL;
static CL_ProviderType_Register<CL_TargaProvider> *targa_provider = NULL;
static CL_ProviderType_Register<CL_TargaProvider> *tga_provider = NULL;

/////////////////////////////////////////////////////////////////////////////
// CL_SetupDisplay Construction:

CL_SetupDisplay::CL_SetupDisplay()
{
#ifndef WIN32
	// The XInitThreads() function initializes Xlib support for concurrent threads.
	// This function must be the first Xlib function a multi-threaded program calls, and it must complete before any other Xlib call is made.
	XInitThreads();
#endif
	jpeg_provider  = new CL_ProviderType_Register<CL_JPEGProvider>("jpeg");
	jpg_provider   = new CL_ProviderType_Register<CL_JPEGProvider>("jpg");
	pcx_provider   = new CL_ProviderType_Register<CL_PCXProvider>("pcx");
	png_provider   = new CL_ProviderType_Register<CL_PNGProvider>("png");
	targa_provider = new CL_ProviderType_Register<CL_TargaProvider>("targa");
	tga_provider   = new CL_ProviderType_Register<CL_TargaProvider>("tga");
}

CL_SetupDisplay::~CL_SetupDisplay()
{
	delete pcx_provider;
	pcx_provider = NULL;

	delete jpeg_provider;
	jpeg_provider = NULL;

	delete jpg_provider;
	jpg_provider = NULL;

	delete png_provider;
	png_provider = NULL;

	delete targa_provider;
	targa_provider = NULL;

	delete tga_provider;
	tga_provider = NULL;
}
