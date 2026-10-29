/* 0x47bec0, converts an 8 bit shape (palette at 0xf8a120) into a 16 bit one @deep B3, 301 code hunks: the whole function's register allocation differs - the shape pointer s is in ebp where the original has ebx, the result r in ebx where the original has ebp (prologue 0x47beca / 0x47beef), and every read hunk (alloc, memcpy, run counts, pixel loop) is that swap plus scheduling; no meaning error found: the offsets 0x26 / 0x2e / 0x32 / 0x36, the dd_RGB arguments, the mem tag and the loop bounds correspond */

D2SHP *d2_ShpConvert8To16(D2SHP *s)
{
    unsigned short pal[256];
    unsigned short buf[0x480];
    int lines[864];
    int i, n;
    unsigned int j, k, cnt;
    unsigned int *src, *srccnt, *dst, *dstcnt;
    unsigned char *pix;
    D2SHP *r;

    if (s->nruns == -1) {
        err_Message("shp_Convert8To16: Converting of NoReadAndSkip Shapes not surported...");
        return 0;
    }
    r = mem_Alloc(s->size + s->npix, "d2:shp:NewShape");
    memcpy(r, s, 0x32);
    r->nruns = 0;
    r->npix = 0;
    r->size = 0x32;
    r->type = 1;
    for (i = 0; i < 256; i++)
        pal[i] = dd_RGB(d2_Palette[i][0], d2_Palette[i][1], d2_Palette[i][2]);
    src = (unsigned int *)((char *)s + 0x36);
    dst = (unsigned int *)((char *)r + 0x32);
    dstcnt = (unsigned int *)((char *)r + 0x36);
    srccnt = (unsigned int *)((char *)s + 0x32);
    for (i = 0; i < s->h; i++) {
        *dst = *srccnt;
        lines[i] = (char *)dst - (char *)r;
        r->size += 4;
        if (*srccnt != 0) {
            for (j = 0; j < *srccnt; j++) {
                *dstcnt = *src * 2;
                dstcnt[1] = src[1];
                n = src[1];
                if (n != 0) {
                    for (k = 0; k < (unsigned int)n; k++) {
                        buf[k] = pal[((unsigned char *)src)[k + 8]];
                        if (s->mode == 1)
                            buf[k] = buf[k] >> 1 & d2_HighBitMask;
                    }
                }
                memcpy(dstcnt + 2, buf, n * 2);
                r->nruns++;
                r->npix += src[1];
                r->size += src[1] * 2 + 8;
                src = (unsigned int *)((char *)src + src[1] + 8);
                dstcnt = (unsigned int *)((char *)dstcnt + dstcnt[1] * 2 + 8);
            }
        }
        dst = dstcnt;
        dstcnt = dstcnt + 1;
        srccnt = src;
        src = src + 1;
    }
    r->lineoff = r->size;
    r->size += r->h * 4;
    d2_SetLineTable(lines, r);
    return r;
}
