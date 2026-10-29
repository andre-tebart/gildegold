/* 0x4798f0, reads the colour of the pixel (x, y) of surf into rgb[0..2] (r, g, b): a 15 / 16 bpp pixel through
   dd_SplitRGB, the 3 bytes for 24 bpp, one dword for 32 bpp (r / g / b from the dword's bytes 0 / 4 / 8, the
   original's odd offsets); any other depth leaves rgb untouched by the switch (it gets the caller's stack bytes)
   @deep B3 far draft: 99 code hunks, 77 of 89 instructions / 51 of 240 bytes differ, all one register permutation plus bits-load scheduling. Read against 0x4798f0..0x4799df (jump table 0x4799b0, byte index table 0x4799c0, bpp 15/16 -> dd_SplitRGB, 24 -> 3 bytes, 32 -> dword bytes 0/4/8, default skips to the rgb store): same algorithm, same call, same stores, no meaning error. The original keeps surf in eax, bpp in edx, bytes (bpp>>3) in ecx; ours surf in ecx, bpp in eax, bytes in edx, and loads surf->bits before the imul where the original loads it after; with --regs ecx=eax,edx=ecx,eax=edx the remaining diffs are that bits-load / x-in-register scheduling in the 15/16 and 0x18 arms. spell.py: reload at 0x4798f0 (drop the local / re-read at the use) tried as (a) a local i = surf->pitch*y+x in every arm, as the original's edx/esi pattern suggested, and (b) all index expressions inlined with the i local dropped - both identical (51/240, 77 lines), reverted; dup-tail 0x47995f is the shared rgb[0..2] store epilogue, already one statement list after the switch, nothing to pull or duplicate. No source lever left for the parameter / bytes register roles -> register allocation, deep's toolbox. */

void d2_GetPixelColour(int x, int y, D2SURF *surf, unsigned char *rgb)
{
    unsigned char r, g, b;
    unsigned char *p;
    int i;
    unsigned int bytes = surf->bpp >> 3;

    switch (surf->bpp) {
    case 0xf:
    case 0x10:
        dd_SplitRGB(((unsigned short *)surf->bits)[surf->pitch * y + x], &r, &g, &b);
        break;
    case 0x18:
        p = (unsigned char *)surf->bits + (surf->pitch * y + x) * bytes;
        r = p[0];
        g = p[1];
        b = p[2];
        break;
    case 0x20:
        i = surf->pitch * y + x;
        r = ((unsigned char *)surf->bits)[i * 4];
        g = ((unsigned char *)surf->bits)[i * 4 + 4];
        b = ((unsigned char *)surf->bits)[i * 4 + 8];
        break;
    }
    rgb[0] = r;
    rgb[1] = g;
    rgb[2] = b;
}
