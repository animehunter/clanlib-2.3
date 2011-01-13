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

#pragma once

#include "API/Core/Zip/zip_file_entry.h"
#include "API/Core/IOData/iodevice.h"
#include "zip_flags.h"

class CL_ZipArchive_Impl
{
/// \name Construction
/// \{

public:


/// \}
/// \name Attributes
/// \{

public:
	std::vector<CL_ZipFileEntry> files;

	CL_IODevice input;


/// \}
/// \name Operations
/// \{

public:
	static cl_uint32 calc_crc32(const void *data, cl_int64 size, cl_uint32 crc = CL_ZIP_CRC_START_VALUE, bool last_block = true);

	static void calc_time_and_date(cl_int16 &out_date, cl_int16 &out_time);


/// \}
/// \name Implementation
/// \{

private:
	// crc32_table_quotient = 0xdebb20e3
	static cl_uint32 crc32_table[256];
/// \}
};


