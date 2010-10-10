
struct FragmentQuadInput
{
	FragmentQuadInput(__m128 tx, __m128 ty) : tx(tx), ty(ty) { }

	__m128 tx;
	__m128 ty;
};

struct FragmentQuadOutput
{
	FragmentQuadOutput(__m128i color0) : color0(color0) { }

	__m128i color0;
};

FragmentQuadInput createFragmentQuad(Scanline &scanline, __m128 xnormalized)
{
	__m128 x2 = _mm_set1_ps(scanline.x2);
	__m128 w2 = _mm_set1_ps(scanline.w2);
	__m128 dx = _mm_sub_ps(_mm_set1_ps(scanline.x2), _mm_set1_ps(scanline.x1));
	__m128 dw = _mm_sub_ps(_mm_set1_ps(scanline.w2), _mm_set1_ps(scanline.w1));

	__m128 a = _mm_mul_ps(_mm_sub_ps(x2, _mm_mul_ps(xnormalized, w2)), _mm_rcp_ps(_mm_sub_ps(dx, _mm_mul_ps(xnormalized, dw))));
	__m128 inv_a = _mm_sub_ps(_mm_set1_ps(1.0f), a);

	__m128 tx1 = _mm_set1_ps(scanline.tx1);
	__m128 tx2 = _mm_set1_ps(scanline.tx2);
	__m128 ty1 = _mm_set1_ps(scanline.ty1);
	__m128 ty2 = _mm_set1_ps(scanline.ty2);
	__m128 tx = _mm_add_ps(_mm_mul_ps(a, tx1), _mm_mul_ps(inv_a, tx2));
	__m128 ty = _mm_add_ps(_mm_mul_ps(a, ty1), _mm_mul_ps(inv_a, ty2));

	return FragmentQuadInput(tx, ty);
}

__m128i textureClampToEdge(__m128i v, __m128i size)
{
	v = _mm_min_epi16(v, _mm_sub_epi32(size, _mm_set1_epi32(1)));
	v = _mm_max_epi16(v, _mm_setzero_si128());
	return v;
}

__m128i sampleNearest(Scanline &scanline, __m128 tx, __m128 ty)
{
	__m128 texwidth = _mm_cvtepi32_ps(_mm_set1_epi32(scanline.src_width));
	__m128 texheight = _mm_cvtepi32_ps(_mm_set1_epi32(scanline.src_height));
	__m128i itexwidth = _mm_set1_epi32(scanline.src_width);
	__m128i itexheight = _mm_set1_epi32(scanline.src_height);

	__m128 x = _mm_mul_ps(tx, texwidth);
	__m128 y = _mm_mul_ps(ty, texheight);
	__m128i ix0 = _mm_cvttps_epi32(x);
	__m128i iy0 = _mm_cvttps_epi32(y);

	ix0 = textureClampToEdge(ix0, itexwidth);
	iy0 = textureClampToEdge(iy0, itexheight);

	__m128i ilineoffset0 = _mm_or_si128(_mm_slli_epi32(_mm_mulhi_epu16(iy0, itexwidth), 16), _mm_mullo_epi16(iy0, itexwidth));
	__m128i ioffsetp0 = _mm_add_epi32(ix0, ilineoffset0);

	__declspec(align(16)) unsigned int offsetp0[4];
	_mm_store_si128((__m128i*)offsetp0, ioffsetp0);

	unsigned int *texdata = scanline.src;
	return _mm_set_epi32(texdata[offsetp0[3]], texdata[offsetp0[2]], texdata[offsetp0[1]], texdata[offsetp0[0]]);
}

__m128i blendLow(__m128i p0, __m128i ic0)
{
	__m128i ic0_low = _mm_shuffle_epi32(ic0, _MM_SHUFFLE(1,1,0,0));
	ic0_low = _mm_shufflehi_epi16(ic0_low, _MM_SHUFFLE(0,0,0,0));
	ic0_low = _mm_shufflelo_epi16(ic0_low, _MM_SHUFFLE(0,0,0,0));
	return _mm_mullo_epi16(_mm_unpacklo_epi8(p0, _mm_setzero_si128()), ic0_low);
}

