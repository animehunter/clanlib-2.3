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

class CL_JPEGLoader;
class CL_JPEGMCUDecoder;

class CL_JPEGRGBDecoder
{
public:
	CL_JPEGRGBDecoder(CL_JPEGLoader *loader);
	~CL_JPEGRGBDecoder();

	void decode(CL_JPEGMCUDecoder *mcu_decoder);

	int get_width() const { return mcu_x*8; }
	int get_height() const { return mcu_y*8; }
	const unsigned int *get_pixels() const { return pixels; }

private:
	void upsample(CL_JPEGMCUDecoder *mcu_decoder);
	void convert_monochrome();
	void convert_ycrcb_sse();
	void convert_ycrcb_float();

	CL_JPEGLoader *loader;
	int mcu_x, mcu_y;
	unsigned int *pixels;
	std::vector<unsigned char *> channels;
};
