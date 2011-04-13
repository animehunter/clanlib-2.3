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
#include "soundoutput_macosx.h"
#include "API/Core/System/exception.h"
#include "API/Core/System/system.h"
#include <sys/time.h>

CL_SoundOutput_MacOSX::CL_SoundOutput_MacOSX(int frequency, int latency)
: CL_SoundOutput_Impl(frequency, latency), frequency(frequency), latency(latency), device(0), context(0), source(0), fragment_size(0), next_fragment(0)
{
    fragment_size = frequency * latency / fragment_buffer_count / 1000;
    fragment_size = (fragment_size + 3) & ~3; // Force to be a multiple of 4
    fragments_available = fragment_buffer_count;
    fragment_data = CL_DataBuffer(fragment_size * sizeof(short) * 2 * fragment_buffer_count);
    
    start_mixer_thread();
}

void CL_SoundOutput_MacOSX::init_openal()
{
    device = alcOpenDevice(0);
    if (device == 0)
        throw CL_Exception("Unable to open default sound device");
    
    context = alcCreateContext(device, 0);
    if (context == 0)
    {
        alcCloseDevice(device);
        device = 0;
        throw CL_Exception("Unable to create OpenAL context for the sound device");
    }
    
    alcMakeContextCurrent(context);
    
    alBufferDataStatic = reinterpret_cast<alBufferDataStaticProcPtr>(alGetProcAddress("alBufferDataStatic"));
    if (alBufferDataStatic == 0)
    {
        alcMakeContextCurrent(0);
        alcDestroyContext(context);
        alcCloseDevice(device);
        context = 0;
        device = 0;
        throw CL_Exception("OpenAL does not have alBufferDataStatic");
    }
    
    alGenBuffers(fragment_buffer_count, fragment_buffers);

    for (size_t i = 0; i < fragment_buffer_count; i++)
    {
        short *data = reinterpret_cast<short*>(fragment_data.get_data());
        alBufferDataStatic(fragment_buffers[next_fragment], AL_FORMAT_STEREO16, data + fragment_size * 2 * i, fragment_size * sizeof(short) * 2, frequency);
    }
    
    alGenSources(1, &source);
    alSource3f(source, AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSource3f(source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alSource3f(source, AL_DIRECTION, 0.0f, 0.0f, 0.0f);
    alSourcef(source, AL_ROLLOFF_FACTOR, 0.0f);
    alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE);
    
    if (alGetError() != AL_NO_ERROR)
    {
        alcMakeContextCurrent(0);
        alcDestroyContext(context);
        alcCloseDevice(device);
        device = 0;
        context = 0;
        throw CL_Exception("OpenAL initialization failed");
    }
    
    alcMakeContextCurrent(0);
}

CL_SoundOutput_MacOSX::~CL_SoundOutput_MacOSX()
{
	stop_mixer_thread();
    if (device)
    {
        alcMakeContextCurrent(context);
        alSourceStop(source);
        alDeleteSources(1, &source);
        alDeleteBuffers(fragment_buffer_count, fragment_buffers);
        alcMakeContextCurrent(0);
        alcDestroyContext(context);
        alcCloseDevice(device);
    }
}

void CL_SoundOutput_MacOSX::silence()
{
}

int CL_SoundOutput_MacOSX::get_fragment_size()
{
    return fragment_size;
}

void CL_SoundOutput_MacOSX::write_fragment(float *dataf)
{
    if (device == 0)
        init_openal();
    
    if (fragments_available == 0)
        wait();
    
    short *data16 = reinterpret_cast<short*>(fragment_data.get_data()) + next_fragment * fragment_size * 2;
    for (int i = 0; i < fragment_size * 2; i++)
    {
        data16[i] = (short)(dataf[i]*32767);
    }

    alcMakeContextCurrent(context);
    alSourceQueueBuffers(source, 1, &fragment_buffers[next_fragment]);

    ALint source_state = 0;
    alGetSourcei(source, AL_SOURCE_STATE, &source_state);
    if (source_state != AL_PLAYING)
        alSourcePlay(source);

    if (alGetError() != AL_NO_ERROR)
    {
        alcMakeContextCurrent(0);
        alcDestroyContext(context);
        alcCloseDevice(device);
        throw CL_Exception("OpenAL mixing failed");
    }
    
    alcMakeContextCurrent(0);
    
    fragments_available--;
    next_fragment++;
    if (next_fragment == fragment_buffer_count)
        next_fragment = 0;
}

void CL_SoundOutput_MacOSX::wait()
{
    if (device == 0)
        init_openal();
    
    alcMakeContextCurrent(context);
    
    while (fragments_available == 0)
    {
        ALint processed = 0;
        alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
        fragments_available += processed;
        
        if (processed != 0)
        {
            ALuint processed_buffers[fragment_buffer_count];
            alSourceUnqueueBuffers(source, processed, processed_buffers);
            break;
        }
        else
        {
            ALint source_state = 0;
            alGetSourcei(source, AL_SOURCE_STATE, &source_state);
            if (source_state != AL_PLAYING)
                alSourcePlay(source);

            ALint queued = 0;
            alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);
            if (queued == 0)
            {
                fragments_available = fragment_buffer_count;
                break;
            }
        }
        
        if (fragments_available == 0)
        {
            // Sigh.  No official way to wait for a buffer to be processed?
            timeval timeout;
            timeout.tv_sec = 0;
            timeout.tv_usec = latency * 1000 / 4;
            select(0, 0, 0, 0, &timeout);
        }
    }
                     
    alcMakeContextCurrent(0);
}
