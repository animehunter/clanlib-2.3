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

#include "API/Sound/SoundProviders/soundprovider_session.h"
#include "API/Sound/SoundProviders/soundprovider_raw.h"

class CL_SoundProvider_Raw_Session : public CL_SoundProvider_Session
{
/// \name Construction
/// \{

public:
	CL_SoundProvider_Raw_Session(CL_SoundProvider_Raw &source);
	~CL_SoundProvider_Raw_Session();


/// \}
/// \name Attributes
/// \{

public:
	int get_num_samples() const;
	int get_frequency() const;
	int get_num_channels() const;
	int get_position() const;


/// \}
/// \name Operations
/// \{

public:
	bool eof() const;
	void stop();
	bool play();
	bool set_position(int pos);
	bool set_end_position(int pos) { return false; }
	int get_data(float **data_ptr, int data_requested);


/// \}
/// \name Implementation
/// \{

private:
	CL_SoundProvider_Raw source;

	int position;
	int num_samples;
	int frequency;

	bool reached_end;
/// \}
};


