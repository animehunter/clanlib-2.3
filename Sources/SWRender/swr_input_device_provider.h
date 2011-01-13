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

#pragma once


#include "API/Display/TargetProviders/input_device_provider.h"

class CL_SWRenderInputDeviceProvider : public CL_InputDeviceProvider
{
/// \name Construction
/// \{

public:
	CL_SWRenderInputDeviceProvider();

	~CL_SWRenderInputDeviceProvider();

	void destroy();


/// \}
/// \name Attributes
/// \{

public:
	CL_String get_name() const;

	CL_String get_device_name() const;

	CL_InputDevice::Type get_type() const;

	CL_String get_key_name(int id) const;

//	bool supports_keyid_mapping() const { return false; }

//	CL_String keyid_to_string(int keycode) const { return CL_String(); }

//	int string_to_keyid(const CL_String &str) const { return 0; }

	bool get_keycode(int keycode) const;

	int get_x() const;

	int get_y() const;

	float get_axis(int index) const;

	int get_axis_count() const;

	int get_button_count() const;


/// \}
/// \name Operations
/// \{

public:
	void init(CL_Signal_v1<const CL_InputEvent &> *sig_provider_event);

	void set_position(int x, int y);

	bool poll(bool peek_only);


/// \}
/// \name Implementation
/// \{

private:
/// \}
};


