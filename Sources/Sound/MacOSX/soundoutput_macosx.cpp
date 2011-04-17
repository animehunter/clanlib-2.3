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

CL_SoundOutput_MacOSX::CL_SoundOutput_MacOSX(int frequency, int latency)
: CL_SoundOutput_Impl(frequency, latency), frequency(frequency), latency(latency), fragment_size(0), next_fragment(0), read_cursor(0), fragments_available(0)
{
    fragment_size = frequency * latency / fragment_buffer_count / 1000;
    fragment_size = (fragment_size + 3) & ~3; // Force to be a multiple of 4
    fragments_available = fragment_buffer_count;
    fragment_data = CL_DataBuffer(fragment_size * sizeof(short) * 2 * fragment_buffer_count);
        
    start_mixer_thread();
}

CL_SoundOutput_MacOSX::~CL_SoundOutput_MacOSX()
{
	stop_mixer_thread();
}

void CL_SoundOutput_MacOSX::mixer_thread_starting()
{
    audio_format.mSampleRate = frequency;
    audio_format.mFormatID = kAudioFormatLinearPCM;
    audio_format.mFormatFlags = kAudioFormatFlagsCanonical;
    audio_format.mBytesPerPacket = 2 * sizeof(short);
    audio_format.mFramesPerPacket = 1;
    audio_format.mBytesPerFrame = 2 * sizeof(short);
    audio_format.mChannelsPerFrame = 2;
    audio_format.mBitsPerChannel = sizeof(short) * 8;
    audio_format.mReserved = 0;

    OSStatus result = AudioQueueNewOutput(&audio_format, &CL_SoundOutput_MacOSX::static_audio_queue_callback, this, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode, 0, &audio_queue);
    if (result != 0)
        throw CL_Exception("AudioQueueNewOutput failed");

    for (int i = 0; i < fragment_buffer_count; i++)
    {
        result = AudioQueueAllocateBuffer(audio_queue, fragment_size * sizeof(short) * 2, &audio_buffers[i]);
        if (result != 0)
            throw CL_Exception("AudioQueueAllocateBuffer failed");
        audio_queue_callback(audio_queue, audio_buffers[i]);
    }
    
    result = AudioQueueStart(audio_queue, 0);
    if (result != 0)
        throw CL_Exception("AudioQueueStart failed");
}

void CL_SoundOutput_MacOSX::mixer_thread_stopping()
{
    AudioQueueStop(audio_queue, true);
    AudioQueueDispose(audio_queue, true);
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
    if (fragments_available == 0)
        wait();
    
    short *databuf = reinterpret_cast<short*>(fragment_data.get_data()) + next_fragment * fragment_size * 2;
    for (int i = 0; i < fragment_size * 2; i++)
    {
        databuf[i] = (short)(dataf[i]*32767);
    }
    
    next_fragment++;
    if (next_fragment == fragment_buffer_count)
        next_fragment = 0;
    fragments_available--;
}

void CL_SoundOutput_MacOSX::wait()
{
    while (fragments_available == 0)
    {
        CFRunLoopRunInMode(kCFRunLoopDefaultMode, 1, true);
    }
}

void CL_SoundOutput_MacOSX::audio_queue_callback(AudioQueueRef queue, AudioQueueBufferRef buffer)
{
    if (fragments_available != fragment_buffer_count)
    {
        short *databuf = reinterpret_cast<short*>(fragment_data.get_data()) + read_cursor * fragment_size * 2;
        memcpy(buffer->mAudioData, databuf, fragment_size * sizeof(short) * 2);
        buffer->mAudioDataByteSize = fragment_size * sizeof(short) * 2;
        fragments_available++;
        read_cursor++;
        if (read_cursor == fragment_buffer_count)
            read_cursor = 0;
    }
    else
    {
        memset(buffer->mAudioData, 0, fragment_size * sizeof(short) * 2);
        buffer->mAudioDataByteSize = fragment_size * sizeof(short) * 2;
    }

    OSStatus result = AudioQueueEnqueueBuffer(queue, buffer, 0, 0);
    if (result != 0)
        throw CL_Exception("AudioQueueEnqueueBuffer failed");
}

void CL_SoundOutput_MacOSX::static_audio_queue_callback(void *userdata, AudioQueueRef queue, AudioQueueBufferRef buffer)
{
    CL_SoundOutput_MacOSX *self = reinterpret_cast<CL_SoundOutput_MacOSX *>(userdata);
    self->audio_queue_callback(queue, buffer);
}
