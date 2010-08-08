
void clipScanline(Scanline *d, int clipx1, int clipx2)
{
	int x1 = (int)(d->x1+0.5f);
	int x2 = ((int)(d->x2-0.5f))+1;
	x1 = cl_max(x1, clipx1);
	x2 = cl_min(x2, clipx2);
	d->startx = x1;
	d->endx = x2;
}
