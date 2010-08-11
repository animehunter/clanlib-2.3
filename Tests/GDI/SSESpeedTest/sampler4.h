
inline void sampleLinearRepeat4(int xx0, int yy0, int xx1, int yy1, int texwidth, __m128i &clampmax, unsigned int *texdata, __m128i &pixel)
{
	__m128i tmp0, tmp1, pos, frac, inv_frac, c0213;
	tmp0 = _mm_set_epi32(yy1, xx1, yy0, xx0);

	pos = _mm_srai_epi32(tmp0, 15);
	frac = _mm_and_si128(_mm_srai_epi32(tmp0, 8), _mm_set1_epi32(0x7f));
	inv_frac = _mm_sub_epi32(_mm_set1_epi32(128), frac);

	// frac =     [000000aa 000000bb  000000aa  000000bb]
	// inv_frac = [000000ia 000000ib  000000ia  000000ib]
	// tmp0 =     [00aa 00bb 00aa 00bb  00ia 00ib 00ia 00ib]
	// tmp0 =     [00aa 00bb 00ia 00ib  00aa 00bb 00ia 00ib]
	// tmp1 =     [00bb 00ib 00aa 00ia  00bb 00ib 00aa 00ia]
	// c0213 =    [ c3   c1   c2   c0    c3   c1   c2   c0 ]
	//
	// int c0 = (inv_a*inv_b+64)>>7;
	// int c1 = (a*inv_b+64)>>7;
	// int c2 = (inv_a*b)>>7;
	// int c3 = (a*b)>>7;

	tmp0 = _mm_packs_epi32(inv_frac, frac);
	tmp0 = _mm_shuffle_epi32(tmp0, _MM_SHUFFLE(3,1,2,0));
	tmp1 = _mm_shufflehi_epi16(tmp0, _MM_SHUFFLE(2,0,3,1));
	tmp1 = _mm_shufflelo_epi16(tmp1, _MM_SHUFFLE(2,0,3,1));
	c0213 = _mm_mullo_epi16(tmp0, tmp1);
	c0213 = _mm_add_epi16(c0213, _mm_set_epi16(0, 64, 0, 64, 0, 64, 0, 64));
	c0213 = _mm_srli_epi16(c0213, 7);
	//c0213 = _mm_set_epi16(0, 0, 0, 128, 0, 0, 0, 128);

	// wrapClampToEdge:

	pos = _mm_packs_epi32(pos, pos);
	pos = _mm_add_epi16(pos, _mm_set_epi16(1, 1, 1, 1, 0, 0, 0, 0));
	pos = _mm_max_epi16(pos, _mm_setzero_si128());
	pos = _mm_min_epi16(pos, clampmax);
	short texpos[8]; // x0, y0, x1, y1, x0+1, y0+1, x1+1, y1+1
	_mm_storeu_si128((__m128i*)texpos, pos);

	int lineoffset1 = texpos[1]*texwidth;
	int lineoffset2 = texpos[5]*texwidth;
	int lineoffset3 = texpos[3]*texwidth;
	int lineoffset4 = texpos[7]*texwidth;

	tmp0 = _mm_set_epi32(0, 0, texdata[texpos[4]+lineoffset3], texdata[texpos[0]+lineoffset1]);
	tmp0 = _mm_unpacklo_epi8(tmp0, _mm_setzero_si128());
	tmp1 = _mm_shufflehi_epi16(c0213, _MM_SHUFFLE(0,0,0,0));
	tmp1 = _mm_shufflelo_epi16(tmp1, _MM_SHUFFLE(0,0,0,0));
	pixel = _mm_mullo_epi16(tmp0, tmp1);

	tmp0 = _mm_set_epi32(0, 0, texdata[texpos[6]+lineoffset3], texdata[texpos[2]+lineoffset1]);
	tmp0 = _mm_unpacklo_epi8(tmp0, _mm_setzero_si128());
	tmp1 = _mm_shufflehi_epi16(c0213, _MM_SHUFFLE(2,2,2,2));
	tmp1 = _mm_shufflelo_epi16(tmp1, _MM_SHUFFLE(2,2,2,2));
	pixel = _mm_add_epi16(pixel, _mm_mullo_epi16(tmp0, tmp1));

	tmp0 = _mm_set_epi32(0, 0, texdata[texpos[4]+lineoffset4], texdata[texpos[0]+lineoffset2]);
	tmp0 = _mm_unpacklo_epi8(tmp0, _mm_setzero_si128());
	tmp1 = _mm_shufflehi_epi16(c0213, _MM_SHUFFLE(1,1,1,1));
	tmp1 = _mm_shufflelo_epi16(tmp1, _MM_SHUFFLE(1,1,1,1));
	pixel = _mm_add_epi16(pixel, _mm_mullo_epi16(tmp0, tmp1));

	tmp0 = _mm_set_epi32(0, 0, texdata[texpos[6]+lineoffset4], texdata[texpos[2]+lineoffset2]);
	tmp0 = _mm_unpacklo_epi8(tmp0, _mm_setzero_si128());
	tmp1 = _mm_shufflehi_epi16(c0213, _MM_SHUFFLE(3,3,3,3));
	tmp1 = _mm_shufflelo_epi16(tmp1, _MM_SHUFFLE(3,3,3,3));
	pixel = _mm_add_epi16(pixel, _mm_mullo_epi16(tmp0, tmp1));

	pixel = _mm_add_epi16(pixel, _mm_srli_epi16(pixel, 8));
	pixel = _mm_add_epi16(pixel, _mm_set1_epi16(0x003f));
	pixel = _mm_srli_epi16(pixel, 7);
	pixel = _mm_packus_epi16(pixel, _mm_setzero_si128());
}
