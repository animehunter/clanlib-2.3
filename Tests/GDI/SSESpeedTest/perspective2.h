
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

FragmentQuadInput createFragment(Scanline &scanline, __m128 xnormalized)
{
	__m128 x2 = _mm_set1_ps(scanline.x2);
	__m128 w2 = _mm_set1_ps(scanline.w2);
	__m128 dx = _mm_sub_ps(_mm_set1_ps(scanline.x2), _mm_set1_ps(scanline.x1));
	__m128 dw = _mm_sub_ps(_mm_set1_ps(scanline.w2), _mm_set1_ps(scanline.w1));
	__m128 tx1 = _mm_set1_ps(scanline.tx1);
	__m128 tx2 = _mm_set1_ps(scanline.tx2);
	__m128 ty1 = _mm_set1_ps(scanline.ty1);
	__m128 ty2 = _mm_set1_ps(scanline.ty2);

	__m128 a = _mm_mul_ps(_mm_sub_ps(x2, _mm_mul_ps(xnormalized, w2)), _mm_rcp_ps(_mm_sub_ps(dx, _mm_mul_ps(xnormalized, dw))));
	__m128 inv_a = _mm_sub_ps(_mm_set1_ps(1.0f), a);

	__m128 tx = _mm_add_ps(_mm_mul_ps(a, tx1), _mm_mul_ps(inv_a, tx2));
	__m128 ty = _mm_add_ps(_mm_mul_ps(a, ty1), _mm_mul_ps(inv_a, ty2));

	return FragmentQuadInput(tx, ty);
}

__m128i blendPixel(__m128i p, __m128i blend)
{
	__m128i alpha = _mm_srli_epi32(p, 24);
	__m128i red = _mm_and_si128(_mm_srli_epi32(p, 16), _mm_set1_epi32(0xff));
	__m128i green = _mm_and_si128(_mm_srli_epi32(p, 8), _mm_set1_epi32(0xff));
	__m128i blue = _mm_and_si128(p, _mm_set1_epi32(0xff));

	alpha = _mm_srli_epi32(_mm_mullo_epi16(alpha, blend), 8);
	red = _mm_srli_epi32(_mm_mullo_epi16(red, blend), 8);
	green = _mm_srli_epi32(_mm_mullo_epi16(green, blend), 8);
	blue = _mm_srli_epi32(_mm_mullo_epi16(blue, blend), 8);

	return _mm_or_si128(_mm_or_si128(_mm_or_si128(_mm_slli_epi32(alpha, 24), _mm_slli_epi32(red, 16)), _mm_slli_epi32(green, 8)), blue);
}

