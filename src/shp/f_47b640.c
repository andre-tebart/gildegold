/* 0x47b640, the light-table variant of d2_ShpBlit16Scaled: the same 1/n scaled, clipped draw of a 16 bit
   run-length shape, but every destination pixel goes through d2_ShpLightTable (used when the shape was grabbed
   with a palette); returns 0 when completely clipped, else 1.
   @deep behaviour draft (332 code hunks, 619/720 bytes): the light-table twin of 0x47b350 - every meaning read against 47b640..47b8ff and corresponds (the acc arithmetic made explicit: acc++; acc = (*run >> acc) % n, byte-neutral; the two clipped branches, d += ((*run>>1)+acc)/n, first = row%n==0, the literal run through d2_ShpLightTable[*px] every nth source pixel, the acc remainder, the run chain, line += pitch on first, early-outs vs ClipTop/ClipRight/ClipLeft/ClipBottom). The remaining hunks are the same codegen-only register/slot homes as 0x47b350 (line/acc/row spilled into parameter slots here, other homes in the original); no source meaning fix found */

int d2_ShpBlit16ScaledLit(int x, unsigned int y, D2SURF *surf, D2SHP *s, unsigned int n)
{
    int h, row, first;
    unsigned int acc, k, m;
    unsigned int *cp, *run, *next;
    unsigned short *line, *d, *px;

#ifdef REA_FRAME_DUMP
    if (s->w > 300)
        REA_DUMP_TRACE((rd_f, "%u ShpBlit16%s %dx%d at %d,%d mode=%d bpp=%d\n", GetTickCount(), "ScaledLit", s->w, s->h, x, y, s->mode, surf->bpp));
#endif

    cp = (unsigned int *)((char *)s + 0x32);
    run = (unsigned int *)((char *)s + 0x36);
    line = surf->bits + (surf->pitch * y + x);
    h = s->h;
    if ((int)(h + y) < d2_ClipTop)
        return 0;
    if ((int)(h / n + y) > d2_ClipBottom)
        h = (d2_ClipBottom - y) * n;
    acc = 0;
    if ((int)y < d2_ClipTop) {
        for (row = 0; row < h; row++) {
            first = row % n == 0;
            d = line;
            next = run;
            if (*cp != 0) {
                k = 0;
                do {
                    d += ((*run >> 1) + acc) / n;
                    acc++;
                    acc = (*run >> acc) % n;
                    if (first) {
                        px = (unsigned short *)(run + 2);
                        m = 0;
                        if ((run[1] + acc) / n != 0) {
                            do {
                                if (d2_ClipTop <= row / n + (int)y)
                                    *d = d2_ShpLightTable[*px];
                                px += n;
                                d++;
                                m++;
                            } while (m < (run[1] + acc) / n);
                        }
                        acc = (run[1] + acc) % n;
                    }
                    k++;
                    run = (unsigned int *)((char *)run + run[1] * 2 + 8);
                    next = run;
                } while (k < *cp);
            }
            if (first)
                line += surf->pitch;
            run = next + 1;
            cp = next;
        }
    } else {
        for (row = 0; row < h; row++) {
            first = row % n == 0;
            d = line;
            next = run;
            if (*cp != 0) {
                k = 0;
                do {
                    d += ((*run >> 1) + acc) / n;
                    acc++;
                    acc = (*run >> acc) % n;
                    if (first) {
                        px = (unsigned short *)(run + 2);
                        m = 0;
                        if ((run[1] + acc) / n != 0) {
                            do {
                                *d = d2_ShpLightTable[*px];
                                px += n;
                                d++;
                                m++;
                            } while (m < (run[1] + acc) / n);
                        }
                        acc = (run[1] + acc) % n;
                    }
                    k++;
                    run = (unsigned int *)((char *)run + run[1] * 2 + 8);
                    next = run;
                } while (k < *cp);
            }
            if (first)
                line += surf->pitch;
            run = next + 1;
            cp = next;
        }
    }
    return 1;
}
