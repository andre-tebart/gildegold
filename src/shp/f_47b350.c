/* 0x47b350, draws a 16 bit run-length shape at 1/n size, clipped to d2_ClipLeft / d2_ClipRight / d2_ClipTop /
   d2_ClipBottom: skip runs are divided by n with the fractional accumulator carried across runs and rows, and
   every n-th source pixel of a literal run lands on the destination; returns 0 when the shape is completely
   clipped, else 1.
   @deep behaviour draft (345 code hunks, 664/752 bytes, frame 0x18 both sides): the meaning was read against 47b350..47b63f and corresponds, including the acc arithmetic (ours had (*run >> 1 + acc) % n, which compiles to the original's inc ecx + shr eax,cl; made explicit as acc++; acc = (*run >> acc) % n with no byte change), the two clipped branches (row/n + y vs ClipTop only in the y<ClipTop branch), d += ((*run>>1)+acc)/n, first = row%n==0, the literal run of (run[1]+acc)/n pixels every nth source pixel, the acc remainder after it, run = run + run[1]*2 + 8, next run chain, line += pitch on first, and the three early-outs vs ClipTop/ClipRight/ClipLeft/ClipBottom. What differs is codegen only: ours spills line into the s parameter slot, acc into the x slot and row into a local, the original keeps d/m/px in other registers and row/n in a slot - no source meaning fix found */

int d2_ShpBlit16Scaled(int x, unsigned int y, D2SURF *surf, D2SHP *s, unsigned int n)
{
    int h, row, first;
    unsigned int acc, k, m;
    unsigned int *cp, *run, *next;
    unsigned short *line, *d, *px;

#ifdef REA_FRAME_DUMP
    if (s->w > 300)
        REA_DUMP_TRACE((rd_f, "%u ShpBlit16%s %dx%d at %d,%d mode=%d bpp=%d\n", GetTickCount(), "Scaled", s->w, s->h, x, y, s->mode, surf->bpp));
#endif
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
                                    *d = *px;
                                px += n;
                                d++;
                                m++;
                            } while (m < (run[1] + acc) / n);
                        }
                        acc = (run[1] + acc) % n;
                    }
                    run = (unsigned int *)((char *)run + run[1] * 2 + 8);
                    k++;
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
                                *d = *px;
                                px += n;
                                d++;
                                m++;
                            } while (m < (run[1] + acc) / n);
                        }
                        acc = (run[1] + acc) % n;
                    }
                    run = (unsigned int *)((char *)run + run[1] * 2 + 8);
                    k++;
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
