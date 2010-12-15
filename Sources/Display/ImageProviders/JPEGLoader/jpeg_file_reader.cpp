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

#include "Display/precomp.h"
#include "jpeg_file_reader.h"
#include "jpeg_define_huffman_table.h"
#include "jpeg_define_quantization_table.h"
#include "jpeg_start_of_frame.h"
#include "jpeg_start_of_scan.h"
#include "jpeg_markers.h"

CL_JPEGFileReader::CL_JPEGFileReader(CL_IODevice iodevice)
: iodevice(iodevice)
{
	iodevice.set_big_endian_mode();
}

CL_JPEGMarker CL_JPEGFileReader::read_marker()
{
	cl_uint8 b1 = iodevice.read_uint8();
	if (b1 != 0xff)
		throw CL_Exception("Invalid JPEG file");
	return (CL_JPEGMarker) iodevice.read_uint8();
}

void CL_JPEGFileReader::skip_unknown()
{
	cl_uint16 size = iodevice.read_uint16();
	if (!iodevice.seek(size-2, CL_IODevice::seek_cur))
		throw CL_Exception("Invalid JPEG file");
}

CL_JPEGStartOfFrame CL_JPEGFileReader::read_sof()
{
	CL_JPEGStartOfFrame header;

	cl_uint16 size = iodevice.read_uint16();
	if (size < 8)
		throw CL_Exception("Invalid JPEG file");

	header.sample_precision = iodevice.read_uint8();
	if (header.sample_precision != 8 && header.sample_precision != 16)
		throw CL_Exception("Invalid JPEG file");

	header.height = iodevice.read_uint16();
	header.width = iodevice.read_uint16();
	cl_uint8 num_components = iodevice.read_uint8();
	if (num_components == 0 || num_components > 4) // To do: Only do this check for progressive; sequential allows up to 255 components
		throw CL_Exception("Invalid JPEG file");

	if (size != 8 + 3*num_components)
		throw CL_Exception("Invalid JPEG file");

	for (int i = 0; i < num_components; i++)
	{
		CL_JPEGStartOfFrameComponent c;
		c.id = iodevice.read_uint8();
		cl_uint8 sampling_factor = iodevice.read_uint8();
		c.horz_sampling_factor = (sampling_factor & 0xf0) >> 4;
		c.vert_sampling_factor = (sampling_factor & 0x0f);
		c.quantization_table_selector = iodevice.read_uint8();

		if (c.horz_sampling_factor == 0 || c.horz_sampling_factor > 4 ||
			c.vert_sampling_factor == 0 || c.horz_sampling_factor == 0 ||
			c.quantization_table_selector > 3)
		{
			throw CL_Exception("Invalid JPEG file");
		}

		header.components.push_back(c);
	}

	return header;
}

CL_JPEGDefineQuantizationTable CL_JPEGFileReader::read_dqt()
{
	CL_JPEGDefineQuantizationTable tables;

	cl_uint16 size = iodevice.read_uint16();
	if (size < 3)
		throw CL_Exception("Invalid JPEG file");

	int p = 2;
	while (p < size)
	{
		CL_JPEGQuantizationTable table;
		cl_uint8 v = iodevice.read_uint8();
		cl_uint8 precision = (v & 0xf0) >> 4;
		table.table_index = (v & 0x0f);

		if (precision > 1 || table.table_index > 3)
			throw CL_Exception("Invalid JPEG file");

		if (precision == 0) // 8 bit
		{
			if (size < p + 1 + 64)
				throw CL_Exception("Invalid JPEG file");

			cl_uint8 values[64];
			iodevice.read(values, 64);
			for (int i = 0; i < 64; i++)
				table.values[i] = values[i];

			p += 1 + 64;
		}
		else // 16 bit
		{
			if (size < p + 1 + 64*2)
				throw CL_Exception("Invalid JPEG file");
			for (int i = 0; i < 64; i++)
				table.values[i] = iodevice.read_uint16();

			p += 1 + 64*2;
		}

		tables.push_back(table);
	}

	return tables;
}

