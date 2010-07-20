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

#pragma once

#include <vector>
#include "API/Core/System/sharedptr.h"
#include "API/Core/System/mutex.h"
#include "API/Sound/soundformat.h"
#include "API/Sound/soundoutput.h"
#include "API/Sound/soundbuffer.h"

class CL_SoundFilter;
class CL_SoundBuffer_Impl;
class CL_SoundProvider_Session;
class CL_SoundOutput_Impl;

class CL_SoundBuffer_Session_Impl
{
/// \name Construction
/// \{

public:
	CL_SoundBuffer_Session_Impl(
		CL_SoundBuffer &soundbuffer,
		bool looping,
		CL_SoundOutput &output);

	virtual ~CL_SoundBuffer_Session_Impl();


/// \}
/// \name Attributes
/// \{

public:
	CL_SoundBuffer soundbuffer;
	CL_SoundProvider_Session *provider_session;
	CL_SoundOutput output;
	float volume;
	float frequency;
	float pan;
	bool looping;
	bool playing;
	std::vector<CL_SoundFilter> filters;
	mutable CL_Mutex mutex;


/// \}
/// \name Operations
/// \{

public:
	bool mix_to(float **sample_data, float **temp_data, int num_samples, int num_channels);

/// \}
/// \name Implementation
/// \{

private:
	/// \brief Mixes the sample data from 'temp_data' into 'sample_data'
	void mix_channels( int num_channels, int num_samples, float ** sample_data, float ** temp_data );

	/// \brief Returns the volume of left and right channel
	void get_channel_volume(float *out_volume);

	/// \brief Reads data into temp_data in the mixers native frequency
	void get_data_in_mixer_frequency( int num_samples, float **temp_data );

	/// \brief Runs the sample data through attached filters
	void run_filters( float ** temp_data, int num_samples );

	/// \brief Fills temporary buffers with data from provider.
	void get_data();

	/// \brief Temporary channel buffers containing sound data in provider frequency.
	float **float_buffer_data;

	std::vector<float*> float_buffer_data_offsetted;

	/// \brief Size of temporary channel buffers.
	int num_buffer_samples;

	/// \brief Number of temporary channel buffers;
	int num_buffer_channels;

	/// \brief Current playback position in temporary buffers.
	double buffer_position;

	/// \brief Number of samples currently written to buffer_data.
	int buffer_samples_written;
/// \}
};


