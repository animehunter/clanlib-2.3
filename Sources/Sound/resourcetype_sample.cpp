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
*/

#include "Sound/precomp.h"

#ifdef _MSC_VER
#pragma warning (disable:4786)
#endif

#include "resourcetype_sample.h"
#include "API/Core/System/exception.h"
#include "API/Core/Resources/resource.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/IOData/virtual_directory.h"
#include "API/Core/XML/dom_element.h"
#include "API/Sound/soundbuffer.h"
#include "API/Sound/SoundProviders/soundprovider.h"
#include "API/Sound/SoundProviders/soundprovider_wave.h"
#include "API/Sound/SoundProviders/soundprovider_factory.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Sample Construction:

CL_ResourceData_Sample::CL_ResourceData_Sample(class CL_Resource &resource)
{
	CL_DomElement &element = resource.get_element();

	CL_SoundProvider *provider = 0;

	CL_String name = resource.get_element().get_attribute("file");
	CL_String sound_format = resource.get_element().get_attribute("format");
	bool streamed = (element.get_attribute("stream", "no") == "yes");

	provider = CL_SoundProviderFactory::load(
		name,
		streamed,
		resource.get_manager().get_directory(resource), sound_format);

	if (provider)
	{
		soundbuffer = new CL_SoundBuffer(provider);
	}
	else
	{
		throw CL_Exception("Unknown sample format");
	}
}

CL_ResourceData_Sample::~CL_ResourceData_Sample()
{
	delete soundbuffer;
	soundbuffer = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Sample Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Sample Implementation:
