
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

FragmentQuadInput createFragmentQuad(Scanline &scanline, int x)
{
	__m128 viewportwidth, viewportcenter, one, dx, dw, x2, w2, tx1, tx2, ty1, ty2;
	viewportwidth = _mm_rcp_ss(_mm_cvtsi32_ss(_mm_setzero_ps(), scanline.viewport_width));
	viewportwidth = _mm_shuffle_ps(viewportwidth, viewportwidth, _MM_SHUFFLE(0,0,0,0));
	viewportcenter = _mm_cvtsi32_ss(_mm_setzero_ps(), scanline.viewport_center);
	viewportcenter = _mm_shuffle_ps(viewportcenter, viewportcenter, _MM_SHUFFLE(0,0,0,0));
	one = _mm_set1_ps(1.0f);
	x2 = _mm_set1_ps(scanline.x2);
	w2 = _mm_set1_ps(scanline.w2);
	dx = _mm_set1_ps(scanline.x2-scanline.x1);
	dw = _mm_set1_ps(scanline.w2-scanline.w1);
	tx1 = _mm_set1_ps(scanline.tx1);
	tx2 = _mm_set1_ps(scanline.tx2);
	ty1 = _mm_set1_ps(scanline.ty1);
	ty2 = _mm_set1_ps(scanline.ty2);

	__m128 xw, a, inv_a, tx, ty;
	xw = _mm_mul_ps(_mm_sub_ps(_mm_set_ps(x+3.5f, x+2.5f, x+1.5f, x+0.5f), viewportcenter), viewportwidth);
	a = _mm_mul_ps(_mm_sub_ps(x2, _mm_mul_ps(xw, w2)), _mm_rcp_ss(_mm_sub_ps(dx, _mm_mul_ps(xw, dw))));
	inv_a = _mm_sub_ps(one, a);

	tx = _mm_add_ps(_mm_mul_ps(a, tx1), _mm_mul_ps(inv_a, tx2));
	ty = _mm_add_ps(_mm_mul_ps(a, ty1), _mm_mul_ps(inv_a, ty2));

	return FragmentQuadInput(tx, ty);
}

