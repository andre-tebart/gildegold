/* 0x41ec50, 16-bit box filter shrinking src into dst */

void surf_Shrink16(LPDDSURFACEDESC2 dst, LPDDSURFACEDESC2 src)
{
    DWORD m, x, y, x0, x1, y0, y1, i, j, n, r, g, b, c, w;
    int rs, gs, bs, rb, gb, bb;
    unsigned short *d, *s;

    for (rs = 0, m = src->ddpfPixelFormat.dwRBitMask; !(m & 1); m >>= 1)
        rs++;
    for (rb = 0; m; m >>= 1)
        rb++;
    for (gs = 0, m = src->ddpfPixelFormat.dwGBitMask; !(m & 1); m >>= 1)
        gs++;
    for (gb = 0; m; m >>= 1)
        gb++;
    for (bs = 0, m = src->ddpfPixelFormat.dwBBitMask; !(m & 1); m >>= 1)
        bs++;
    for (bb = 0; m; m >>= 1)
        bb++;
    y1 = 0;
    for (y = 0; y < dst->dwHeight; y++) {
        d = (unsigned short *)((unsigned char *)dst->lpSurface + dst->lPitch * y);
        y0 = y1;
        y1 = src->dwHeight * (y + 1) / dst->dwHeight;
        x1 = 0;
        for (x = 0; x < dst->dwWidth; x++) {
            x0 = x1;
            x1 = src->dwWidth * (x + 1) / dst->dwWidth;
            w = x1 - x0;
            n = b = g = r = 0;
            for (j = 0; j < y1 - y0; j++) {
                s = (unsigned short *)((unsigned char *)src->lpSurface + src->lPitch * (y0 + j));
                for (i = 0; i < w; i++) {
                    r += (s[x0 + i] & src->ddpfPixelFormat.dwRBitMask) >> rs;
                    g += (s[x0 + i] & src->ddpfPixelFormat.dwGBitMask) >> gs;
                    b += (s[x0 + i] & src->ddpfPixelFormat.dwBBitMask) >> bs;
                    n++;
                }
            }
            d[x] = (unsigned short)((r / n << rs) | (g / n << gs) | (b / n << bs));
        }
    }
}
