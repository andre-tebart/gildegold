/* 0x41f000 @deep behaviour: no meaning difference against the Ghidra reading (the same trailing-zero shift count per channel, y0/y1 and x0/x1 box bounds, the same sum of masked channels divided by n and OR of b/n << bs | g/n << gs | r/n << rs); the 302 code hunks are scheduling and register/slot allocation (the src parameter in ebp against esi, the slot order, hoisted row bases); the original's shift counters are bytes (unsigned char rs, gs, bs), but that made the diff worse (426 hunks), so it belongs with the shape work; readability met */

void surf_Down32(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src)
{
    int rs, gs, bs;
    unsigned int m;
    unsigned int y, x, y0, y1, x0, x1, n, r, g, b, yy, xx, p;
    unsigned int *sp;
    char *row;
    unsigned int *d;

    rs = 0;
    for (m = src->ddpfPixelFormat.dwRBitMask; !(m & 1); m >>= 1)
        rs++;
    for (; m; m >>= 1)
        ;
    gs = 0;
    for (m = src->ddpfPixelFormat.dwGBitMask; !(m & 1); m >>= 1)
        gs++;
    for (; m; m >>= 1)
        ;
    bs = 0;
    for (m = src->ddpfPixelFormat.dwBBitMask; !(m & 1); m >>= 1)
        bs++;
    for (; m; m >>= 1)
        ;
    y0 = 0;
    for (y = 0; y < dst->dwHeight; y++) {
        d = (unsigned int *)((char *)dst->lpSurface + dst->lPitch * y);
        y1 = src->dwHeight * (y + 1) / dst->dwHeight;
        x0 = 0;
        for (x = 0; x < dst->dwWidth; x++) {
            x1 = src->dwWidth * (x + 1) / dst->dwWidth;
            n = 0;
            r = 0;
            g = 0;
            b = 0;
            if (y1 - y0) {
                row = (char *)src->lpSurface + src->lPitch * y0;
                yy = y1 - y0;
                do {
                    if (x1 - x0) {
                        n += x1 - x0;
                        sp = (unsigned int *)row + x0;
                        xx = x1 - x0;
                        do {
                            p = *sp++;
                        r += (p & src->ddpfPixelFormat.dwRBitMask) >> rs;
                        g += (p & src->ddpfPixelFormat.dwGBitMask) >> gs;
                        b += (p & src->ddpfPixelFormat.dwBBitMask) >> bs;
                        } while (--xx);
                    }
                    row += src->lPitch;
                } while (--yy);
            }
            d[x] = b / n << bs | g / n << gs | r / n << rs;
            x0 = x1;
        }
        y0 = y1;
    }
}
