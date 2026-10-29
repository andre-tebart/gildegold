/* 0x420450, converts the 24-bit (three bytes per pixel) src surface to the 16-bit dst surface: each
   8-bit channel of the source pixel is scaled down to the dst format's channel width and placed at its
   bit position; writes dst only, returns nothing */

void surf_24To16(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src)
{
    int rs, gs, bs;
    int rb, gb, bb;
    unsigned int m, y, x;
    unsigned char *p;
    unsigned short *d;

    rs = 0;
    for (m = dst->ddpfPixelFormat.dwRBitMask; !(m & 1); m >>= 1)
        rs++;
    rb = 0;
    while (m) {
        m >>= 1;
        rb++;
    }
    gs = 0;
    for (m = dst->ddpfPixelFormat.dwGBitMask; !(m & 1); m >>= 1)
        gs++;
    gb = 0;
    while (m) {
        m >>= 1;
        gb++;
    }
    bs = 0;
    for (m = dst->ddpfPixelFormat.dwBBitMask; !(m & 1); m >>= 1)
        bs++;
    bb = 0;
    while (m) {
        m >>= 1;
        bb++;
    }
    for (y = 0; y < src->dwHeight; y++) {
        p = (unsigned char *)src->lpSurface + src->lPitch * y;
        d = (unsigned short *)((char *)dst->lpSurface + dst->lPitch * y);
        for (x = 0; x < src->dwWidth; x++) {
            d[x]
                = (unsigned short)(p[x * 3] >> (BYTE)(8 - rb)) << rs | (unsigned short)(p[x * 3 + 1] >> (BYTE)(8 - gb)) << gs
                | (unsigned short)(p[x * 3 + 2] >> (BYTE)(8 - bb)) << bs;
        }
    }
}
