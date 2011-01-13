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
#include "API/Display/Image/pixel_buffer.h"
#include "jpeg_file_reader.h"
#include "jpeg_start_of_frame.h"
#include "jpeg_start_of_scan.h"
#include "jpeg_define_huffman_table.h"
#include "jpeg_define_quantization_table.h"
#include "jpeg_component_dcts.h"
#include "jpeg_markers.h"

class CL_JPEGBitReader;

class CL_JPEGLoader
{
public:
	static CL_PixelBuffer load(CL_IODevice iodevice);

private:
	CL_JPEGLoader(CL_IODevice iodevice);

	void process_dnl(CL_JPEGFileReader &reader);
	void process_sos(CL_JPEGFileReader &reader);
	void process_sos_sequential(CL_JPEGStartOfScan &start_of_scan, std::vector<int> component_to_sof, CL_JPEGBitReader &bit_reader);
	void process_sos_progressive(CL_JPEGStartOfScan &start_of_scan, std::vector<int> component_to_sof, CL_JPEGBitReader &bit_reader);
	void process_dqt(CL_JPEGFileReader &reader);
	void process_dht(CL_JPEGFileReader &reader);
	void process_sof(CL_JPEGMarker marker, CL_JPEGFileReader &reader);
	CL_JPEGStartOfFrame start_of_frame;
	CL_JPEGHuffmanTable huffman_dc_tables[4];
	CL_JPEGHuffmanTable huffman_ac_tables[4];
	CL_JPEGQuantizationTable quantization_tables[4];
	std::vector<CL_JPEGComponentDCTs> component_dcts;
	bool progressive;
	int scan_count;
	int mcu_x;
	int mcu_y;
	int mcu_width;
	int mcu_height;
	int restart_interval;
	int eobrun;
	std::vector<short> last_dc_values;

	static int zigzag_map[64];

	friend class CL_JPEGMCUDecoder;
	friend class CL_JPEGRGBDecoder;
};
