/* 0x478bd0, rgb bytes (w x h) to the 16 bit pixels of surface s @deep B3 8 code hunks / 9 differing instructions, all one fold: the original keeps the row pointer at src+1 (lea esi,[edx+0x1] at 0x478bed, store [esp+0x28]) and reads the first pixel as p[-1],p[0],p[1] (0x478bff mov al,[esi+1]; 0x478c02 mov cl,[esi]; 0x478c04 mov dl,[esi-1]); ours rebases p to src (mov esi,edx; loads [esi+2],[esi+1],[esi]) - same addresses, same dd_RGB call, 35 equal instructions. Meaning read against 0x478bd0..0x478c4f: stride=w*3, rp=src+1, p=rp, y< h / x<w unsigned loops, store s->bits[s->pitch*y+x], rp += stride - all correspond. diffclass verdict: 3 hunks, code 9, 'compare the meaning with the original disassembly first' - done, no meaning error. spell.py: no hunk matched the menu. Tried (one edit/build): rp = src; rp++; -> identical fold, no change, reverted. notes/units/gfx.md (d2q) records the same fold as unsolved and says everything else is equal. The +1 induction-variable rebase is VC6's optimizer with no source lever -> deep. Readability done: Q1 fixed (converts rgb bytes to 16 bit pixels, sibling 0x478c50 is the reverse). */

void d2_ConvertRgbTo16(int a, int b, unsigned int w, unsigned int h, unsigned char *src, int c, int d, D2SURF *s)
{
    unsigned int x, y, stride;
    unsigned char *p, *rp;

    y = 0;
    if (h > 0) {
        stride = w * 3;
        rp = src + 1;
        do {
            x = 0;
            p = rp;
            if (w > 0) {
                do {
                    s->bits[s->pitch * y + x] = (unsigned short)dd_RGB(p[-1], p[0], p[1]);
                    x++;
                    p += 3;
                } while (x < w);
            }
            y++;
            rp += stride;
        } while (y < h);
    }
}
