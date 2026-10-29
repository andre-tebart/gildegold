/* 0x468eb0, adds a rectangle (clipped, x made even) to the dirty list of the current page */

void d2_AddDirty(int x, int y, int w, int h)
{
    int i = dd_Page;
    int n = d2_DirtyCount[i];
    D2RECT **dp;

    if (n < 0x300) {
        if (x > d2_ClipRight)
            x = d2_ClipRight;
        if (x & 1) {
            x--;
            w = (w + 2) & ~1;
        }
        if (y + h > d2_ClipBottom)
            h = d2_ClipBottom - y;
        if (x + w > d2_ClipRight)
            w = d2_ClipRight - x;
        if (y < d2_ClipTop)
            y = d2_ClipTop;
        if (x < d2_ClipLeft)
            x = d2_ClipLeft;
        if (0 < w && 0 < h && (y <= d2_DirtyRectTop || y + h >= d2_DirtyRectBottom || x <= d2_DirtyRectLeft ||
                               x + w >= d2_DirtyRectRight)) {
            dp = &d2_Dirty[dd_Page];
            (*dp)[n].x = x;
            (*dp)[n].y = y;
            (*dp)[n].w = w;
            (*dp)[n].h = h;
            d2_DirtyCount[i]++;
        }
    }
}
