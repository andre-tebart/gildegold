/* 0x47ce60, fills a rectangle (clipped at the top and bottom) */

void d2_FillRect(int x, int y, int w, int h, void *surf)
{
    int old;
    D2SURF *s = (D2SURF *)surf;

#ifdef REA_FRAME_DUMP
    if (w > 300)
        REA_DUMP_TRACE((rd_f, "%u FillRect %dx%d at %d,%d bpp=%d\n", GetTickCount(), w, h, x, y, s->bpp));
#endif
    if (y < d2_ClipTop) {
        h = h + (y - d2_ClipTop);
        y = 0;
    }
    if (y + h > d2_ClipBottom)
        h = d2_ClipBottom - y - 1;
    old = d2_SurfPitch;
    d2_SurfPitch = s->pitch;
    d2_ShpDarken16(s->bits + (s->pitch * y + x), w, h);
    d2_SurfPitch = old;
}
