/* 0x47b900, draws a 16 bit run-length shape half dark: every pixel of the literal runs is replaced by its grey
   (r + b) * 0.2f + g * 0.59f through dd_RGB, the skip runs are stepped over (the x / y parameters are reused as
   the run / pixel counters).
   @deep behaviour draft (186 code hunks, 302/336 bytes, frame 0x18 vs 0x1c): the meaning was read against 47b900..47ba5f and corresponds - line = bits + (pitch*y + x), the row loop over s->h reusing the x and y parameters as the run / pixel counters, d += run[0] >> 1, the pixel loop with dd_SplitRGB(*px, &r,&g,&b), v = (int)((r + b) * 0.2f + g * 0.59f) (the original's fild/fiadd/fmul 0.2f, fild/fmul 0.59f, faddp, _ftol), dd_RGB(v,v,v), the run chain run + run[1]*2 + 8 and cp = run, line += pitch. What differs is codegen only: frame 0x18 vs 0x1c and other register/slot homes for cp/run/line/d; no source meaning fix found */

void d2_ShpBlit16HalfDark(unsigned int x, unsigned int y, D2SURF *surf, D2SHP *s)
{
    unsigned int *cp, *run;
    unsigned int i;
    unsigned short *line, *d;
    unsigned short *px;
    int r, g, b;
    int v;

#ifdef REA_FRAME_DUMP
    if (s->w > 300)
        REA_DUMP_TRACE((rd_f, "%u ShpBlit16%s %dx%d at %d,%d mode=%d bpp=%d\n", GetTickCount(), "HalfDark", s->w, s->h, x, y, s->mode, surf->bpp));
#endif

    cp = (unsigned int *)((char *)s + 0x32);
    line = surf->bits + (surf->pitch * y + x);
    run = (unsigned int *)((char *)s + 0x36);
    for (i = 0; i < s->h; i++) {
        d = line;
        for (x = 0; x < *cp; x++) {
            px = (unsigned short *)(run + 2);
            d += run[0] >> 1;
            for (y = 0; y < run[1]; y++) {
                dd_SplitRGB(*px, (unsigned char *)&r, (unsigned char *)&g, (unsigned char *)&b);
                v = (int)(((float)(r & 0xff) + (b & 0xff)) * 0.2f + (g & 0xff) * 0.59f);
                *d = (unsigned short)dd_RGB(v, v, v);
                px++;
                d++;
            }
            run = (unsigned int *)((char *)run + run[1] * 2 + 8);
        }
        cp = run;
        line += surf->pitch;
        run = cp + 1;
    }
}
