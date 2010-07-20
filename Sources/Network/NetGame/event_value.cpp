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

#include "Network/precomp.h"
#include "API/Network/NetGame/event_value.h"

CL_NetGameEventValue::CL_NetGameEventValue()
: type(null), value_int(0)
{
}

CL_NetGameEventValue::CL_NetGameEventValue(int value)
: type(integer), value_int(value)
{
}

CL_NetGameEventValue::CL_NetGameEventValue(unsigned int value)
: type(uinteger), value_uint(value)
{
}

CL_NetGameEventValue::CL_NetGameEventValue(float value)
: type(number), value_float(value)
{
}

CL_NetGameEventValue::CL_NetGameEventValue(const CL_String &value)
: type(string), value_string(value)
{
}

CL_NetGameEventValue::CL_NetGameEventValue(const CL_StringRef &value)
: type(string), value_string(value)
{
}

CL_NetGameEventValue::CL_NetGameEventValue(const char *value)
: type(string), value_string(value)
{
}

CL_NetGameEventValue::CL_NetGameEventValue(const wchar_t *value)
: type(string), value_string(value)
{
}

CL_NetGameEventValue::CL_NetGameEventValue(bool value)
: type(boolean), value_bool(value)
{
}

CL_NetGameEventValue::CL_NetGameEventValue(Type type)
: type(type), value_int(0)
{
}

CL_NetGameEventValue::Type CL_NetGameEventValue::get_type() const
{
	return type;
}

bool CL_NetGameEventValue::is_null() const
{
	return type == null;
}

bool CL_NetGameEventValue::is_uinteger() const
{
	return type == uinteger;
}

bool CL_NetGameEventValue::is_integer() const
{
	return type == integer;
}

bool CL_NetGameEventValue::is_number() const
{
	return type == number;
}

bool CL_NetGameEventValue::is_string() const
{
	return type == string;
}

bool CL_NetGameEventValue::is_boolean() const
{
	return type == boolean;
}

bool CL_NetGameEventValue::is_complex() const
{
	return type == complex;
}

unsigned int CL_NetGameEventValue::get_member_count() const
{
	throw_if_not_complex();
	return value_complex.size();
}

const CL_NetGameEventValue &CL_NetGameEventValue::get_member(unsigned int index) const
{
	throw_if_not_complex();
	return value_complex.at(index);
}

void CL_NetGameEventValue::add_member(const CL_NetGameEventValue &value)
{
	throw_if_not_complex();
	value_complex.push_back(value);
}

void CL_NetGameEventValue::set_member(unsigned int index, const CL_NetGameEventValue &value)
{
	throw_if_not_complex();
	value_complex.at(index) = value;
}

void CL_NetGameEventValue::throw_if_not_complex() const
{
	if (type != complex)
		throw CL_Exception("CL_NetGameEventValue is not a complex type");
}

unsigned int CL_NetGameEventValue::to_uinteger() const
{
	if (is_uinteger())
		return value_uint;
	else
		throw CL_Exception("CL_NetGameEventValue is not an unsigned integer");
}

int CL_NetGameEventValue::to_integer() const
{
	if (is_integer())
		return value_int;
	else
		throw CL_Exception("CL_NetGameEventValue is not an integer");
}

float CL_NetGameEventValue::to_number() const
{
	if (is_number())
		return value_float;
	else
		throw CL_Exception("CL_NetGameEventValue is not a floating point number");
}

CL_String CL_NetGameEventValue::to_string() const
{
	if (is_string())
		return value_string;
	else
		throw CL_Exception("CL_NetGameEventValue is not a string");
}

bool CL_NetGameEventValue::to_boolean() const
{
	if (is_boolean())
		return value_bool;
	else
		throw CL_Exception("CL_NetGameEventValue is not a boolean");
}
