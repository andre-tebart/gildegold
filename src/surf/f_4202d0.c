/* 0x4202d0, converts the 8-bit palette-indexed src surface to the 16-bit dst surface: each palette
   entry of pal is scaled down from 8 bits to the dst format's channel widths and placed at their bit
   positions; writes dst only, returns nothing */

void surf_Pal8To16(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src, PALETTEENTRY *pal)
{
    int rs, gs, bs;
    int rb, gb, bb;
    unsigned int m, y, x;
    PALETTEENTRY *c;
    unsigned char *s;
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
        s = (unsigned char *)src->lpSurface + src->lPitch * y;
        d = (unsigned short *)((char *)dst->lpSurface + dst->lPitch * y);
        for (x = 0; x < src->dwWidth; x++) {
            c = &pal[s[x]];
            d[x]
                = (unsigned short)(c->peRed >> (BYTE)(8 - rb)) << rs | (unsigned short)(c->peGreen >> (BYTE)(8 - gb)) << gs
                | (unsigned short)(c->peBlue >> (BYTE)(8 - bb)) << bs;
        }
    }
}