__m128i sampleLinearQuad(Scanline &scanline, const FragmentQuadInput &input)
{
	__m128 one, texwidth, texheight, value256, half;
	__m128i izero, ione, ihalf, itexwidth, itexheight, itexwidthmax, itexheightmax;
	one = _mm_set1_ps(1.0f);
	value256 = _mm_set1_ps(256.0f);
	half = _mm_set1_ps(0.5f);
	texwidth = _mm_set1_ps((float)scanline.src_width);
	texheight = _mm_set1_ps((float)scanline.src_height);
	izero = _mm_setzero_si128();
	ione = _mm_set1_epi32(1);
	ihalf = _mm_set1_epi16(0x0080);
	itexwidth = _mm_set1_epi32(scanline.src_width);
	itexheight = _mm_set1_epi32(scanline.src_height);
	itexwidthmax = _mm_sub_epi32(itexwidth, ione);
	itexheightmax = _mm_sub_epi32(itexheight, ione);

	__m128 x, y;
	__m128i ix0, ix1, iy0, iy1;

	x = _mm_mul_ps(input.tx, texwidth);
	y = _mm_mul_ps(input.ty, texheight);
	ix0 = _mm_cvttps_epi32(x);
	iy0 = _mm_cvttps_epi32(y);
	ix1 = _mm_add_epi32(ix0, ione);
	iy1 = _mm_add_epi32(iy0, ione);

	__m128 a, b, inv_a, inv_b, c0, c1, c2, c3;
	a = _mm_sub_ps(x, _mm_cvtepi32_ps(ix0));
	b = _mm_sub_ps(y, _mm_cvtepi32_ps(iy0));
	inv_a = _mm_sub_ps(one, a);
	inv_b = _mm_sub_ps(one, b);

	c0 = _mm_mul_ps(inv_a, inv_b);
	c1 = _mm_mul_ps(a, inv_b);
	c2 = _mm_mul_ps(inv_a, b);
	c3 = _mm_mul_ps(a, b);

	ix0 = _mm_min_epi16(ix0, itexwidthmax);
	ix1 = _mm_min_epi16(ix1, itexwidthmax);
	iy0 = _mm_min_epi16(iy0, itexheightmax);
	iy1 = _mm_min_epi16(iy1, itexheightmax);
	ix0 = _mm_max_epi16(ix0, izero);
	ix1 = _mm_max_epi16(ix1, izero);
	iy0 = _mm_max_epi16(iy0, izero);
	iy1 = _mm_max_epi16(iy1, izero);

	iy0 = _mm_or_si128(_mm_slli_epi32(_mm_mulhi_epi16(iy0, itexwidth), 16), _mm_mullo_epi16(iy0, itexwidth));
	iy1 = _mm_or_si128(_mm_slli_epi32(_mm_mulhi_epi16(iy1, itexwidth), 16), _mm_mullo_epi16(iy1, itexwidth));

	__declspec(align(16)) int ioffsetp0[4], ioffsetp1[4], ioffsetp2[4], ioffsetp3[4];
	_mm_store_si128((__m128i*)ioffsetp0, _mm_add_epi32(ix0, iy0));
	_mm_store_si128((__m128i*)ioffsetp1, _mm_add_epi32(ix1, iy0));
	_mm_store_si128((__m128i*)ioffsetp2, _mm_add_epi32(ix0, iy1));
	_mm_store_si128((__m128i*)ioffsetp3, _mm_add_epi32(ix1, iy1));

	unsigned int *texdata = scanline.src;
	__m128i p0 = _mm_set_epi32(texdata[ioffsetp0[3]], texdata[ioffsetp0[2]], texdata[ioffsetp0[1]], texdata[ioffsetp0[0]]);
	__m128i p1 = _mm_set_epi32(texdata[ioffsetp1[3]], texdata[ioffsetp1[2]], texdata[ioffsetp1[1]], texdata[ioffsetp1[0]]);
	__m128i p2 = _mm_set_epi32(texdata[ioffsetp2[3]], texdata[ioffsetp2[2]], texdata[ioffsetp2[1]], texdata[ioffsetp2[0]]);
	__m128i p3 = _mm_set_epi32(texdata[ioffsetp3[3]], texdata[ioffsetp3[2]], texdata[ioffsetp3[1]], texdata[ioffsetp3[0]]);

	__m128i ic0, ic1, ic2, ic3;
	ic0 = _mm_cvttps_epi32(_mm_add_ps(_mm_mul_ps(c0, value256), half));
	ic1 = _mm_cvttps_epi32(_mm_add_ps(_mm_mul_ps(c1, value256), half));
	ic2 = _mm_cvttps_epi32(_mm_add_ps(_mm_mul_ps(c2, value256), half));
	ic3 = _mm_cvttps_epi32(_mm_add_ps(_mm_mul_ps(c3, value256), half));
	ic0 = _mm_packs_epi32(ic0, ic0);
	ic1 = _mm_packs_epi32(ic1, ic1);
	ic2 = _mm_packs_epi32(ic2, ic2);
	ic3 = _mm_packs_epi32(ic3, ic3);

	__m128i result0, result1, tmp0, tmp1;
	tmp0 = _mm_unpacklo_epi8(p0, izero);
	tmp1 = _mm_shufflelo_epi16(ic0, _MM_SHUFFLE(0,0,0,0));
	tmp1 = _mm_shufflehi_epi16(tmp1, _MM_SHUFFLE(1,1,1,1));
	result0 = _mm_mullo_epi16(tmp0, tmp1);

	tmp0 = _mm_unpacklo_epi8(p1, izero);
	tmp1 = _mm_shufflelo_epi16(ic1, _MM_SHUFFLE(0,0,0,0));
	tmp1 = _mm_shufflehi_epi16(tmp1, _MM_SHUFFLE(1,1,1,1));
	result0 = _mm_add_epi16(result0, _mm_mullo_epi16(tmp0, tmp1));

	tmp0 = _mm_unpacklo_epi8(p2, izero);
	tmp1 = _mm_shufflelo_epi16(ic2, _MM_SHUFFLE(0,0,0,0));
	tmp1 = _mm_shufflehi_epi16(tmp1, _MM_SHUFFLE(1,1,1,1));
	result0 = _mm_add_epi16(result0, _mm_mullo_epi16(tmp0, tmp1));

	tmp0 = _mm_unpacklo_epi8(p3, izero);
	tmp1 = _mm_shufflelo_epi16(ic3, _MM_SHUFFLE(0,0,0,0));
	tmp1 = _mm_shufflehi_epi16(tmp1, _MM_SHUFFLE(1,1,1,1));
	result0 = _mm_add_epi16(result0, _mm_mullo_epi16(tmp0, tmp1));

	result0 = _mm_add_epi16(result0, ihalf);
	result0 = _mm_srli_epi16(result0, 8);

	tmp0 = _mm_unpackhi_epi8(p0, izero);
	tmp1 = _mm_shufflelo_epi16(ic0, _MM_SHUFFLE(2,2,2,2));
	tmp1 = _mm_shufflehi_epi16(tmp1, _MM_SHUFFLE(3,3,3,3));
	result1 = _mm_mullo_epi16(tmp0, tmp1);

	tmp0 = _mm_unpackhi_epi8(p1, izero);
	tmp1 = _mm_shufflelo_epi16(ic1, _MM_SHUFFLE(2,2,2,2));
	tmp1 = _mm_shufflehi_epi16(tmp1, _MM_SHUFFLE(3,3,3,3));
	result1 = _mm_add_epi16(result1, _mm_mullo_epi16(tmp0, tmp1));

	tmp0 = _mm_unpackhi_epi8(p2, izero);
	tmp1 = _mm_shufflelo_epi16(ic2, _MM_SHUFFLE(2,2,2,2));
	tmp1 = _mm_shufflehi_epi16(tmp1, _MM_SHUFFLE(3,3,3,3));
	result1 = _mm_add_epi16(result1, _mm_mullo_epi16(tmp0, tmp1));

	tmp0 = _mm_unpackhi_epi8(p3, izero);
	tmp1 = _mm_shufflelo_epi16(ic3, _MM_SHUFFLE(2,2,2,2));
	tmp1 = _mm_shufflehi_epi16(tmp1, _MM_SHUFFLE(3,3,3,3));
	result1 = _mm_add_epi16(result1, _mm_mullo_epi16(tmp0, tmp1));

	result1 = _mm_add_epi16(result1, ihalf);
	result1 = _mm_srli_epi16(result1, 8);

	return _mm_packs_epi16(result1, result0);
}

