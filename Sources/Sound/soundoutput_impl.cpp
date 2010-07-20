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
#include "soundoutput_impl.h"
#include "soundbuffer_session_impl.h"
#include "API/Sound/soundfilter.h"
#include <algorithm>
#include "API/Sound/sound_sse.h"

CL_Mutex CL_SoundOutput_Impl::singleton_mutex;
CL_SoundOutput_Impl *CL_SoundOutput_Impl::instance = 0;

/////////////////////////////////////////////////////////////////////////////
// CL_SoundOutput_Impl construction:

CL_SoundOutput_Impl::CL_SoundOutput_Impl(int mixing_frequency, int latency)
: mixing_frequency(mixing_frequency), mixing_latency(latency), volume(1.0f),
  pan(0.0f), mix_buffer_size(0)
{
 	mix_buffers[0] = 0;
	mix_buffers[1] = 0;
	temp_buffers[0] = 0;
	temp_buffers[1] = 0;
	stereo_buffer = 0;

	CL_MutexSection lock(&singleton_mutex);
	if (instance)
		throw CL_Exception("Only a single instance of CL_SoundOutput is allowed");
	instance = this;

}

CL_SoundOutput_Impl::~CL_SoundOutput_Impl()
{
	CL_SoundSSE::aligned_free(stereo_buffer);
	CL_SoundSSE::aligned_free(mix_buffers[0]);
	CL_SoundSSE::aligned_free(mix_buffers[1]);
	CL_SoundSSE::aligned_free(temp_buffers[0]);
	CL_SoundSSE::aligned_free(temp_buffers[1]);

	CL_MutexSection lock(&singleton_mutex);
	instance = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundOutput_Impl attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_SoundOutput_Impl operations:

void CL_SoundOutput_Impl::play_session(CL_SoundBuffer_Session &session)
{
	CL_MutexSection mutex_lock(&mutex);
	sessions.push_back(session);
}

void CL_SoundOutput_Impl::stop_session(CL_SoundBuffer_Session &session)
{
	CL_MutexSection mutex_lock(&mutex);

	for (std::vector<CL_SoundBuffer_Session>::iterator it = sessions.begin(); it != sessions.end(); ++it)
	{
		if (session.impl.get() == it->impl.get())
		{
			sessions.erase(it);
			break;
		}
	}
}

void CL_SoundOutput_Impl::start_mixer_thread()
{
	thread.start(this, &CL_SoundOutput_Impl::mixer_thread);
//	thread.set_priority(cl_priority_highest);
}

void CL_SoundOutput_Impl::stop_mixer_thread()
{
	CL_MutexSection mutex_lock(&mutex);
	stop_mixer.set();
	mutex_lock.unlock();
	thread.join();
	thread = CL_Thread();
}

void CL_SoundOutput_Impl::mix_fragment()
{
	resize_mix_buffers();
	clear_mix_buffers();
	fill_mix_buffers();
	filter_mix_buffers();
	apply_master_volume_on_mix_buffers();
	clamp_mix_buffers();
	CL_SoundSSE::pack_float_stereo(mix_buffers, mix_buffer_size, stereo_buffer);
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundOutput_Impl implementation:

void CL_SoundOutput_Impl::mixer_thread()
{
	while (if_continue_mixing())
	{
		// Mix some audio:
		mix_fragment();

		// Send mixed data to sound card:
		write_fragment(stereo_buffer);

		// Wait for sound card to want more:
		wait();
	}
}

bool CL_SoundOutput_Impl::if_continue_mixing()
{
	CL_MutexSection mutex_lock(&mutex);
	if (stop_mixer.wait(0))
		return false;
	else
		return true;
}

void CL_SoundOutput_Impl::resize_mix_buffers()
{
	if (get_fragment_size() != mix_buffer_size)
	{
		CL_SoundSSE::aligned_free(stereo_buffer); stereo_buffer = 0;
		CL_SoundSSE::aligned_free(mix_buffers[0]); mix_buffers[0] = 0;
		CL_SoundSSE::aligned_free(mix_buffers[1]); mix_buffers[1] = 0;
		CL_SoundSSE::aligned_free(temp_buffers[0]); temp_buffers[0] = 0;
		CL_SoundSSE::aligned_free(temp_buffers[1]); temp_buffers[1] = 0;

		mix_buffer_size = get_fragment_size();
		//if (mix_buffer_size & 3)
		//	throw CL_Exception("Fragment size must be a multiple of 4");

		mix_buffers[0] = (float *) CL_SoundSSE::aligned_alloc(sizeof(float) * mix_buffer_size );
		mix_buffers[1] = (float *) CL_SoundSSE::aligned_alloc(sizeof(float) * mix_buffer_size );
		temp_buffers[0] = (float *) CL_SoundSSE::aligned_alloc(sizeof(float) * mix_buffer_size );
		temp_buffers[1] = (float *) CL_SoundSSE::aligned_alloc(sizeof(float) * mix_buffer_size );
		stereo_buffer = (float *) CL_SoundSSE::aligned_alloc(sizeof(float) * mix_buffer_size * 2);
		CL_SoundSSE::set_float(stereo_buffer, mix_buffer_size*2, 0.0f);
	}
}

void CL_SoundOutput_Impl::clear_mix_buffers()
{
	// Clear channel mixing buffers:
	CL_SoundSSE::set_float(mix_buffers[0], mix_buffer_size, 0.0f);
	CL_SoundSSE::set_float(mix_buffers[1], mix_buffer_size, 0.0f);
}

void CL_SoundOutput_Impl::fill_mix_buffers()
{
	CL_MutexSection mutex_lock(&mutex);
	std::vector< CL_SoundBuffer_Session > ended_sessions;
	std::vector< CL_SoundBuffer_Session >::iterator it;
	for (it = sessions.begin(); it != sessions.end(); ++it)
	{
		CL_SoundBuffer_Session session = *it;
		bool playing = session.impl->mix_to(mix_buffers, temp_buffers, mix_buffer_size, 2);
		if (!playing) ended_sessions.push_back(session);
	}

	// Release any sessions pending for removal:
	int size_ended_sessions = ended_sessions.size();
	for (int i = 0; i < size_ended_sessions; i++) stop_session(ended_sessions[i]);
}

void CL_SoundOutput_Impl::filter_mix_buffers()
{
	// Apply global filters to mixing buffers:
	CL_MutexSection mutex_lock(&mutex);
	int size_filters = filters.size();
	int i;
	for (i = 0; i < size_filters; i++)
	{
		filters[i].filter(mix_buffers, mix_buffer_size, 2);
	}
}

void CL_SoundOutput_Impl::apply_master_volume_on_mix_buffers()
{
	// Calculate volume on left and right channel:
	float left_pan = 1-pan;
	float right_pan = 1+pan;
	if (left_pan < 0.0f) left_pan = 0.0f;
	if (left_pan > 1.0f) left_pan = 1.0f;
	if (right_pan < 0.0f) right_pan = 0.0f;
	if (right_pan > 1.0f) right_pan = 1.0f;
	if (volume < 0.0f) volume = 0.0f;
	if (volume > 1.0f) volume = 1.0f;

	float left_volume = volume * left_pan;
	float right_volume = volume * right_pan;

	CL_SoundSSE::multiply_float(mix_buffers[0], mix_buffer_size, left_volume);
	CL_SoundSSE::multiply_float(mix_buffers[1], mix_buffer_size, right_volume);
}

void CL_SoundOutput_Impl::clamp_mix_buffers()
{
	// Make sure values stay inside 16 bit range:
	for (int chan = 0; chan < 2; chan++)
	{
		for (int k=0; k<mix_buffer_size; k++)
		{
			if      (mix_buffers[chan][k] > 1.0f)  mix_buffers[chan][k] = 1.0f;
			else if (mix_buffers[chan][k] < -1.0f) mix_buffers[chan][k] = -1.0f;
		}
	}
}
