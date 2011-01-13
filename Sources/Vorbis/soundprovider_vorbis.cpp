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
**    (if your name is missing here, please add it)
*/

#include "Sound/precomp.h"
#include "API/Vorbis/soundprovider_vorbis.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/IOData/virtual_directory.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/IOData/path_help.h"
#include "soundprovider_vorbis_impl.h"
#include "soundprovider_vorbis_session.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Vorbis construction:

CL_SoundProvider_Vorbis::CL_SoundProvider_Vorbis(
	const CL_String &filename,
	const CL_VirtualDirectory &directory,
	bool stream)
: impl(new CL_SoundProvider_Vorbis_Impl)
{
	CL_VirtualDirectory new_directory = directory;
	CL_IODevice input = new_directory.open_file(filename, CL_File::open_existing, CL_File::access_read, CL_File::share_all);
	impl->load(input);
}

CL_SoundProvider_Vorbis::CL_SoundProvider_Vorbis(
	const CL_String &fullname, bool stream)
: impl(new CL_SoundProvider_Vorbis_Impl)
{
	CL_String path = CL_PathHelp::get_fullpath(fullname, CL_PathHelp::path_type_file);
	CL_String filename = CL_PathHelp::get_filename(fullname, CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs(path);
	CL_VirtualDirectory dir = vfs.get_root_directory();
	CL_IODevice input = dir.open_file(filename, CL_File::open_existing, CL_File::access_read, CL_File::share_all);
	impl->load(input);
}

CL_SoundProvider_Vorbis::CL_SoundProvider_Vorbis(
	CL_IODevice &file, bool stream)
: impl(new CL_SoundProvider_Vorbis_Impl)
{
	impl->load(file);
}

CL_SoundProvider_Vorbis::~CL_SoundProvider_Vorbis()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Vorbis operations:

CL_SoundProvider_Session *CL_SoundProvider_Vorbis::begin_session()
{
	return new CL_SoundProvider_Vorbis_Session(*this);
}

void CL_SoundProvider_Vorbis::end_session(CL_SoundProvider_Session *session)
{
	delete session;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Vorbis implementation:

void CL_SoundProvider_Vorbis_Impl::load(CL_IODevice &input)
{
	int size = input.get_size();
	buffer = CL_DataBuffer(size);
	int bytes_read = input.read(buffer.get_data(), buffer.get_size());
	buffer.set_size(bytes_read);
}