__m128i blendQuad(__m128i dest, __m128i src)
{
	__m128i zero = _mm_setzero_si128();
	__m128i half7f = _mm_set1_epi16(0x007f);
	__m128i one = _mm_set1_epi16(256);

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

FragmentQuadOutput fragmentProgram(Scanline &scanline, const FragmentQuadInput &input)
{
	return FragmentQuadOutput(sampleLinearQuad(scanline, input));
/*
	__m128 x = _mm_mul_ps(input.tx, _mm_set1_ps(255.0f));
	__m128 y = _mm_mul_ps(input.ty, _mm_set1_ps(255.0f));
	__m128i ix = _mm_cvtps_epi32(x);
	__m128i iy = _mm_cvtps_epi32(y);
	__m128i px = _mm_set1_epi32(0xffffffff);
	px = _mm_sub_epi32(px, ix);
	px = _mm_sub_epi32(px, _mm_slli_epi32(iy, 8));
	return FragmentQuadOutput(px);
*/
}

void perspective2(Scanline *d)
{
	for (int x = d->startx; x < d->endx; x+=4)
	{
		FragmentQuadInput input = createFragmentQuad(*d, x);
		input.tx = _mm_set_ps((x+3.0f)/d->viewport_width, (x+2.0f)/d->viewport_width, (x+1.0f)/d->viewport_width, (x+0.0f)/d->viewport_width);
		input.ty = _mm_set1_ps(d->ty1);
		FragmentQuadOutput output = fragmentProgram(*d, input);
		//__m128i dest = _mm_loadu_si128((__m128i*)(d->dest+x));
		__m128i src = output.color0; //blendQuad(dest, output.color0);
		_mm_storeu_si128((__m128i*)(d->dest+x), src);
	}
}
