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

#include "Core/precomp.h"
#include "API/Core/Zip/zip_file_entry.h"
#include "zip_file_entry_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ZipFileEntry construction:

CL_ZipFileEntry::CL_ZipFileEntry()
: impl(new CL_ZipFileEntry_Impl)
{
	impl->type = CL_ZipFileEntry_Impl::type_file;
	impl->is_directory = false;
}
	
CL_ZipFileEntry::CL_ZipFileEntry(const CL_ZipFileEntry &copy)
: impl(copy.impl)
{
}
	
CL_ZipFileEntry::~CL_ZipFileEntry()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ZipFileEntry attributes:

CL_StringRef CL_ZipFileEntry::get_input_filename() const
{
	return impl->filename;
}

CL_StringRef CL_ZipFileEntry::get_archive_filename() const
{
	return impl->record.filename;
}

cl_byte64 CL_ZipFileEntry::get_uncompressed_size()
{
	return impl->record.uncompressed_size;
}
	
cl_byte64 CL_ZipFileEntry::get_compressed_size()
{
	return impl->record.compressed_size;
}

bool CL_ZipFileEntry::is_directory() const
{
	return impl->is_directory;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ZipFileEntry operations:

CL_ZipFileEntry &CL_ZipFileEntry::operator =(const CL_ZipFileEntry &copy)
{
	impl = copy.impl;
	return *this;
}
	
void CL_ZipFileEntry::set_input_filename(const CL_StringRef &filename)
{
	impl->filename = filename;
}

void CL_ZipFileEntry::set_archive_filename(const CL_StringRef &filename)
{
	impl->record.file_name_length = filename.length();
	impl->record.filename = filename;
}

void CL_ZipFileEntry::set_directory( bool is_directory )
{
	impl->is_directory = is_directory;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ZipFileEntry implementation:
