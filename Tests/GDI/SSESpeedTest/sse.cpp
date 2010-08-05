
#include <ClanLib/core.h>
#include <emmintrin.h>

struct Data
{
	char src[64];
	char dest[64];
	char red[64];
	char green[64];
	char blue[64];
	char alpha[64];
	unsigned int width;
};

void blend1(Data *d)
{
	unsigned int *dest = (unsigned int*)d->dest;
	unsigned int *src = (unsigned int*)d->src;
	unsigned int width = d->width;

	__m128i one, half, zero;
	one = _mm_set1_epi16(0x0100);
	half = _mm_set1_epi16(0x007f);
	zero = _mm_setzero_si128();
	for (unsigned int i = 0; i < 160*1600*100; i++)
	{
		for (unsigned int x = 0; x < width; x+=2)
		{
			__m128i src0, dest0;
			src0 = _mm_loadl_epi64((__m128i*)(src+x));
			dest0 = _mm_loadl_epi64((__m128i*)(dest+x));

			src0 = _mm_unpacklo_epi8(src0, zero);
			dest0 = _mm_unpacklo_epi8(dest0, zero);
			
			__m128i src_alpha, invsrc_alpha;
			src_alpha = src0;
			src_alpha = _mm_shufflelo_epi16(src_alpha, 0xff);
			src_alpha = _mm_shufflehi_epi16(src_alpha, 0xff);
			invsrc_alpha = _mm_sub_epi16(one, src_alpha);
			dest0 = _mm_mullo_epi16(dest0, invsrc_alpha);
			dest0 = _mm_add_epi16(dest0, half);
			dest0 = _mm_srli_epi16(dest0, 8);
			dest0 = _mm_add_epi16(dest0, src0);

			dest0 = _mm_packus_epi16(dest0, zero);
			_mm_storel_epi64((__m128i*)(dest+x), dest0);
		}
	}
}

void blend2(Data *d)
{
	unsigned int *dest = (unsigned int*)d->dest;
	unsigned int *src = (unsigned int*)d->src;
	unsigned int *red = (unsigned int*)d->red;
	unsigned int *green = (unsigned int*)d->green;
	unsigned int *blue = (unsigned int*)d->blue;
	unsigned int *alpha = (unsigned int*)d->alpha;
	unsigned int width = d->width;

	__m128i one, half, zero;
	one = _mm_set1_epi16(0x0100);
	half = _mm_set1_epi16(0x007f);
	zero = _mm_setzero_si128();
	for (unsigned int i = 0; i < 160*1600*100; i++)
	{
		for (unsigned int x = 0; x < width; x+=4)
		{
			__m128i src0, dest0, srcwrk, destwrk, tmp;
			src0 = _mm_load_si128((__m128i*)(src+x));
			dest0 = _mm_load_si128((__m128i*)(dest+x));

			srcwrk = _mm_unpacklo_epi8(src0, zero);
			destwrk = _mm_unpacklo_epi8(dest0, zero);
			
			__m128i src_alpha, invsrc_alpha;
			src_alpha = srcwrk;
			src_alpha = _mm_shufflelo_epi16(src_alpha, 0xff);
			src_alpha = _mm_shufflehi_epi16(src_alpha, 0xff);
			invsrc_alpha = _mm_sub_epi16(one, src_alpha);
			destwrk = _mm_mullo_epi16(destwrk, invsrc_alpha);
			destwrk = _mm_add_epi16(destwrk, half);
			destwrk = _mm_srli_epi16(destwrk, 8);
			destwrk = _mm_add_epi16(destwrk, srcwrk);
			srcwrk = _mm_unpacklo_epi8(src0, zero);
			destwrk = _mm_unpacklo_epi8(dest0, zero);
			tmp = _mm_unpackhi_epi8(dest0, zero);
			dest0 = _mm_packus_epi16(destwrk, tmp);

			srcwrk = _mm_unpackhi_epi8(src0, zero);
			destwrk = tmp;
			src_alpha = srcwrk;
			src_alpha = _mm_shufflelo_epi16(src_alpha, 0xff);
			src_alpha = _mm_shufflehi_epi16(src_alpha, 0xff);
			invsrc_alpha = _mm_sub_epi16(one, src_alpha);
			destwrk = _mm_mullo_epi16(destwrk, invsrc_alpha);
			destwrk = _mm_add_epi16(destwrk, half);
			destwrk = _mm_srli_epi16(destwrk, 8);
			destwrk = _mm_add_epi16(destwrk, srcwrk);
			srcwrk = _mm_unpacklo_epi8(src0, zero);
			destwrk = _mm_unpacklo_epi8(dest0, zero);
			tmp = _mm_unpacklo_epi8(dest0, zero);
			dest0 = _mm_packus_epi16(tmp, destwrk);

			_mm_store_si128((__m128i*)(dest+x), dest0);
		}
	}
}

void transpose1(Data *d)
{
	unsigned int *dest = (unsigned int*)d->dest;
	unsigned int *src = (unsigned int*)d->src;
	unsigned int *red = (unsigned int*)d->red;
	unsigned int *green = (unsigned int*)d->green;
	unsigned int *blue = (unsigned int*)d->blue;
	unsigned int *alpha = (unsigned int*)d->alpha;
	unsigned int width = d->width;

	__m128i one, half, zero;
	one = _mm_set1_epi16(0x0100);
	half = _mm_set1_epi16(0x007f);
	zero = _mm_setzero_si128();
	for (unsigned int i = 0; i < 160*1600*100; i++)
	{
		for (unsigned int x = 0; x < width; x+=4)
		{
			__m128i dest0, tmp0, row0, row1, row2, row3;
			dest0 = _mm_load_si128((__m128i*)(dest+x));

			tmp0 = _mm_unpacklo_epi8(dest0, zero);
			row0 = _mm_unpacklo_epi16(tmp0, zero);
			row1 = _mm_unpackhi_epi16(tmp0, zero);

			tmp0 = _mm_unpackhi_epi8(dest0, zero);
			row2 = _mm_unpacklo_epi16(tmp0, zero);
			row3 = _mm_unpackhi_epi16(tmp0, zero);

			__m128 row0ps, row1ps, row2ps, row3ps;
			row0ps = _mm_castsi128_ps(row0);
			row1ps = _mm_castsi128_ps(row1);
			row2ps = _mm_castsi128_ps(row2);
			row3ps = _mm_castsi128_ps(row3);
			_MM_TRANSPOSE4_PS(row0ps, row1ps, row2ps, row3ps);
			row0 = _mm_castps_si128(row0ps);
			row1 = _mm_castps_si128(row1ps);
			row2 = _mm_castps_si128(row2ps);
			row3 = _mm_castps_si128(row3ps);

			_mm_store_si128((__m128i*)(red+x), row0);
			_mm_store_si128((__m128i*)(green+x), row1);
			_mm_store_si128((__m128i*)(blue+x), row2);
			_mm_store_si128((__m128i*)(alpha+x), row3);
	
			// dest0 [ aarr ggbb aarr ggbb  aarr ggbb aarr ggbb ]
			// tmp0  [ 00aa 00rr 00gg 00bb  00aa 00rr 00gg 00bb ]
			// row0  [ 0000 00aa 0000 00rr  0000 00gg 0000 00bb ]
			// row1  [ 0000 00aa 0000 00rr  0000 00gg 0000 00bb ]
		}
	}
}

unsigned int sampleNearestRepeat(float x, float y, int texwidth, int texheight, unsigned int *texdata)
{
	int tx = (int)x;
	int ty = (int)y;
	tx = (int)((x-tx)*texwidth);
	ty = (int)((y-ty)*texheight);
	if (tx < 0)
		tx += texwidth;
	if (ty < 0)
		ty += texheight;
	int texoffset = tx+ty*texwidth;
	return texdata[texoffset];
}

void nearest1(Data *d)
{
	unsigned int *dest = (unsigned int*)d->dest;
	unsigned int *src = (unsigned int*)d->src;
	unsigned int width = d->width;

	float tx = 0.2f;
	float ty = 0.2f;
	float dtx = 0.04f;
	float dty = 0.04f;

	__m128i one, half, zero;
	one = _mm_set1_epi16(0x0100);
	half = _mm_set1_epi16(0x007f);
	zero = _mm_setzero_si128();
	for (unsigned int i = 0; i < 160*1600*100; i++)
	{
		for (unsigned int x = 0; x < width; x+=4)
		{
			unsigned int s0 = sampleNearestRepeat(tx, ty, 5, 3, src); tx += dtx; ty += dty;
			unsigned int s1 = sampleNearestRepeat(tx, ty, 5, 3, src); tx += dtx; ty += dty;
			unsigned int s2 = sampleNearestRepeat(tx, ty, 5, 3, src); tx += dtx; ty += dty;
			unsigned int s3 = sampleNearestRepeat(tx, ty, 5, 3, src); tx += dtx; ty += dty;

			__m128i src0, dest0, srcwrk, destwrk, tmp;
			src0 = _mm_set_epi32(s3,s2,s1,s0);
			dest0 = _mm_load_si128((__m128i*)(dest+x));

			srcwrk = _mm_unpacklo_epi8(src0, zero);
			destwrk = _mm_unpacklo_epi8(dest0, zero);
			
			__m128i src_alpha, invsrc_alpha;
			src_alpha = srcwrk;
			src_alpha = _mm_shufflelo_epi16(src_alpha, 0xff);
			src_alpha = _mm_shufflehi_epi16(src_alpha, 0xff);
			invsrc_alpha = _mm_sub_epi16(one, src_alpha);
			destwrk = _mm_mullo_epi16(destwrk, invsrc_alpha);
			destwrk = _mm_add_epi16(destwrk, half);
			destwrk = _mm_srli_epi16(destwrk, 8);
			destwrk = _mm_add_epi16(destwrk, srcwrk);
			srcwrk = _mm_unpacklo_epi8(src0, zero);
			destwrk = _mm_unpacklo_epi8(dest0, zero);
			tmp = _mm_unpackhi_epi8(dest0, zero);
			dest0 = _mm_packus_epi16(destwrk, tmp);

			srcwrk = _mm_unpackhi_epi8(src0, zero);
			destwrk = tmp;
			src_alpha = srcwrk;
			src_alpha = _mm_shufflelo_epi16(src_alpha, 0xff);
			src_alpha = _mm_shufflehi_epi16(src_alpha, 0xff);
			invsrc_alpha = _mm_sub_epi16(one, src_alpha);
			destwrk = _mm_mullo_epi16(destwrk, invsrc_alpha);
			destwrk = _mm_add_epi16(destwrk, half);
			destwrk = _mm_srli_epi16(destwrk, 8);
			destwrk = _mm_add_epi16(destwrk, srcwrk);
			srcwrk = _mm_unpacklo_epi8(src0, zero);
			destwrk = _mm_unpacklo_epi8(dest0, zero);
			tmp = _mm_unpacklo_epi8(dest0, zero);
			dest0 = _mm_packus_epi16(tmp, destwrk);
			
			_mm_store_si128((__m128i*)(dest+x), dest0);
		}
	}
}

unsigned int sampleLinearRepeat(float x, float y, int texwidth, int texheight, unsigned int *texdata)
{
	int tx = (int)x;
	int ty = (int)y;
	tx = (int)((x-tx)*texwidth);
	ty = (int)((y-ty)*texheight);
	if (tx < 0)
		tx += texwidth;
	if (ty < 0)
		ty += texheight;
	int texoffset = tx+ty*texwidth;

	unsigned int p1 = texdata[texoffset];
	unsigned int p2 = texdata[texoffset+1];
	unsigned int p3 = texdata[texoffset+texwidth];
	unsigned int p4 = texdata[texoffset+texwidth+1];
	return p1+p2+p3+p4;
}

