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
**    (if your name is missing here, please add it)
*/

#include "Sound/precomp.h"
#include "API/Sound/cd_drive.h"
#include "cd_drive_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_CDDrive construction:

CL_CDDrive::CL_CDDrive()
{
}

CL_CDDrive::~CL_CDDrive()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_CDDrive attributes:

std::vector<CL_CDDrive> &CL_CDDrive::get_drives()
{
	static std::vector<CL_CDDrive> drives;
	return drives;
}

void CL_CDDrive::throw_if_null() const
{
	if (impl.is_null())
		throw CL_Exception("CL_CDDrive is null");
}

CL_String8 CL_CDDrive::get_drive_path()
{
	if (impl) return impl->get_drive_path();
	return CL_String8();
}

CL_String8 CL_CDDrive::get_cd_name()
{
	if (impl) return impl->get_cd_name();
	return CL_String8();
}

int CL_CDDrive::get_num_tracks()
{
	if (impl) return impl->get_num_tracks();
	return 0;
}

bool CL_CDDrive::is_playing()
{
	if (impl) return impl->is_playing();
	return false;
}

int CL_CDDrive::get_cur_track()
{
	if (impl) return impl->get_cur_track();
	return 0;
}

int CL_CDDrive::get_cur_frame()
{
	if (impl) return impl->get_cur_frame();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_CDDrive operations:

bool CL_CDDrive::play_tracks(int track, int end_track)
{
	if (impl) return impl->play_tracks(track, end_track);
	return false;
}

bool CL_CDDrive::play_frames(int frame, int end_frame)
{
	if (impl) return impl->play_frames(frame, end_frame);
	return false;
}

bool CL_CDDrive::play_track(int track)
{
	if (impl) return impl->play_track(track);
	return false;
}

void CL_CDDrive::stop()
{
	if (impl) impl->stop();
}

void CL_CDDrive::pause()
{
	if (impl) impl->pause();
}

void CL_CDDrive::resume()
{
	if (impl) impl->resume();
}

/////////////////////////////////////////////////////////////////////////////
// CL_CDDrive implementation:
