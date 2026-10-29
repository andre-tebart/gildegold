/* 0x468e30, draws the saved background back for all dirty rectangles of the current page */

void d2_FlushDirty(void)
{
    int n = d2_DirtyCount[dd_Page];
    int i;
    D2RECT *r;

    if (n > 0) {
        i = 0;
        do {
            r = (D2RECT *)((char *)d2_Dirty[dd_Page] + i);
            d2_BlitPart(r->x, r->y, r->w, r->h, d2_BackSurf, r->x, r->y, d2_ScreenSurf);
            i += 0x10;
            n--;
        } while (n != 0);
    }
    d2_DirtyCount[dd_Page] = 0;
}
