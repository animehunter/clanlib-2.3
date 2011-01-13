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
**    Harry Storbacka
**    Magnus Norddahl
**    Mark Page
**    (if your name is missing here, please add it)
*/

#include "Display/precomp.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/IOData/virtual_directory.h"
#include "API/Core/IOData/path_help.h"
#include "API/Display/Collision/outline_provider_file.h"
#include "API/Display/Collision/outline_circle.h"
#include "outline_provider_file_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_OutlineProviderFile Construction:

CL_OutlineProviderFile::CL_OutlineProviderFile(CL_IODevice &file)
{
	impl = CL_SharedPtr<CL_OutlineProviderFile_Generic> (new CL_OutlineProviderFile_Generic( file ));
}

CL_OutlineProviderFile::CL_OutlineProviderFile(const CL_StringRef &fullname)
{
	CL_String path = CL_PathHelp::get_fullpath(fullname, CL_PathHelp::path_type_file);
	CL_String filename = CL_PathHelp::get_filename(fullname, CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs(path);
	CL_IODevice file = vfs.get_root_directory().open_file_read(filename);
	impl = CL_SharedPtr<CL_OutlineProviderFile_Generic> (new CL_OutlineProviderFile_Generic( file ));
}

CL_OutlineProviderFile::CL_OutlineProviderFile(const CL_StringRef &filename, const CL_VirtualDirectory &directory)
{
	CL_IODevice file = directory.open_file_read(filename);
	impl = CL_SharedPtr<CL_OutlineProviderFile_Generic> (new CL_OutlineProviderFile_Generic( file ));
}

CL_OutlineProviderFile::~CL_OutlineProviderFile()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_OutlineProviderFile Attributes:

std::vector<CL_Contour> CL_OutlineProviderFile::get_contours()
{
	return impl->contours;
}

int CL_OutlineProviderFile::get_width()
{
	return impl->width;
}

int CL_OutlineProviderFile::get_height()
{
	return impl->height;
}

void CL_OutlineProviderFile::destroy()
{
	delete this;
}