__m128i blendHigh(__m128i p0, __m128i ic0)
{
	__m128i ic0_high = _mm_shuffle_epi32(ic0, _MM_SHUFFLE(3,3,2,2));
	ic0_high = _mm_shufflehi_epi16(ic0_high, _MM_SHUFFLE(0,0,0,0));
	ic0_high = _mm_shufflelo_epi16(ic0_high, _MM_SHUFFLE(0,0,0,0));
	return _mm_mullo_epi16(_mm_unpackhi_epi8(p0, _mm_setzero_si128()), ic0_high);
}

__m128i sampleLinear(Scanline &scanline, __m128 tx, __m128 ty)
{
	__m128 texwidth = _mm_cvtepi32_ps(_mm_set1_epi32(scanline.src_width));
	__m128 texheight = _mm_cvtepi32_ps(_mm_set1_epi32(scanline.src_height));
	__m128i itexwidth = _mm_set1_epi32(scanline.src_width);
	__m128i itexheight = _mm_set1_epi32(scanline.src_height);

	__m128 x = _mm_mul_ps(tx, texwidth);
	__m128 y = _mm_mul_ps(ty, texheight);
	__m128i ix0 = _mm_cvttps_epi32(x);
	__m128i iy0 = _mm_cvttps_epi32(y);
	__m128i ix1 = _mm_add_epi32(ix0, _mm_set1_epi32(1));
	__m128i iy1 = _mm_add_epi32(iy0, _mm_set1_epi32(1));

	__m128 a = _mm_sub_ps(x, _mm_cvtepi32_ps(ix0));
	__m128 b = _mm_sub_ps(y, _mm_cvtepi32_ps(iy0));
	__m128 inv_a = _mm_sub_ps(_mm_set1_ps(1.0f), a);
	__m128 inv_b = _mm_sub_ps(_mm_set1_ps(1.0f), b);

	ix0 = textureClampToEdge(ix0, itexwidth);
	ix1 = textureClampToEdge(ix1, itexwidth);
	iy0 = textureClampToEdge(iy0, itexheight);
	iy1 = textureClampToEdge(iy1, itexheight);

	__m128i ilineoffset0 = _mm_or_si128(_mm_slli_epi32(_mm_mulhi_epu16(iy0, itexwidth), 16), _mm_mullo_epi16(iy0, itexwidth));
	__m128i ilineoffset1 = _mm_or_si128(_mm_slli_epi32(_mm_mulhi_epu16(iy1, itexwidth), 16), _mm_mullo_epi16(iy1, itexwidth));
	__m128i ioffsetp0 = _mm_add_epi32(ix0, ilineoffset0);
	__m128i ioffsetp1 = _mm_add_epi32(ix1, ilineoffset0);
	__m128i ioffsetp2 = _mm_add_epi32(ix0, ilineoffset1);
	__m128i ioffsetp3 = _mm_add_epi32(ix1, ilineoffset1);

	__declspec(align(16)) unsigned int offsetp0[4], offsetp1[4], offsetp2[4], offsetp3[4];
	_mm_store_si128((__m128i*)offsetp0, ioffsetp0);
	_mm_store_si128((__m128i*)offsetp1, ioffsetp1);
	_mm_store_si128((__m128i*)offsetp2, ioffsetp2);
	_mm_store_si128((__m128i*)offsetp3, ioffsetp3);

	__m128 c0 = _mm_mul_ps(inv_a, inv_b);
	__m128 c1 = _mm_mul_ps(a, inv_b);
	__m128 c2 = _mm_mul_ps(inv_a, b);
	__m128 c3 = _mm_mul_ps(a, b);

	unsigned int *texdata = scanline.src;
	__m128i p0 = _mm_set_epi32(texdata[offsetp0[3]], texdata[offsetp0[2]], texdata[offsetp0[1]], texdata[offsetp0[0]]);
	__m128i p1 = _mm_set_epi32(texdata[offsetp1[3]], texdata[offsetp1[2]], texdata[offsetp1[1]], texdata[offsetp1[0]]);
	__m128i p2 = _mm_set_epi32(texdata[offsetp2[3]], texdata[offsetp2[2]], texdata[offsetp2[1]], texdata[offsetp2[0]]);
	__m128i p3 = _mm_set_epi32(texdata[offsetp3[3]], texdata[offsetp3[2]], texdata[offsetp3[1]], texdata[offsetp3[0]]);

	__m128 const256 = _mm_set1_ps(256.0f);
	__m128 const05 = _mm_set1_ps(0.5f);
	__m128i ic0 = _mm_cvttps_epi32(_mm_add_ps(_mm_mul_ps(c0, const256), const05));
	__m128i ic1 = _mm_cvttps_epi32(_mm_add_ps(_mm_mul_ps(c1, const256), const05));
	__m128i ic2 = _mm_cvttps_epi32(_mm_add_ps(_mm_mul_ps(c2, const256), const05));
	__m128i ic3 = _mm_cvttps_epi32(_mm_add_ps(_mm_mul_ps(c3, const256), const05));

	__m128i low = _mm_adds_epu16(_mm_adds_epu16(_mm_adds_epu16(blendLow(p0, ic0), blendLow(p1, ic1)), blendLow(p2, ic2)), blendLow(p3, ic3));
	__m128i high = _mm_adds_epu16(_mm_adds_epu16(_mm_adds_epu16(blendHigh(p0, ic0), blendHigh(p1, ic1)), blendHigh(p2, ic2)), blendHigh(p3, ic3));
	low = _mm_srli_epi16(_mm_adds_epu16(low, _mm_set1_epi16(128)), 8);
	high = _mm_srli_epi16(_mm_adds_epu16(high, _mm_set1_epi16(128)), 8);
	return _mm_packus_epi16(low, high);
}

