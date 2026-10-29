/* 0x47ba60, draws a 16 bit shape made of plain rows (nruns == -1) onto a surface, clipped to the ClipLeft /
   ClipRight / ClipTop / ClipBottom rectangle: mode 0 copies the non-zero pixels, mode 2 halves them through
   d2_HighBitMask, mode 3 maps them through d2_ShpLightTable; d2_ShpW / d2_ShpH are set to the shape size.
   @deep behaviour draft (402 code hunks, 670/768 bytes): every meaning was read against 47ba60..47bd6f and corresponds - the x clip (ClipLeft -> skipl, ClipRight -> skipr, return when outside or when w+x is left of ClipLeft), the y clip (ClipTop -> rows, ClipBottom -> h = ClipBottom - y, the two early returns), clipped = 1 for both, d = bits + (pitch*y + x), the six loop copies (clipped/unclipped x mode 0/2/3; mode 2 is *d = *d >> 1 & d2_HighBitMask, mode 3 is d2_ShpLightTable[*d], both only when *p != 0), p += w*rows / skipl / skipr, d += pitch - vis, and the d2_ShpH / d2_ShpW stores. What remains is codegen / register allocation only (the 6 loop copies compile with other register and slot homes, 12 regs hunks) - no source meaning fix found */

void d2_ShpBlit16Rows(int x, int y, D2SURF *surf, D2SHP *s)
{
    unsigned int w, h, n;
    int clipped, skipl, skipr, vis, rows;
    unsigned short *d, *p;
    char mode;

#ifdef REA_FRAME_DUMP
    if (s->w > 300)
        REA_DUMP_TRACE((rd_f, "%u ShpBlit16%s %dx%d at %d,%d mode=%d bpp=%d\n", GetTickCount(), "Rows", s->w, s->h, x, y, s->mode, surf->bpp));
#endif

    w = s->w;
    h = s->h;
    clipped = 0;
    skipl = 0;
    skipr = 0;
    vis = w;
    if (x < d2_ClipLeft) {
        rows = d2_ClipLeft - x;
        x = d2_ClipLeft;
        skipl = rows;
    clip1:
        clipped = 1;
        vis = w - rows;
    } else {
        if (x > d2_ClipRight)
            return;
        if ((int)(w + x) < d2_ClipLeft)
            return;
        if ((int)(w + x) > d2_ClipRight) {
            rows = (w - d2_ClipRight) + x;
            skipr = rows;
            goto clip1;
        }
    }
    if (y < d2_ClipTop) {
        rows = d2_ClipTop - y;
        h = h - rows;
        y = d2_ClipTop;
    } else {
        if (y > d2_ClipBottom)
            return;
        if ((int)(h + y) < d2_ClipTop)
            return;
        rows = 0;
        if ((int)(h + y) <= d2_ClipBottom)
            goto clip2;
        h = d2_ClipBottom - y;
    }
    clipped = 1;
clip2:
    d = surf->bits + (surf->pitch * y + x);
    mode = s->mode;
    p = (unsigned short *)((char *)s + 0x32);
    if (clipped) {
        if (mode == 0) {
            p = p + w * rows;
            for (; h > 0; h--) {
                p = p + skipl;
                for (n = vis; n > 0; n--) {
                    if (*p != 0)
                        *d = *p;
                    d++;
                    p++;
                }
                p = p + skipr;
                d = d + (surf->pitch - vis);
            }
        } else if (mode == 2) {
            p = p + w * rows;
            for (; h > 0; h--) {
                p = p + skipl;
                for (n = vis; n > 0; n--) {
                    if (*p != 0)
                        *d = *d >> 1 & d2_HighBitMask;
                    d++;
                    p++;
                }
                p = p + skipr;
                d = d + (surf->pitch - vis);
            }
        } else if (mode == 3) {
            p = p + w * rows;
            for (; h > 0; h--) {
                p = p + skipl;
                for (n = vis; n > 0; n--) {
                    if (*p != 0)
                        *d = d2_ShpLightTable[*d];
                    d++;
                    p++;
                }
                p = p + skipr;
                d = d + (surf->pitch - vis);
            }
        }
    } else if (mode == 0) {
        for (; h > 0; h--) {
            for (n = vis; n > 0; n--) {
                if (*p != 0)
                    *d = *p;
                d++;
                p++;
            }
            d = d + (surf->pitch - vis);
        }
    } else if (mode == 2) {
        for (; h > 0; h--) {
            for (n = vis; n > 0; n--) {
                if (*p != 0)
                    *d = *d >> 1 & d2_HighBitMask;
                d++;
                p++;
            }
            d = d + (surf->pitch - vis);
        }
    } else if (mode == 3) {
        for (; h > 0; h--) {
            for (n = vis; n > 0; n--) {
                if (*p != 0)
                    *d = d2_ShpLightTable[*d];
                d++;
                p++;
            }
            d = d + (surf->pitch - vis);
        }
    }
    d2_ShpH = s->h;
    d2_ShpW = s->w;
}
