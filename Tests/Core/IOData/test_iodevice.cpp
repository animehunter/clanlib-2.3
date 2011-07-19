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
**    Mark Page
**    (if your name is missing here, please add it)
*/

#include "test.h"

void TestApp::test_iodevice(void)
{
	CL_Console::write_line(" Header: iodevice.h");
	CL_Console::write_line("  Class: CL_IODevice");
	CL_Console::write_line("  (Using CL_IODevice_Memory as a base class)");

	CL_String str;
	CL_String str2;
	const size_t buffer_size = 1024;
	CL_DataBuffer data(buffer_size);
	CL_IODevice_Memory mem(data);
	const size_t test_data_size = 256;
	char test_data[test_data_size];
	char test_data2[test_data_size];

	for (int cnt=0; cnt<test_data_size; cnt++)
	{
		test_data[cnt] = cnt;
	}

//*** testing get_size()
	CL_Console::write_line("   Function: int get_size()");
	if (mem.get_size() != buffer_size) fail();

//*** testing get_position()
	CL_Console::write_line("   Function: int get_position()");
	if (mem.get_position() != 0) fail();
	mem.seek(123);
	if (mem.get_position() != 123) fail();
	mem.seek(0);

//*** testing is_little_endian()
	CL_Console::write_line("   Function: bool is_little_endian()");
	if (mem.is_little_endian() != true) fail();
	mem.set_big_endian_mode();
	if (mem.is_little_endian() == true) fail();
	mem.set_little_endian_mode();
	if (mem.is_little_endian() != true) fail();

//*** testing int send()
	CL_Console::write_line("   Function: int send(const void *data, int len, bool send_all = true)");
	if (mem.send(test_data, test_data_size, true) != test_data_size) fail();
	if (memcmp(data.get_data(), test_data, test_data_size)) fail();
	if (mem.get_position() != test_data_size) fail();
	mem.seek(0);

//*** testing int receive()
	CL_Console::write_line("   Function: int receive(void *data, int len, bool receive_all = true)");
	memset(test_data2, 0, sizeof(test_data2));
	if (mem.receive(test_data2, test_data_size, true) != test_data_size) fail();
	if (memcmp(test_data2, test_data, test_data_size)) fail();
	if (mem.get_position() != test_data_size) fail();
	mem.seek(0);

//*** testing int peek()
	CL_Console::write_line("   Function: int peek(void *data, int len)");
	memset(test_data2, 0, sizeof(test_data2));
	if (mem.peek(test_data2, test_data_size) != test_data_size) fail();
	if (memcmp(test_data2, test_data, test_data_size)) fail();
	if (mem.get_position() != 0) fail();

//*** testing int seek()
	CL_Console::write_line("   Function: bool seek(int position, SeekMode mode = seek_set)");
	memset(test_data2, 0, sizeof(test_data2));
	if (mem.get_position() != 0) fail();
	if (!mem.seek(123)) fail();
	if (mem.get_position() != 123) fail();
	if (!mem.seek(7, CL_IODevice::seek_cur)) fail();
	if (mem.get_position() != 130) fail();
	if (!mem.seek(-7, CL_IODevice::seek_end)) fail();
	if (mem.get_position() != buffer_size-7) fail();

	if (!mem.seek(0)) fail();

//*** testing int write()
	CL_Console::write_line("   Function: int write(const void *data, int len, bool send_all = true)");
	if (mem.write(test_data, test_data_size, true) != test_data_size) fail();
	if (memcmp(data.get_data(), test_data, test_data_size)) fail();
	if (mem.get_position() != test_data_size) fail();
	mem.seek(0);

//*** testing int read()
	CL_Console::write_line("   Function: int read(void *data, int len, bool receive_all = true)");
	memset(test_data2, 0, sizeof(test_data2));
	if (mem.read(test_data2, test_data_size, true) != test_data_size) fail();
	if (memcmp(test_data2, test_data, test_data_size)) fail();
	if (mem.get_position() != test_data_size) fail();
	mem.seek(0);

//*** testing set_system_mode()
	CL_Console::write_line("   Function: void set_system_mode()");
	mem.set_system_mode();
	if (CL_Endian::is_system_big() == mem.is_little_endian()) fail();
	mem.set_little_endian_mode();

//*** testing set_big_endian_mode
	CL_Console::write_line("   Function: void set_big_endian_mode()");
	mem.set_big_endian_mode();
	if (mem.is_little_endian() == true) fail();
	mem.set_little_endian_mode();
	if (mem.is_little_endian() != true) fail();

//*** testing set_little_endian_mode
	CL_Console::write_line("   Function: void set_little_endian_mode()");
	mem.set_little_endian_mode();
	if (mem.is_little_endian() != true) fail();
	mem.set_big_endian_mode();
	if (mem.is_little_endian() == true) fail();

//*** testing void write_int64();
	CL_Console::write_line("   Function: void write_int64(cl_long data);");
	cl_long *vptr_int64 = (cl_long *) test_data;
	mem.set_big_endian_mode();
	mem.write_int64(*vptr_int64);
	mem.set_little_endian_mode();
	mem.write_int64(*vptr_int64);
	if (mem.get_position() != 16) fail();
	vptr_int64 = (cl_long *) data.get_data();
	if (vptr_int64[0] != 0x0001020304050607) fail();
	if (vptr_int64[1] != 0x0706050403020100) fail();
	mem.seek(0);

//*** testing void write_uint64();
	CL_Console::write_line("   Function: void write_uint64(cl_ulong data);");
	cl_ulong *vptr_uint64 = (cl_ulong *) test_data;
	mem.set_big_endian_mode();
	mem.write_uint64(*vptr_uint64);
	mem.set_little_endian_mode();
	mem.write_uint64(*vptr_uint64);
	if (mem.get_position() != 16) fail();
	vptr_uint64 = (cl_ulong *) data.get_data();
	if (vptr_uint64[0] != 0x0001020304050607) fail();
	if (vptr_uint64[1] != 0x0706050403020100) fail();
	mem.seek(0);

//*** testing void write_int32();
	CL_Console::write_line("   Function: void write_int32(cl_int data);");
	cl_int *vptr_int32 = (cl_int *) test_data;
	mem.set_big_endian_mode();
	mem.write_int32(*vptr_int32);
	mem.set_little_endian_mode();
	mem.write_int32(*vptr_int32);
	if (mem.get_position() != 8) fail();
	vptr_int32 = (cl_int *) data.get_data();
	if (vptr_int32[0] != 0x00010203) fail();
	if (vptr_int32[1] != 0x03020100) fail();
	mem.seek(0);

//*** testing void write_uint32();
	CL_Console::write_line("   Function: void write_uint32(cl_uint data);");
	cl_uint *vptr_uint32 = (cl_uint *) test_data;
	mem.set_big_endian_mode();
	mem.write_uint32(*vptr_uint32);
	mem.set_little_endian_mode();
	mem.write_uint32(*vptr_uint32);
	if (mem.get_position() != 8) fail();
	vptr_uint32 = (cl_uint *) data.get_data();
	if (vptr_uint32[0] != 0x00010203) fail();
	if (vptr_uint32[1] != 0x03020100) fail();
	mem.seek(0);

//*** testing void write_int16();
	CL_Console::write_line("   Function: void write_int16(cl_short data);");
	cl_short *vptr_int16 = (cl_short *) test_data;
	mem.set_big_endian_mode();
	mem.write_int16(*vptr_int16);
	mem.set_little_endian_mode();
	mem.write_int16(*vptr_int16);
	if (mem.get_position() != 4) fail();
	vptr_int16 = (cl_short *) data.get_data();
	if (vptr_int16[0] != 0x0001) fail();
	if (vptr_int16[1] != 0x0100) fail();
	mem.seek(0);

//*** testing void write_uint16();
	CL_Console::write_line("   Function: void write_uint16(cl_ushort data);");
	cl_ushort *vptr_uint16 = (cl_ushort *) test_data;
	mem.set_big_endian_mode();
	mem.write_uint16(*vptr_uint16);
	mem.set_little_endian_mode();
	mem.write_uint16(*vptr_uint16);
	if (mem.get_position() != 4) fail();
	vptr_uint16 = (cl_ushort *) data.get_data();
	if (vptr_uint16[0] != 0x0001) fail();
	if (vptr_uint16[1] != 0x0100) fail();
	mem.seek(0);

//*** testing void write_int8();
	CL_Console::write_line("   Function: void write_int8(cl_char data);");
	cl_char *vptr_int8 = (cl_char *) test_data;
	mem.set_big_endian_mode();
	mem.write_int8(*vptr_int8);
	mem.set_little_endian_mode();
	mem.write_int8(*vptr_int8);
	if (mem.get_position() != 2) fail();
	vptr_int8 = (cl_char *) data.get_data();
	if (vptr_int8[0] != 0x00) fail();
	if (vptr_int8[1] != 0x00) fail();
	mem.seek(0);

//*** testing void write_uint8();
	CL_Console::write_line("   Function: void write_uint8(cl_uchar data);");
	cl_uchar *vptr_uint8 = (cl_uchar *) test_data;
	mem.set_big_endian_mode();
	mem.write_uint8(*vptr_uint8);
	mem.set_little_endian_mode();
	mem.write_uint8(*vptr_uint8);
	if (mem.get_position() != 2) fail();
	vptr_uint8 = (cl_uchar *) data.get_data();
	if (vptr_uint8[0] != 0x00) fail();
	if (vptr_uint8[1] != 0x00) fail();
	mem.seek(0);

//*** testing cl_long read_int64()
	CL_Console::write_line("   Function: cl_long read_int64()");
	if (mem.send(test_data, 64, true) != 64) fail();
	mem.seek(0);
	vptr_int64 = (cl_long *) data.get_data();
	mem.set_big_endian_mode();
	if (mem.read_int64() != 0x0001020304050607) fail();
	mem.set_little_endian_mode();
	if (mem.read_int64() != 0x0f0e0d0c0b0a0908) fail();
	if (mem.get_position() != 16) fail();
	mem.seek(0);

//*** testing cl_ulong read_uint64()
	CL_Console::write_line("   Function: cl_ulong read_uint64()");
	if (mem.send(test_data, 64, true) != 64) fail();
	mem.seek(0);
	vptr_uint64 = (cl_ulong *) data.get_data();
	mem.set_big_endian_mode();
	if (mem.read_uint64() != 0x0001020304050607) fail();
	mem.set_little_endian_mode();
	if (mem.read_uint64() != 0x0f0e0d0c0b0a0908) fail();
	if (mem.get_position() != 16) fail();
	mem.seek(0);

//*** testing cl_int read_int32()
	CL_Console::write_line("   Function: cl_int read_int32()");
	if (mem.send(test_data, 64, true) != 64) fail();
	mem.seek(0);
	vptr_int32 = (cl_int *) data.get_data();
	mem.set_big_endian_mode();
	if (mem.read_int32() != 0x00010203) fail();
	mem.set_little_endian_mode();
	if (mem.read_int32() != 0x07060504) fail();
	if (mem.get_position() != 8) fail();
	mem.seek(0);

//*** testing cl_uint read_uint32()
	CL_Console::write_line("   Function: cl_uint read_uint32()");
	if (mem.send(test_data, 64, true) != 64) fail();
	mem.seek(0);
	vptr_uint32 = (cl_uint *) data.get_data();
	mem.set_big_endian_mode();
	if (mem.read_uint32() != 0x00010203) fail();
	mem.set_little_endian_mode();
	if (mem.read_uint32() != 0x07060504) fail();
	if (mem.get_position() != 8) fail();
	mem.seek(0);

//*** testing cl_short read_int16()
	CL_Console::write_line("   Function: cl_short read_int16()");
	if (mem.send(test_data, 64, true) != 64) fail();
	mem.seek(0);
	vptr_int16 = (cl_short *) data.get_data();
	mem.set_big_endian_mode();
	if (mem.read_int16() != 0x0001) fail();
	mem.set_little_endian_mode();
	if (mem.read_int16() != 0x0302) fail();
	if (mem.get_position() != 4) fail();
	mem.seek(0);

//*** testing cl_ushort read_uint16()
	CL_Console::write_line("   Function: cl_ushort read_uint16()");
	if (mem.send(test_data, 64, true) != 64) fail();
	mem.seek(0);
	vptr_uint16 = (cl_ushort *) data.get_data();
	mem.set_big_endian_mode();
	if (mem.read_uint16() != 0x0001) fail();
	mem.set_little_endian_mode();
	if (mem.read_uint16() != 0x0302) fail();
	if (mem.get_position() != 4) fail();
	mem.seek(0);

//*** testing cl_char read_int8()
	CL_Console::write_line("   Function: cl_char read_int8()");
	if (mem.send(test_data, 64, true) != 64) fail();
	mem.seek(0);
	vptr_int8 = (cl_char *) data.get_data();
	mem.set_big_endian_mode();
	if (mem.read_int8() != 0x00) fail();
	mem.set_little_endian_mode();
	if (mem.read_int8() != 0x01) fail();
	if (mem.get_position() != 2) fail();
	mem.seek(0);

//*** testing cl_uchar read_uint8()
	CL_Console::write_line("   Function: cl_uchar read_uint8()");
	if (mem.send(test_data, 64, true) != 64) fail();
	mem.seek(0);
	vptr_uint8 = (cl_uchar *) data.get_data();
	mem.set_big_endian_mode();
	if (mem.read_uint8() != 0x00) fail();
	mem.set_little_endian_mode();
	if (mem.read_uint8() != 0x01) fail();
	if (mem.get_position() != 2) fail();
	mem.seek(0);

//*** testing write_float()
	CL_Console::write_line("   Function: write_float(float)");
	// Tests contains in read_float()

//*** testing cl_float read_float()
	CL_Console::write_line("   Function: float read_float()");
	mem.seek(0);
	mem.set_big_endian_mode();
	mem.write_float(1234);
	mem.set_little_endian_mode();
	mem.write_float(4321);
	mem.seek(0);
	mem.set_big_endian_mode();
	if (mem.read_float() != 1234) fail();
	mem.set_little_endian_mode();
	if (mem.read_float() != 4321) fail();
	mem.seek(0);

//*** testing void write_string_a();
	CL_Console::write_line("   Function: void write_string_a(const CL_StringRef8 &str)");
	// Test contained in read_string_a()
//*** testing CL_String8 read_string_a();
	CL_Console::write_line("   Function: CL_String8 read_string_a()");

	CL_StringRef8 str8a = "Hello World";
	CL_String str8b;
	mem.set_big_endian_mode();
	mem.write_string_a(str8a);
	mem.set_little_endian_mode();
	mem.write_string_a(str8a);
	if (data.get_data()[4] != 'H') fail();
	if (mem.get_position() != ( (11+4) *2) ) fail();
	mem.seek(0);
	mem.set_big_endian_mode();
	str8b = mem.read_string_a();
	if (str8b != str8a) fail();
	mem.set_little_endian_mode();
	str8b = mem.read_string_a();
	if (str8b != str8a) fail();
	mem.seek(0);

//*** testing CL_String8 read_string_nul();
	CL_Console::write_line("   Function: CL_String8 read_string_nul()");

	CL_StringRef8 str8c = "Hello World";
	CL_StringRef8 str8d = "ABC";

	mem.write(str8c.c_str(), str8c.length()+1);
	mem.write(str8d.c_str(), str8d.length()+1);
	mem.seek(0);

	str8b = mem.read_string_nul();
	if (str8c != str8b) fail();
	str8b = mem.read_string_nul();
	if (str8d != str8b) fail();
	mem.seek(0);

//*** testing CL_String8 read_string_text();
	CL_Console::write_line("   Function: CL_String8 read_string_text()");

	CL_StringRef8 str8e = "Hello World";
	CL_StringRef8 str8f = "ABC";

	mem.write(str8e.c_str(), str8e.length()+1);
	mem.write(str8f.c_str(), str8f.length()+1);
	mem.seek(0);

	// Test read until NUL #1
	str8b = mem.read_string_text(NULL, NULL, false);
	if (str8e != str8b) fail();
	mem.seek(0);

	// Test read until NUL #2
	str8b = mem.read_string_text(NULL, "A", false);
	if (str8e != str8b) fail();
	mem.seek(0);

	// Test read until NUL #3
	str8b = mem.read_string_text("Hello ", "A", false);
	if ("World" != str8b) fail();
	mem.seek(0);

	// Test read general #1
	str8b = mem.read_string_text("ol eH", "dr", false);
	if ("Wo" != str8b) fail();

	// Test read general #2
	str8b = mem.read_string_text(NULL, "d", false);
	if ("rl" != str8b) fail();
	mem.seek(0);
}