__m128i blendFunc(__m128i src, __m128i dest)
{
	__m128i src_alpha = _mm_srli_epi32(src, 24);
	__m128i src_red = _mm_and_si128(_mm_srli_epi32(src, 16), _mm_set1_epi32(0xff));
	__m128i src_green = _mm_and_si128(_mm_srli_epi32(src, 8), _mm_set1_epi32(0xff));
	__m128i src_blue = _mm_and_si128(src, _mm_set1_epi32(0xff));

	__m128i dest_alpha = _mm_srli_epi32(src, 24);
	__m128i dest_red = _mm_and_si128(_mm_srli_epi32(src, 16), _mm_set1_epi32(0xff));
	__m128i dest_green = _mm_and_si128(_mm_srli_epi32(src, 8), _mm_set1_epi32(0xff));
	__m128i dest_blue = _mm_and_si128(src, _mm_set1_epi32(0xff));

	__m128i alpha = _mm_or_si128(src_alpha, _mm_srli_epi32(src_alpha, 7)); // convert alpha to be in range 0-256
	__m128i inv_alpha = _mm_sub_epi32(_mm_set1_epi32(256), alpha);

	dest_alpha = _mm_srli_epi32(_mm_add_epi32(_mm_mullo_epi16(dest_alpha, inv_alpha), _mm_mullo_epi16(src_alpha, alpha)), 8);
	dest_red = _mm_srli_epi32(_mm_add_epi32(_mm_mullo_epi16(dest_red, inv_alpha), _mm_mullo_epi16(src_red, alpha)), 8);
	dest_green = _mm_srli_epi32(_mm_add_epi32(_mm_mullo_epi16(dest_green, inv_alpha), _mm_mullo_epi16(src_green, alpha)), 8);
	dest_blue = _mm_srli_epi32(_mm_add_epi32(_mm_mullo_epi16(dest_blue, inv_alpha), _mm_mullo_epi16(src_blue, alpha)), 8);

	return _mm_or_si128(_mm_or_si128(_mm_or_si128(_mm_slli_epi32(dest_alpha, 24), _mm_slli_epi32(dest_red, 16)), _mm_slli_epi32(dest_green, 8)), dest_blue);
}
/*
__m128i blendFunc(__m128i src, __m128i dest)
{
	__m128i zero = _mm_setzero_si128();
	__m128i one = _mm_set1_epi16(0x0100);
	__m128i half7f = _mm_set1_epi16(0x007f);

	__m128i src0, dest0;
	__m128i src_alpha, invsrc_alpha;
	src0 = _mm_unpacklo_epi8(src, zero);
	dest0 = _mm_unpacklo_epi8(dest, zero);
	src_alpha = _mm_shufflelo_epi16(src0, _MM_SHUFFLE(3,3,3,3));
	src_alpha = _mm_shufflehi_epi16(src_alpha, _MM_SHUFFLE(3,3,3,3));
	invsrc_alpha = _mm_sub_epi16(one, src_alpha);

	src0 = _mm_mullo_epi16(src0, src_alpha);
	dest0 = _mm_mullo_epi16(dest0, invsrc_alpha);
	dest0 = _mm_add_epi16(dest0, src0);
	dest0 = _mm_add_epi16(dest0, half7f);
	dest0 = _mm_srli_epi16(dest0, 8);

	__m128i src1, dest1;
	src1 = _mm_unpackhi_epi8(src, zero);
	dest1 = _mm_unpackhi_epi8(dest, zero);
	src_alpha = _mm_shufflelo_epi16(src1, _MM_SHUFFLE(3,3,3,3));
	src_alpha = _mm_shufflehi_epi16(src_alpha, _MM_SHUFFLE(3,3,3,3));
	invsrc_alpha = _mm_sub_epi16(one, src_alpha);

	src1 = _mm_mullo_epi16(src1, src_alpha);
	dest1 = _mm_mullo_epi16(dest1, invsrc_alpha);
	dest1 = _mm_add_epi16(dest1, src1);
	dest1 = _mm_add_epi16(dest1, half7f);
	dest1 = _mm_srli_epi16(dest1, 8);

	return _mm_packus_epi16(dest0, dest1);
}
*/
void blendFragment(unsigned int *dest, __m128i src)
{
	_mm_storeu_si128((__m128i*)dest, blendFunc(src, _mm_loadu_si128((__m128i*)dest)));
}

FragmentQuadOutput fragmentProgram(Scanline &scanline, const FragmentQuadInput &input)
{
//	return FragmentQuadOutput(sampleNearest(scanline, input.tx, input.ty));
	return FragmentQuadOutput(sampleLinear(scanline, input.tx, input.ty));
}

void perspective2(Scanline *d)
{
	__m128 rcp_half_viewportwidth = _mm_rcp_ps(_mm_cvtepi32_ps(_mm_set1_epi32(d->half_viewport_width)));

	int tmp = d->startx - d->viewport_center;
	__m128 pos = _mm_add_ps(_mm_cvtepi32_ps(_mm_set_epi32(tmp+3, tmp+2, tmp+1, tmp)), _mm_set1_ps(0.5f));
	__m128 blocksize = _mm_set1_ps(4.0f);

	for (int x = d->startx; x < d->endx; x+=4, pos = _mm_add_ps(pos, blocksize))
	{
		__m128 xnormalized = _mm_mul_ps(pos, rcp_half_viewportwidth);

		FragmentQuadInput input = createFragmentQuad(*d, xnormalized);
		FragmentQuadOutput output = fragmentProgram(*d, input);
		blendFragment(d->dest+x, output.color0);
	}
}
