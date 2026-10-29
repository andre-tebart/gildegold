/* 0x479610, blits the part (sx, sy, w, h) of the DirectDraw surface img to (x, y) of the surface surf, clipped to
   the surface edges; 1 on success, 0 when the clip leaves nothing or img is NULL (the reverse of
   d2_BlitPartToDD, the d2.c texture upload calls it) */

int d2_BlitPartFromDD(int sx, int sy, int w, int h, LPDIRECTDRAWSURFACE4 img, int x, int y, D2SURF *surf)
{
    RECT src, dst;

    if (sy < 0) {
        h += sy;
        sy = 0;
    }
    if (sx < 0) {
        w += sx;
        sx = 0;
    }
    if (x < 0) {
        sx -= x;
        w += x;
        x = 0;
    }
    if (y < 0) {
        sy -= y;
        h += y;
        y = 0;
    }
    if (h != 0 && w != 0) {
        if (img != 0) {
            src.left = sx;
            src.top = sy;
            dst.top = y;
            src.bottom = sy + h;
            src.right = sx + w;
            dst.left = x;
            dst.bottom = y + h;
            dst.right = x + w;
            surf->DDSurface->lpVtbl->Blt(surf->DDSurface, &dst, img, &src, DDBLT_WAIT, 0);
        }
        return 1;
    }
    return 0;
}
