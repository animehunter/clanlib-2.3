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
#include "cursor_provider_win32.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/IOData/iodevice_memory.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/2D/sprite_description.h"
#include "win32_window.h"

/////////////////////////////////////////////////////////////////////////////
// CL_CursorProvider_Win32 Construction:

CL_CursorProvider_Win32::CL_CursorProvider_Win32(const CL_SpriteDescription &sprite_description, const CL_Point &hotspot)
: handle(0)
{
	handle = create_cursor(sprite_description, hotspot);
}

CL_CursorProvider_Win32::~CL_CursorProvider_Win32()
{
	if (handle)
		DestroyCursor(handle);
}

/////////////////////////////////////////////////////////////////////////////
// CL_CursorProvider_Win32 Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_CursorProvider_Win32 Operations:

/////////////////////////////////////////////////////////////////////////////
// CL_CursorProvider_Win32 Implementation:

HCURSOR CL_CursorProvider_Win32::create_cursor(const CL_SpriteDescription &sprite_description, const CL_Point &hotspot)
{
	if (sprite_description.get_frames().empty())
		throw CL_Exception("Cannot create cursor with no image frames");
	CL_DataBuffer ani_file = create_ani_file(sprite_description, hotspot);
	int desired_width = sprite_description.get_frames().front().rect.get_width();
	int desired_height = sprite_description.get_frames().front().rect.get_height();
	HICON icon = CreateIconFromResourceEx((PBYTE) ani_file.get_data(), ani_file.get_size(), FALSE, 0x00030000, desired_width, desired_height, LR_DEFAULTCOLOR);
	return (HCURSOR) icon;
}

CL_DataBuffer CL_CursorProvider_Win32::create_ico_file(const CL_PixelBuffer &image)
{
	return create_ico_helper(image, CL_Rectf(image.get_size()), 1, CL_Point(0, 0));
}

CL_DataBuffer CL_CursorProvider_Win32::create_cur_file(const CL_PixelBuffer &image, const CL_Rect &rect, const CL_Point &hotspot)
{
	return create_ico_helper(image, rect, 2, hotspot);
}

CL_DataBuffer CL_CursorProvider_Win32::create_ico_helper(const CL_PixelBuffer &image, const CL_Rect &rect, WORD type, const CL_Point &hotspot)
{
	std::vector<CL_PixelBuffer> images;
	std::vector<CL_Rect> rects;
	std::vector<CL_Point> hotspots;
	images.push_back(image);
	rects.push_back(rect);
	hotspots.push_back(hotspot);
	return create_ico_helper(images, rects, type, hotspots);
}

CL_DataBuffer CL_CursorProvider_Win32::create_ico_helper(const std::vector<CL_PixelBuffer> &images, const std::vector<CL_Rect> &rects, WORD type, const std::vector<CL_Point> &hotspots)
{
	CL_DataBuffer buf;
	buf.set_capacity(32*1024);
	CL_IODevice_Memory device(buf);

	ICONHEADER header;
	memset(&header, 0, sizeof(ICONHEADER));
	header.idType = type;
	header.idCount = images.size();
	device.write(&header, sizeof(ICONHEADER));

	std::vector<CL_PixelBuffer> bmp_images;
	for (size_t i = 0; i < images.size(); i++)
		bmp_images.push_back(CL_Win32Window::create_bitmap_data(images[i], rects[i]));

	for (size_t i = 0; i < bmp_images.size(); i++)
	{
		IconDirectoryEntry entry;
		memset(&entry, 0, sizeof(IconDirectoryEntry));
		entry.bWidth = bmp_images[i].get_width();
		entry.bHeight = bmp_images[i].get_height();
		entry.bColorCount = 0;
		entry.wPlanes = 1;
		entry.wBitCount = 32;
		entry.dwBytesInRes = sizeof(BITMAPINFOHEADER) + bmp_images[i].get_pitch() * bmp_images[i].get_height();
		entry.dwImageOffset = size_header + size_direntry*bmp_images.size();
		if (type == 2)
		{
			entry.XHotspot = hotspots[i].x;
			entry.YHotspot = hotspots[i].y;
		}
		device.write(&entry, sizeof(IconDirectoryEntry));
	}

	for (size_t i = 0; i < bmp_images.size(); i++)
	{
		BITMAPINFOHEADER bmp_header;
		memset(&bmp_header, 0, sizeof(BITMAPINFOHEADER));
		bmp_header.biSize = sizeof(BITMAPINFOHEADER);
		bmp_header.biWidth = bmp_images[i].get_width();
		bmp_header.biHeight = bmp_images[i].get_height()*2; // why on earth do I have to multiply this by two??
		bmp_header.biPlanes = 1;
		bmp_header.biBitCount = 32;
		bmp_header.biCompression = BI_RGB;
		bmp_header.biSizeImage = bmp_images[i].get_pitch() * bmp_images[i].get_height();
		device.write(&bmp_header, sizeof(BITMAPINFOHEADER));
		device.write(bmp_images[i].get_data(), bmp_images[i].get_pitch() * bmp_images[i].get_height());
	}

	return device.get_data();
}

