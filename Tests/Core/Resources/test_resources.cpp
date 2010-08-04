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
**    Mark Page
**    (if your name is missing here, please add it)
*/

#include "test.h"

void TestApp::test_resources(void)
{
	CL_Console::write_line(" Header: resource.h");
	CL_Console::write_line("  Class: CL_Resource");

	// Construct resource manager. This will cause sig_resource_added() to be signalled for each
	// resource object located in the resource file. This means ResourceApp::on_resource_added will
	// get called for each resource.
	CL_ResourceManager resources("resources.xml");

	// Lets try to access some of the clanlib objects in the resource file:
	CL_String config_name = resources.get_string_resource("Configuration/name", "");
	int config_width = resources.get_integer_resource("Configuration/width", 0 );
	int config_height = resources.get_integer_resource("Configuration/height", 0 );

	CL_Console::write_line(CL_String("name = ") + config_name );
	CL_Console::write_line(CL_String("width = ") + CL_StringHelp::int_to_text(config_width));
	CL_Console::write_line(CL_String("height = ") + CL_StringHelp::int_to_text(config_height));

	// Get a list over all classes
	std::vector<CL_String> v = resources.get_resource_names_of_type("uclass", "Classes");
	std::vector<CL_String>::iterator it;
	for(it = v.begin(); it != v.end(); ++it)
	{
		// Get one of our custom resources:
		CL_Resource resource = resources.get_resource(*it);

		// Its possible to access the dom element object in the resource:
		CL_DomElement element = resource.get_element();
		int strength = CL_StringHelp::text_to_int(element.get_attribute("strength"));
		int magic = CL_StringHelp::text_to_int(element.get_attribute("magic"));

		CL_Console::write_line(CL_String("\n") + "name = " + resource.get_name());
		CL_Console::write_line(CL_String("strength = ") + CL_StringHelp::int_to_text(strength));
		CL_Console::write_line(CL_String("magic = ") + CL_StringHelp::int_to_text(magic) );
	}

}

