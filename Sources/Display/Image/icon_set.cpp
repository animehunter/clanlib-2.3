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
#include "API/Display/Image/icon_set.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Core/IOData/datatypes.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/IOData/iodevice_memory.h"

class CL_IconSet_Impl
{
public:
	std::vector<CL_PixelBuffer> images;

	static CL_DataBuffer create_ico_helper(const std::vector<CL_PixelBuffer> &images, int type, const std::vector<CL_Point> &hotspots);
	static CL_PixelBuffer create_bitmap_data(const CL_PixelBuffer &image);

	struct IconHeader
	{
		cl_uint16 idReserved;
		cl_uint16 idType; // 1 = ICO, 2 = CUR
		cl_uint16 idCount;
	};

	struct IconDirectoryEntry
	{
		cl_uint8 bWidth;
		cl_uint8 bHeight;
		cl_uint8 bColorCount;
		cl_uint8 bReserved;
		union
		{
			cl_uint16 wPlanes;   // ICO format
			cl_int16 XHotspot;   // CUR format
		};
		union
		{
			cl_uint16 wBitCount; // ICO format
			cl_int16 YHotspot;   // CUR format
		};
		cl_uint32 dwBytesInRes;
		cl_uint32 dwImageOffset;
		/** cl_uint16 nID; // Mentioned by http://msdn2.microsoft.com/en-us/library/ms997538.aspx but not in other ICO docs.*/
	};

	struct IconBitmapInfoHeader
	{
        cl_uint32 biSize;
        cl_int32 biWidth;
        cl_int32 biHeight;
        cl_uint16 biPlanes;
        cl_uint16 biBitCount;
        cl_uint32 biCompression;
        cl_uint32 biSizeImage;
        cl_int32 biXPelsPerMeter;
        cl_int32 biYPelsPerMeter;
        cl_uint32 biClrUsed;
        cl_uint32 biClrImportant;
	};

	enum IconBitmapInfoCompressionType
	{
		bi_rgb = 0,
		bi_rle8 = 1,
		bi_rle4 = 2,
		bi_bitfields = 3,
		bi_jpeg = 4,
		bi_png = 5
	};

	enum { size_header = 6, size_direntry = 16, size_bitmap_info = 40 };
};

CL_IconSet::CL_IconSet()
: impl(new CL_IconSet_Impl)
{
}

std::vector<CL_PixelBuffer> CL_IconSet::get_images() const
{
	return impl->images;
}

void CL_IconSet::add_image(const CL_PixelBuffer &image)
{
	impl->images.push_back(image);
}

CL_DataBuffer CL_IconSet::create_win32_icon()
{
	return impl->create_ico_helper(impl->images, 1, std::vector<CL_Point>());
}

CL_DataBuffer CL_IconSet_Impl::create_ico_helper(const std::vector<CL_PixelBuffer> &images, int type, const std::vector<CL_Point> &hotspots)
{
	CL_DataBuffer buf;
	buf.set_capacity(32*1024);
	CL_IODevice_Memory device(buf);

	IconHeader header;
	memset(&header, 0, sizeof(IconHeader));
	header.idType = type;
	header.idCount = images.size();
	device.write(&header, sizeof(IconHeader));

	std::vector<CL_PixelBuffer> bmp_images;
	for (size_t i = 0; i < images.size(); i++)
		bmp_images.push_back(create_bitmap_data(images[i]));

	int image_offset = size_header + size_direntry*bmp_images.size();
	for (size_t i = 0; i < bmp_images.size(); i++)
	{
		IconDirectoryEntry entry;
		memset(&entry, 0, sizeof(IconDirectoryEntry));
		entry.bWidth = bmp_images[i].get_width();
		entry.bHeight = bmp_images[i].get_height();
		entry.bColorCount = 0;
		entry.wPlanes = 1;
		entry.wBitCount = 32;
		entry.dwBytesInRes = size_bitmap_info + bmp_images[i].get_pitch() * bmp_images[i].get_height();
		entry.dwImageOffset = image_offset;
		if (type == 2)
		{
			entry.XHotspot = hotspots[i].x;
			entry.YHotspot = hotspots[i].y;
		}
		device.write(&entry, size_direntry);
		image_offset += entry.dwBytesInRes;
	}

	for (size_t i = 0; i < bmp_images.size(); i++)
	{
		IconBitmapInfoHeader bmp_header;
		memset(&bmp_header, 0, sizeof(IconBitmapInfoHeader));
		bmp_header.biSize = size_bitmap_info;
		bmp_header.biWidth = bmp_images[i].get_width();
		bmp_header.biHeight = bmp_images[i].get_height() * 2; // why on earth do I have to multiply this by two??
		bmp_header.biPlanes = 1;
		bmp_header.biBitCount = 32;
		bmp_header.biCompression = bi_rgb;
		device.write(&bmp_header, size_bitmap_info);
		device.write(bmp_images[i].get_data(), bmp_images[i].get_pitch() * bmp_images[i].get_height());
	}

	return device.get_data();
}

CL_PixelBuffer CL_IconSet_Impl::create_bitmap_data(const CL_PixelBuffer &image)
{
	// Convert pixel buffer to DIB compatible format:

	CL_PixelBuffer bmp_image(image.get_width(), image.get_height(), cl_argb8);

	image.convert(bmp_image, CL_Rect(0, 0, bmp_image.get_width(), bmp_image.get_height()));

	// Note that the APIs use pre-multiplied alpha, which means that the red,
	// green and blue channel values in the bitmap must be pre-multiplied with
	// the alpha channel value. For example, if the alpha channel value is x,
	// the red, green and blue channels must be multiplied by x and divided by
	// 0xff prior to the call.
	int w = bmp_image.get_width();
	int h = bmp_image.get_height();
	unsigned int *p = (unsigned int *) bmp_image.get_data();
	for (int y = 0; y < h; y++)
	{
		int index = y * w;
		unsigned int *line = p + index;
		for (int x = 0; x < w; x++)
		{
			unsigned int a = ((line[x] >> 24) & 0xff);
			unsigned int r = ((line[x] >> 16) & 0xff);
			unsigned int g = ((line[x] >> 8) & 0xff);
			unsigned int b = (line[x] & 0xff);

			r = r * a / 255;
			g = g * a / 255;
			b = b * a / 255;

			line[x] = (a << 24) + (r << 16) + (g << 8) + b;
		}
	}

	// Flip image upside down
	for (int y = 0; y < h/2; y++)
	{
		for (int x = 0; x < w; x++)
		{
			unsigned int l1 = p[y*w+x];
			unsigned int l2 = p[(w-1-y)*w+x];
			p[(w-1-y)*w+x] = l1;
			p[y*w+x] = l2;
		}
	}

	return bmp_image;
}
