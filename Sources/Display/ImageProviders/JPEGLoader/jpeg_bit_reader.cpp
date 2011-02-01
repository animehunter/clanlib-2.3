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

#include "Display/precomp.h"
#include "jpeg_bit_reader.h"
#include "jpeg_file_reader.h"

CL_JPEGBitReader::CL_JPEGBitReader(CL_JPEGFileReader *reader)
: reader(reader), length(0), pos(0), bitpos(0)
{
	buffer.resize(16*1024);
}

void CL_JPEGBitReader::reset()
{
	length = 0;
	pos = 0;
	bitpos = 0;
	buffer.resize(16*1024);
}

unsigned int CL_JPEGBitReader::get_bit()
{
	if (bitpos == 8)
	{
		pos++;
		bitpos = 0;
	}
	if (pos == length)
	{
		length = reader->read_entropy_data(&buffer[0], buffer.size());
		if (length == 0)
		{
			//JPEGMarker marker = reader->read_marker();
			throw CL_Exception("Premature end of JPEG entropy data");
		}
		pos = 0;
	}

	unsigned int v = (buffer[pos] >> (7-bitpos)) & 0x01;
	bitpos++;
	return v;
}

unsigned int CL_JPEGBitReader::get_bits(int count)
{
	int v = 0;
	for (int i = 0; i < count; i++)
	{
		v = (v << 1)|get_bit();
	}
	return v;
}