CL_DataBuffer CL_CursorProvider_Win32::create_ani_file(const CL_SpriteDescription &sprite_description, const CL_Point &hotspot)
{
/*
	"RIFF" {Length of File}
		"ACON"
		"LIST" {Length of List}
			"INFO"
				"INAM" {Length of Title} {Data}
				"IART" {Length of Author} {Data}
		"anih" {Length of ANI header (36 bytes)} {Data}   ; (see ANI Header TypeDef )
		"rate" {Length of rate block} {Data}      ; ea. rate is a long (length is 1 to cSteps)
		"seq " {Length of sequence block} {Data} ; ea. seq is a long (length is 1 to cSteps)
		"LIST" {Length of List}
			"fram"
				"icon" {Length of Icon} {Data}      ; 1st in list
				...
				"icon" {Length of Icon} {Data}      ; Last in list  (1 to cFrames)
*/

	ANIFrames ani_frames;
	std::vector<DWORD> rates;
	std::vector<DWORD> steps;

	const std::vector<CL_SpriteDescriptionFrame> &frames = sprite_description.get_frames();
	for (std::vector<CL_SpriteDescriptionFrame>::size_type i = 0; i < frames.size(); i++)
	{
		if (frames[i].type != CL_SpriteDescriptionFrame::type_pixelbuffer)
			throw CL_Exception("Only pixel buffer sprite frames currently supported for cursors");

		CL_DataBuffer ico_file = create_cur_file(frames[i].pixelbuffer, frames[i].rect, hotspot);
		ani_frames.icons.push_back(ico_file);
		DWORD rate = static_cast<DWORD>(frames[i].delay * 60);
		if (rate == 0)
			rate = 1;
		rates.push_back(rate);
		steps.push_back(i);
	}

	ANIHeader ani_header;
	memset(&ani_header, 0, sizeof(ANIHeader));
	ani_header.cbSizeOf = sizeof(ANIHeader);
	ani_header.flags = AF_ICON;
	ani_header.JifRate = 30;
	ani_header.cBitCount = 32;
	ani_header.cPlanes = 1;
	ani_header.cFrames = ani_frames.icons.size();
	ani_header.cSteps = steps.size();
	ani_header.cx = hotspot.x;
	ani_header.cy = hotspot.y;

	ANIInfo ani_info;
	ani_info.author = "clanlib";
	ani_info.title = "clanlib";


	int size_file_header = 8 + 4;
	int size_list1 = 8 + ani_info.length();
	int size_anih = 8 + sizeof(ANIHeader);
	int size_rate = 8 + ani_header.cSteps * 4;
	int size_seq = 8 + ani_header.cSteps * 4;
	int size_list2 = 8 + ani_frames.length();
	int size = size_file_header + size_list1 + size_anih + size_rate + size_seq + size_list2;

	CL_DataBuffer ani_file(size);
	char *data = ani_file.get_data();

	set_riff_header(data, "RIFF", size);
	memcpy(data + 8, "ACON", 4);
	data += size_file_header;

	set_riff_header(data, "LIST", size_list1);
	ani_info.write(data + 8);
	data += size_list1;

	set_riff_header(data, "anih", size_anih);
	memcpy(data + 8, &ani_header, sizeof(ANIHeader));
	data += size_anih;

	set_riff_header(data, "rate", size_rate);
	DWORD *rate = (DWORD *) (data + 8);
	for (DWORD i = 0; i < ani_header.cSteps; i++)
		rate[i] = rates[i];
	data += size_rate;
	
	set_riff_header(data, "seq ", size_seq);
	DWORD *seq = (DWORD *) (data + 8);
	for (DWORD i = 0; i < ani_header.cSteps; i++)
		seq[i] = steps[i];
	data += size_rate;

	set_riff_header(data, "LIST", size_list2);
	ani_frames.write(data + 8);
	data += size_list2;

	return ani_file;
}

void CL_CursorProvider_Win32::set_riff_header(char *data, const char *type, DWORD size)
{
	memcpy(data, type, 4);
	DWORD *s = (DWORD *) (data + 4);
	*s = size - 8;
}