CL_JPEGDefineHuffmanTable CL_JPEGFileReader::read_dht()
{
	CL_JPEGDefineHuffmanTable tables;

	cl_uint16 size = iodevice.read_uint16();
	if (size < 2 + 17)
		throw CL_Exception("Invalid JPEG file");

	int p = 2;
	while (p < size)
	{
		CL_JPEGHuffmanTable table;

		if (size < p + 17)
			throw CL_Exception("Invalid JPEG file");

		cl_uint8 v = iodevice.read_uint8();
		cl_uint8 table_class = (v & 0xf0) >> 4;
		if (table_class > 1)
			throw CL_Exception("Invalid JPEG file");
		table.table_class = (table_class == 0) ? CL_JPEGHuffmanTable::dc_table : CL_JPEGHuffmanTable::ac_table;
		table.table_index = (v & 0x0f);
		if (table.table_index > 3)
			throw CL_Exception("Invalid JPEG file");

		iodevice.read(table.bits, 16);
		int bindings = 0;
		for(int i = 0; i < 16; i++)
			bindings += table.bits[i];	
		table.values.resize(bindings);
		if (bindings > 0)
			iodevice.read(&table.values[0], bindings);

		if (size < p + 17 + bindings)
			throw CL_Exception("Invalid JPEG file");

		p += 17 + bindings;

		table.build_tree();
		tables.push_back(table);
	}

	return tables;
}

CL_JPEGDefineRestartInterval CL_JPEGFileReader::read_dri()
{
	CL_JPEGDefineRestartInterval interval = 0;
	cl_uint16 size = iodevice.read_uint16();
	if (size != 4)
		throw CL_Exception("Invalid JPEG file");
	interval = iodevice.read_uint16();
	return interval;
}

CL_JPEGStartOfScan CL_JPEGFileReader::read_sos()
{
	CL_JPEGStartOfScan header;

	cl_uint16 size = iodevice.read_uint16();
	if (size < 6)
		throw CL_Exception("Invalid JPEG file");

	cl_uint8 num_components = iodevice.read_uint8();
	if (num_components == 0 || num_components > 4)
		throw CL_Exception("Invalid JPEG file");

	if (size != 6 + 2*num_components)
		throw CL_Exception("Invalid JPEG file");

	for (int i = 0; i < num_components; i++)
	{
		CL_JPEGStartOfScanComponent c;
		c.component_selector = iodevice.read_uint8();
		cl_uint8 table_selector = iodevice.read_uint8();
		c.dc_table_selector = (table_selector & 0xf0) >> 4;
		c.ac_table_selector = (table_selector & 0x0f);

		if (c.ac_table_selector > 3 || c.dc_table_selector > 3)
			throw CL_Exception("Invalid JPEG file");

		header.components.push_back(c);
	}

	header.start_dct_coefficient = iodevice.read_uint8();
	header.end_dct_coefficient = iodevice.read_uint8();

	cl_uint8 ahal = iodevice.read_uint8();
	header.preceding_point_transform = (ahal & 0xf0) >> 4;
	header.point_transform = (ahal & 0x0f);

	if (//(header.start_dct_coefficient == 0 && header.end_dct_coefficient != 0) ||
		header.start_dct_coefficient > 63 || header.end_dct_coefficient > 63 ||
		header.start_dct_coefficient > header.end_dct_coefficient)
		throw CL_Exception("Invalid JPEG file");

	return header;
}

CL_JPEGDefineNumberOfLines CL_JPEGFileReader::read_dnl()
{
	CL_JPEGDefineNumberOfLines lines = 0;
	cl_uint16 size = iodevice.read_uint16();
	if (size != 4)
		throw CL_Exception("Invalid JPEG file");
	lines = iodevice.read_uint16();
	return lines;
}

CL_String8 CL_JPEGFileReader::read_comment()
{
	cl_uint16 size = iodevice.read_uint16();
	if (size < 2)
		throw CL_Exception("Invalid JPEG file");
	CL_String8 s(size-2, ' ');
	iodevice.read(s.data(), s.length());
	return s;
}

int CL_JPEGFileReader::read_entropy_data(void *d, int size)
{
	// FF is always followed by 00 in entropy data. This is done to allow an
	// application to scan for JPEG markers without matching entropy data by
	// accident.

	cl_uint8 *data = reinterpret_cast<cl_uint8*>(d);

	int start = iodevice.get_position();
	int len = iodevice.read(data, size, false);
	if (len == 0)
		return 0;

	int j = 0;
	for (int i = 0; i < len; i++)
	{
		if (data[i] == 0xff && i+1 < len && data[i+1] == 0x00)
		{
			data[j] = 0xff;
			j++;
			i++;
		}
		else if (data[i] == 0xff)
		{
			iodevice.seek(start + i);
			break;
		}
		else
		{
			data[j] = data[i];
			j++;
		}
	}
	return j;
}