void linear1(Data *d)
{
	unsigned int *dest = (unsigned int*)d->dest;
	unsigned int *src = (unsigned int*)d->src;
	unsigned int width = d->width;

	float tx = 0.2f;
	float ty = 0.2f;
	float dtx = 0.04f;
	float dty = 0.04f;

	__m128i one, half, zero;
	one = _mm_set1_epi16(0x0100);
	half = _mm_set1_epi16(0x007f);
	zero = _mm_setzero_si128();
	for (unsigned int i = 0; i < 160*1600*100; i++)
	{
		for (unsigned int x = 0; x < width; x+=4)
		{
			unsigned int s0 = sampleLinearRepeat(tx, ty, 5, 3, src); tx += dtx; ty += dty;
			unsigned int s1 = sampleLinearRepeat(tx, ty, 5, 3, src); tx += dtx; ty += dty;
			unsigned int s2 = sampleLinearRepeat(tx, ty, 5, 3, src); tx += dtx; ty += dty;
			unsigned int s3 = sampleLinearRepeat(tx, ty, 5, 3, src); tx += dtx; ty += dty;

			__m128i src0, dest0, srcwrk, destwrk, tmp;
			src0 = _mm_set_epi32(s3,s2,s1,s0);
			dest0 = _mm_load_si128((__m128i*)(dest+x));

			srcwrk = _mm_unpacklo_epi8(src0, zero);
			destwrk = _mm_unpacklo_epi8(dest0, zero);
			
			__m128i src_alpha, invsrc_alpha;
			src_alpha = srcwrk;
			src_alpha = _mm_shufflelo_epi16(src_alpha, 0xff);
			src_alpha = _mm_shufflehi_epi16(src_alpha, 0xff);
			invsrc_alpha = _mm_sub_epi16(one, src_alpha);
			destwrk = _mm_mullo_epi16(destwrk, invsrc_alpha);
			destwrk = _mm_add_epi16(destwrk, half);
			destwrk = _mm_srli_epi16(destwrk, 8);
			destwrk = _mm_add_epi16(destwrk, srcwrk);
			srcwrk = _mm_unpacklo_epi8(src0, zero);
			destwrk = _mm_unpacklo_epi8(dest0, zero);
			tmp = _mm_unpackhi_epi8(dest0, zero);
			dest0 = _mm_packus_epi16(destwrk, tmp);

			srcwrk = _mm_unpackhi_epi8(src0, zero);
			destwrk = tmp;
			src_alpha = srcwrk;
			src_alpha = _mm_shufflelo_epi16(src_alpha, 0xff);
			src_alpha = _mm_shufflehi_epi16(src_alpha, 0xff);
			invsrc_alpha = _mm_sub_epi16(one, src_alpha);
			destwrk = _mm_mullo_epi16(destwrk, invsrc_alpha);
			destwrk = _mm_add_epi16(destwrk, half);
			destwrk = _mm_srli_epi16(destwrk, 8);
			destwrk = _mm_add_epi16(destwrk, srcwrk);
			srcwrk = _mm_unpacklo_epi8(src0, zero);
			destwrk = _mm_unpacklo_epi8(dest0, zero);
			tmp = _mm_unpacklo_epi8(dest0, zero);
			dest0 = _mm_packus_epi16(tmp, destwrk);
			
			_mm_store_si128((__m128i*)(dest+x), dest0);
		}
	}
}

void time_algorithm(const CL_String &name, Data *data, void(*func)(Data *))
{
	unsigned int start = CL_System::get_time();
	func(data);
	unsigned int end = CL_System::get_time();
	CL_Console::write_line("%3: %1 ms elapsed, %2 fps", end-start, 100000/(end-start), name);
}

int main(int, char **)
{
	Data *d = (Data*)_aligned_malloc(sizeof(Data), 64);
	d->width = 16;

	time_algorithm("blend1", d, blend1);
	time_algorithm("blend2", d, blend2);
	time_algorithm("nearest1", d, nearest1);
	time_algorithm("linear1", d, linear1);
	time_algorithm("transpose1", d, transpose1);

	_aligned_free(d);
	return 0;
}
