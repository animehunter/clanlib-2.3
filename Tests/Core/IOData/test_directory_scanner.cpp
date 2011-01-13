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
**    (if your name is missing here, please add it)
*/

#include "test.h"

void TestApp::test_directory_scanner(void)
{
	CL_Console::write_line(" Header: directory_scanner.h");
	CL_Console::write_line("  Class: CL_DirectoryScanner");

	CL_String str;
	CL_String str2;
	CL_DirectoryScanner scanner;
	bool result;
	int size;
	int cnt;

//*** testing get_directory_path()
	CL_Console::write_line("   Function: CL_String get_directory_path()");
	result = scanner.scan("..");
	if (!result) fail();
	str = scanner.get_directory_path();
#ifdef WIN32
	str2 = "..\\";
#else
	str2 = "../";
#endif
	if (str != str2) fail();

//*** testing get_name()
	CL_Console::write_line("   Function: CL_String get_name()");
	result = scanner.scan("", "Makefile");
	if (!result) fail();
	result = scanner.next();
	if (!result) fail();
	str = scanner.get_name();
	str2 = "Makefile";
	if (str != str2) fail();

//*** testing get_size()
	CL_Console::write_line("   Function: int get_size()");

	// Create a test file
	CL_String filename = "temporary_file.tmp";
	CL_File output_file(filename, CL_File::create_always, CL_File::access_write);
	char buffer[123];
	output_file.write(buffer, 123);
	output_file.close();

	result = scanner.scan("", filename);
	if (!result) fail();
	result = scanner.next();
	if (!result) fail();
	size = scanner.get_size();
	CL_FileHelp::delete_file(filename);
	if (size != 123) fail();

//*** testing get_pathname()
	CL_Console::write_line("   Function: CL_String get_pathname()");
	result = scanner.scan(".", "Makefile");
	if (!result) fail();
	result = scanner.next();
	if (!result) fail();
	str = scanner.get_pathname();
#ifdef WIN32
	str2 = ".\\Makefile";
#else
	str2 = "./Makefile";
#endif
	if (str != str2) fail();

//*** testing is_directory()
	CL_Console::write_line("   Function: CL_String is_directory()");
	result = scanner.scan("..", "IOData");
	if (!result) fail();
	result = scanner.next();
	if (!result) fail();
	str = scanner.get_name();
	str2 = "IOData";
	if (str != str2) fail();
	result = scanner.is_directory();
	if (!result) fail();

	result = scanner.scan(".", "Makefile");
	if (!result) fail();
	result = scanner.next();
	if (!result) fail();
	str = scanner.get_name();
	str2 = "Makefile";
	if (str != str2) fail();
	result = scanner.is_directory();
	if (result) fail();

//*** testing is_hidden()
	CL_Console::write_line("   Function: CL_String is_hidden()");

	result = scanner.scan(".", "Makefile");
	if (!result) fail();
	result = scanner.next();
	if (!result) fail();
	result = scanner.is_hidden();
	if (result) fail();

	// Create a test file

#ifdef WIN32
	CL_Console::write_line("     *** Unable to fully test on WIN32 ***");
#else
	filename = ".temporary_file.tmp";
	output_file.open(filename, CL_File::create_always, CL_File::access_write);
	output_file.close();

	result = scanner.scan("", filename);
	if (!result) fail();
	result = scanner.next();
	if (!result) fail();
	result = scanner.is_hidden();
	CL_FileHelp::delete_file(filename);
	if (!result) fail();
#endif

//*** testing is_readable()
	CL_Console::write_line("   Function: CL_String is_readable()");

	result = scanner.scan(".", "Makefile");
	if (!result) fail();
	result = scanner.next();
	if (!result) fail();
	result = scanner.is_readable();
	if (!result) fail();

	// Create a test file

#ifdef WIN32
	CL_Console::write_line("     *** Unable to fully test on WIN32 ***");
#else
	filename = "temporary_file.tmp";
	CL_String8 filename_a = CL_StringHelp::text_to_local8(filename);
	int handle = ::open(filename_a.c_str(), O_WRONLY | O_CREAT, S_IWUSR);
	if (handle == -1) fail();
	::close(handle);

	result = scanner.scan(".", filename);
	if (!result) fail();
	result = scanner.next();
	if (!result) fail();
	result = scanner.is_readable();
	if (result) fail();
	result = scanner.is_writable();
	if (!result) fail();
	CL_FileHelp::delete_file(filename);
#endif
//*** testing is_writable()
	CL_Console::write_line("   Function: CL_String is_writable()");

	result = scanner.scan(".", "Makefile");
	if (!result) fail();
	result = scanner.next();
	if (!result) fail();
	result = scanner.is_writable();
	if (!result) fail();

	// Create a test file
#ifdef WIN32
	CL_Console::write_line("     *** Unable to fully test on WIN32 ***");
#else
	filename = "temporary_file.tmp";
	filename_a = CL_StringHelp::text_to_local8(filename);
	handle = ::open(filename_a.c_str(), O_WRONLY | O_CREAT, S_IRUSR);
	if (handle == -1) fail();
	::close(handle);

	result = scanner.scan(".", filename);
	if (!result) fail();
	result = scanner.next();
	if (!result) fail();
	result = scanner.is_readable();
	if (!result) fail();
	result = scanner.is_writable();
	if (result) fail();
	CL_FileHelp::delete_file(filename);
#endif

//*** testing scan(const CL_String& pathname)
	CL_Console::write_line("   Function: bool scan(const CL_String& pathname)");
	result = scanner.scan("..");
	if (!result) fail();
	str2 = "IOData";
	do
	{
		result = scanner.next();
		if (!result) fail();
		str = scanner.get_name();

	}while(str != str2);
	result = scanner.is_directory();
	if (!result) fail();

//*** testing scan(const CL_String& pathname, const CL_String& pattern)
	CL_Console::write_line("   Function: bool scan(const CL_String& pathname, const CL_String& pattern)");
	result = scanner.scan("..", "IOData");
	if (!result) fail();
	str2 = "IOData";
	result = scanner.next();
	if (!result) fail();
	str = scanner.get_name();
	if (str != str2) fail();
	result = scanner.is_directory();
	if (!result) fail();
	result = scanner.scan("", "*dir*c*ry_sca?ner*.cp?");
	if (!result) fail();
	result = scanner.next();
	if (!result) fail();
	str2 = "test_directory_scanner.cpp";
	str = scanner.get_name();
	if (str != str2) fail();

//*** testing next()
	CL_Console::write_line("   Function: bool next()");
	result = scanner.scan("");
	if (!result) fail();
	str2 = "test_directory_scanner.cpp";
	do
	{
		result = scanner.next();
		if (!result) fail();
		str = scanner.get_name();

	}while(str != str2);

	for (cnt=999; cnt>0; cnt--)
	{
		result = scanner.next();
		if (!result) break;

	}
	if (!cnt) fail();	// scanner.next() still scanning

//	CL_Console::write_line(str);

}

