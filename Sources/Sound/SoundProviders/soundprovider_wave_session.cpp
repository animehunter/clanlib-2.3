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

#include "Sound/precomp.h"
#include "soundprovider_wave_session.h"
#include "soundprovider_wave_impl.h"
#include "API/Sound/soundformat.h"
#include "API/Sound/sound_sse.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Wave_Session construction:

CL_SoundProvider_Wave_Session::CL_SoundProvider_Wave_Session(CL_SoundProvider_Wave &source) :
	source(source), position(0)
{
	frequency = source.impl->frequency;
	end_position = num_samples = source.impl->num_samples;
}

CL_SoundProvider_Wave_Session::~CL_SoundProvider_Wave_Session()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Wave_Session attributes:

int CL_SoundProvider_Wave_Session::get_num_samples() const
{
	return num_samples;
}

int CL_SoundProvider_Wave_Session::get_frequency() const
{
	return frequency;
}

int CL_SoundProvider_Wave_Session::get_num_channels() const
{
	return source.impl->num_channels;
}

int CL_SoundProvider_Wave_Session::get_position() const
{
	return position;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Wave_Session operations:

bool CL_SoundProvider_Wave_Session::eof() const
{
	return (position >= end_position);
}

void CL_SoundProvider_Wave_Session::stop()
{
}

bool CL_SoundProvider_Wave_Session::play()
{
	return true;
}

bool CL_SoundProvider_Wave_Session::set_position(int pos)
{
	position = pos;
	return true;
}

bool CL_SoundProvider_Wave_Session::set_end_position(int pos)
{
	if (pos > num_samples)
		throw CL_Exception("Attempted to set the sample end position higher than the number of samples");
	end_position = pos;
	return true;
}


int CL_SoundProvider_Wave_Session::get_data(float **data_ptr, int data_requested)
{
	int block_start = position;
	int block_end = position + data_requested;
	if (block_end > end_position)
		block_end = end_position;

	int retrieved = block_end-block_start;

	if (source.impl->format == sf_16bit_signed)
	{
		if (source.impl->num_channels == 2)
		{
			short *src = ((short *) source.impl->data) + position * 2;
			CL_SoundSSE::unpack_16bit_stereo(src, retrieved*2, data_ptr);
		}
		else
		{
			short *src = ((short *) source.impl->data) + position;
			CL_SoundSSE::unpack_16bit_mono(src, retrieved, data_ptr[0]);
		}
	}
	else
	{
		if (source.impl->num_channels == 2)
		{
			unsigned char *src = ((unsigned char *) source.impl->data) + position * 2;
			CL_SoundSSE::unpack_8bit_stereo(src, retrieved*2, data_ptr);
		}
		else
		{
			unsigned char *src = ((unsigned char *) source.impl->data) + position;
			CL_SoundSSE::unpack_8bit_mono(src, retrieved, data_ptr[0]);
		}
	}

	position += retrieved;
	return retrieved;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Wave_Session implementation:
