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

#include "API/Core/IOData/iodevice.h"
#include "jpeg_define_huffman_table.h"
#include "jpeg_define_quantization_table.h"
#include "jpeg_markers.h"

class CL_JPEGStartOfFrame;
class CL_JPEGStartOfScan;

typedef cl_ubyte16 CL_JPEGDefineRestartInterval;
typedef cl_ubyte16 CL_JPEGDefineNumberOfLines;

class CL_JPEGFileReader
{
public:
	CL_JPEGFileReader(CL_IODevice iodevice);

	CL_JPEGMarker read_marker();
	void skip_unknown();
	bool try_read_app0_jfif();
	bool try_read_app14_adobe(int &out_transform);
	CL_JPEGStartOfFrame read_sof();
	CL_JPEGDefineQuantizationTable read_dqt();
	CL_JPEGDefineHuffmanTable read_dht();
	CL_JPEGDefineRestartInterval read_dri();
	CL_JPEGStartOfScan read_sos();
	CL_JPEGDefineNumberOfLines read_dnl();
	CL_String8 read_comment();
	int read_entropy_data(void *d, int size);

private:
	CL_IODevice iodevice;
};
