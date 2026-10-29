/* 0x478c50, 16 bit pixels of src (w x h) to rgb bytes @deep B3 29 code + 12 slots hunks (60/112 bytes): the same +1 pointer fold as its sibling 0x478bd0 (escalated this session). The original keeps the row pointer at dst+1 (lea esi,[eax+0x1] at 0x478c65, slot [esp+0x1c] = the dead w parameter slot) and passes p-1,p,p+1 (lea edx,[esi-1]; push esi; lea ecx,[esi+1]); ours rebases p to dst (dec esi at 0x478c73), pushes [esi+2],[esi+1],esi and stores rp in the dead dst parameter slot [esp+0x30] - the 12 slot hunks are that different dead-parameter slot and follow from the fold. Meaning read against 0x478c50..0x478cbc: y<h and x<w unsigned loops, rp=dst+1, p=rp, dd_SplitRGB(src[dd_PitchPixels*y+x], p-1, p, p+1), p+=3, rp+=w*3, all correspond; the 8 instructions around the call are equal. Q2 fixed: dd_PitchPixels is the screen pitch in pixels, now dd_PitchPixels (dd_vesa.h) in gfx.c; its duplicate extern in d2.h removed and d2.c's two uses renamed. spell.py: no hunk matched the menu; the sibling's tried spelling (rp=src;rp++) changed nothing. The induction-variable +1 rebase is VC6's optimizer with no source lever -> deep. */

void d2_Convert16ToRgb(int a, int b, unsigned int w, unsigned int h, unsigned short *src, int c, int d, unsigned char *dst)
{
    unsigned int x, y;
    unsigned char *p, *rp;

    y = 0;
    if (h > 0) {
        rp = dst + 1;
        do {
            x = 0;
            p = rp;
            if (w > 0) {
                do {
                    dd_SplitRGB(src[dd_PitchPixels * y + x], p - 1, p, p + 1);
                    x++;
                    p += 3;
                } while (x < w);
            }
            rp += w * 3;
            y++;
        } while (y < h);
    }
}
