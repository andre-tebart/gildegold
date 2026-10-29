/* 0x41e760, Bresenham into the locked 16-bit dd_Surface */

void dd_DrawLine16(int x0, int y0, int x1, int y1, int color)
{
    int dx, dy, sx, sy, e, n, p, inc1, inc2;

    if (y0 == y1) {
        if (x1 < x0) {
            p = dd_PitchPixels * y0 + x1;
            n = x0 - x1;
        } else {
            p = dd_PitchPixels * y0 + x0;
            n = x1 - x0;
        }
        for (; n > 0; n--)
            ((unsigned short *)dd_Surface)[p++] = color;
    } else if (x0 == x1) {
        if (y1 < y0) {
            p = dd_PitchPixels * y1 + x0;
            n = y0 - y1;
        } else {
            p = dd_PitchPixels * y0 + x0;
            n = y1 - y0;
        }
        for (; n > 0; n--) {
            ((unsigned short *)dd_Surface)[p] = color;
            p += dd_PitchPixels;
        }
    } else {
        if (x0 < x1) {
            sx = 1;
            dx = x1 - x0;
        } else {
            sx = -1;
            dx = x0 - x1;
        }
        if (y0 < y1) {
            sy = 1;
            dy = y1 - y0;
        } else {
            sy = -1;
            dy = y0 - y1;
        }
        ((unsigned short *)dd_Surface)[dd_PitchPixels * y0 + x0] = color;
        if (dx > dy) {
            inc2 = 2 * (dy - dx);
            inc1 = 2 * dy;
            e = inc1 - dx;
            do {
                x0 += sx;
                if (e >= 0) {
                    y0 += sy;
                    e += inc2;
                } else
                    e += inc1;
                ((unsigned short *)dd_Surface)[dd_PitchPixels * y0 + x0] = color;
            } while (x0 != x1);
        } else {
            inc2 = 2 * (dx - dy);
            inc1 = 2 * dx;
            e = inc1 - dy;
            do {
                y0 += sy;
                if (e >= 0) {
                    x0 += sx;
                    e += inc2;
                } else
                    e += inc1;
                ((unsigned short *)dd_Surface)[dd_PitchPixels * y0 + x0] = color;
            } while (y0 != y1);
        }
    }
}