__m128i sampleLinear(Scanline &scanline, const FragmentQuadInput &input)
{
	__m128 texwidth = _mm_cvtepi32_ps(_mm_set1_epi32(scanline.src_width));
	__m128 texheight = _mm_cvtepi32_ps(_mm_set1_epi32(scanline.src_height));
	__m128i itexwidth = _mm_set1_epi32(scanline.src_width);
	__m128i itexheight = _mm_set1_epi32(scanline.src_height);
	__m128i itexwidthmax = _mm_sub_epi32(itexwidth, _mm_set1_epi32(1));
	__m128i itexheightmax = _mm_sub_epi32(itexheight, _mm_set1_epi32(1));

	__m128 x = _mm_mul_ps(input.tx, texwidth);
	__m128 y = _mm_mul_ps(input.ty, texheight);
	__m128i ix0 = _mm_cvttps_epi32(x);
	__m128i iy0 = _mm_cvttps_epi32(y);
	__m128i ix1 = _mm_add_epi32(ix0, _mm_set1_epi32(1));
	__m128i iy1 = _mm_add_epi32(iy0, _mm_set1_epi32(1));

	__m128 a = _mm_sub_ps(x, _mm_cvtepi32_ps(ix0));
	__m128 b = _mm_sub_ps(y, _mm_cvtepi32_ps(iy0));
	__m128 inv_a = _mm_sub_ps(_mm_set1_ps(1.0f), a);
	__m128 inv_b = _mm_sub_ps(_mm_set1_ps(1.0f), b);

	__m128 c0 = _mm_mul_ps(inv_a, inv_b);
	__m128 c1 = _mm_mul_ps(a, inv_b);
	__m128 c2 = _mm_mul_ps(inv_a, b);
	__m128 c3 = _mm_mul_ps(a, b);

	ix0 = _mm_min_epi16(ix0, itexwidthmax);  // ix0 < itexwidthmax ? ix0 : itexwidthmax;
	ix1 = _mm_min_epi16(ix1, itexwidthmax);  // ix1 < itexwidthmax ? ix1 : itexwidthmax;
	iy0 = _mm_min_epi16(iy0, itexheightmax); // iy0 < itexheightmax ? iy0 : itexheightmax;
	iy1 = _mm_min_epi16(iy1, itexheightmax); // iy1 < itexheightmax ? iy1 : itexheightmax;

	ix0 = _mm_max_epi16(ix0, _mm_setzero_si128()); // ix0 > 0 ? ix0 : 0;
	ix1 = _mm_max_epi16(ix1, _mm_setzero_si128()); // ix1 > 0 ? ix1 : 0;
	iy0 = _mm_max_epi16(iy0, _mm_setzero_si128()); // iy0 > 0 ? iy0 : 0;
	iy1 = _mm_max_epi16(iy1, _mm_setzero_si128()); // iy1 > 0 ? iy1 : 0;

	iy0 = _mm_or_si128(_mm_slli_epi32(_mm_mulhi_epu16(iy0, itexwidth), 16), _mm_mullo_epi16(iy0, itexwidth));
	iy1 = _mm_or_si128(_mm_slli_epi32(_mm_mulhi_epu16(iy1, itexwidth), 16), _mm_mullo_epi16(iy1, itexwidth));

	__m128i ioffsetp0 = _mm_add_epi32(ix0, iy0);
	__m128i ioffsetp1 = _mm_add_epi32(ix1, iy0);
	__m128i ioffsetp2 = _mm_add_epi32(ix0, iy1);
	__m128i ioffsetp3 = _mm_add_epi32(ix1, iy1);

	__declspec(align(16)) unsigned int offsetp0[4], offsetp1[4], offsetp2[4], offsetp3[4];
	_mm_store_si128((__m128i*)offsetp0, ioffsetp0);
	_mm_store_si128((__m128i*)offsetp1, ioffsetp1);
	_mm_store_si128((__m128i*)offsetp2, ioffsetp2);
	_mm_store_si128((__m128i*)offsetp3, ioffsetp3);

	unsigned int *texdata = scanline.src;
	__m128i p0 = _mm_set_epi32(texdata[offsetp0[3]], texdata[offsetp0[2]], texdata[offsetp0[1]], texdata[offsetp0[0]]);
	__m128i p1 = _mm_set_epi32(texdata[offsetp1[3]], texdata[offsetp1[2]], texdata[offsetp1[1]], texdata[offsetp1[0]]);
	__m128i p2 = _mm_set_epi32(texdata[offsetp2[3]], texdata[offsetp2[2]], texdata[offsetp2[1]], texdata[offsetp2[0]]);
	__m128i p3 = _mm_set_epi32(texdata[offsetp3[3]], texdata[offsetp3[2]], texdata[offsetp3[1]], texdata[offsetp3[0]]);

	__m128i ic0 = _mm_cvttps_epi32(_mm_add_ps(_mm_mul_ps(c0, _mm_set1_ps(256.0f)), _mm_set1_ps(0.5f)));
	__m128i ic1 = _mm_cvttps_epi32(_mm_add_ps(_mm_mul_ps(c1, _mm_set1_ps(256.0f)), _mm_set1_ps(0.5f)));
	__m128i ic2 = _mm_cvttps_epi32(_mm_add_ps(_mm_mul_ps(c2, _mm_set1_ps(256.0f)), _mm_set1_ps(0.5f)));
	__m128i ic3 = _mm_cvttps_epi32(_mm_add_ps(_mm_mul_ps(c3, _mm_set1_ps(256.0f)), _mm_set1_ps(0.5f)));

	return _mm_add_epi32(_mm_add_epi32(_mm_add_epi32(blendPixel(p0, ic0), blendPixel(p1, ic1)), blendPixel(p2, ic2)), blendPixel(p3, ic3));
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

FragmentQuadOutput fragmentProgram(Scanline &scanline, const FragmentQuadInput &input)
{
	return FragmentQuadOutput(sampleLinear(scanline, input));
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

		FragmentQuadInput input = createFragment(*d, xnormalized);
		FragmentQuadOutput output = fragmentProgram(*d, input);
		_mm_storeu_si128((__m128i*)(d->dest+x), blendFunc(output.color0, _mm_loadu_si128((__m128i*)(d->dest+x))));
	}
}
