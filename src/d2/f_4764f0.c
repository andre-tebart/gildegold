/* 0x4764f0, line on the paintbox (Bresenham) @deep B3 far draft, readable (Q1 fixed this session: d2_PlotLine -> d2_PlotLine; Q2 clean). 496 bytes, 321 code hunks. Every branch read against the original: the y0==y1 pair, the x0==x1 pair (count = difference, do/while --count) and the two Bresenham arms (sx/sy/dx/dy, e = dy*2 - dx, the d2_Plot pushes b,g,r,y,x) all match; the original keeps sx/sy and the two precomputed steps ((dy-dx)*2, dy*2) in stack slots and re-reads the parameters from their slots each iteration, ours keeps them in registers with a common step tail. Tried the menu dup-tail form (write the e update in both if/else arms, 'step' removed): 321 -> 323 code hunks, 12781 -> 12800 differing bytes, reverted. Remaining: whole-function frame / register allocation and the spill of the parameters -> deep (listing/slotmap) */

void d2_PlotLine(int x0, int y0, int x1, int y1, int r, int g, int b)
{
    int e, step, sx, sy, dx, dy;

    if (y0 == y1) {
        if (x1 < x0) {
            x0 = x0 - x1;
            if (0 < x0) {
                do {
                    d2_Plot(x1++, y0, r, g, b);
                } while (--x0 != 0);
                return;
            }
        } else {
            x1 = x1 - x0;
            if (0 < x1) {
                do {
                    d2_Plot(x0++, y0, r, g, b);
                } while (--x1 != 0);
                return;
            }
        }
    } else if (x0 == x1) {
        if (y1 < y0) {
            y0 = y0 - y1;
            if (0 < y0) {
                do {
                    d2_Plot(x1, y1++, r, g, b);
                } while (--y0 != 0);
                return;
            }
        } else {
            y1 = y1 - y0;
            if (0 < y1) {
                do {
                    d2_Plot(x1, y0++, r, g, b);
                } while (--y1 != 0);
                return;
            }
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
        d2_Plot(x0, y0, r, g, b);
        if (dy < dx) {
            e = dy * 2 - dx;
            do {
                x0 = x0 + sx;
                step = dy * 2;
                if (-1 < e) {
                    y0 = y0 + sy;
                    step = (dy - dx) * 2;
                }
                e = e + step;
                d2_Plot(x0, y0, r, g, b);
            } while (x0 != x1);
            return;
        }
        e = dx * 2 - dy;
        do {
            y0 = y0 + sy;
            step = dx * 2;
            if (-1 < e) {
                x0 = x0 + sx;
                step = (dx - dy) * 2;
            }
            e = e + step;
            d2_Plot(x0, y0, r, g, b);
        } while (y0 != y1);
    }
}
