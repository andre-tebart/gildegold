/* 0x4797d0, clears the rectangle (x, y, w, h) of surf to black, clipped to the surface's width / height: a colour
   fill Blt for a DirectDraw surface, while a system memory surface is cleared whole (the rectangle is ignored);
   returns 1 (0 only for a NULL surf) */

int d2_ClearRect(int x, int y, int w, int h, D2SURF *surf)
{
    int locked;
    int l;
    RECT r;
    DDBLTFX fx;

    if (surf == 0)
        return 0;
    locked = surf->locked;
    if (locked != 0)
        d2_UnlockSurface(surf);
    if (surf->DDSurface == 0) {
        memset(surf->bits, 0, surf->rowbytes * surf->h);
    } else {
        if (x < surf->left)
            x = surf->left;
        r.left = x;
        l = x;
        r.top = y;
        r.right = l + w;
        r.bottom = y + h;
        if (r.right > surf->w - 1)
            r.right = surf->w;
        if (r.bottom > surf->h - 1)
            r.bottom = surf->h;
        if (l < surf->w && y < surf->h) {
            memset(&fx, 0, sizeof(fx));
            fx.dwFillColor = 0;
            fx.dwSize = 100;
            surf->DDSurface->lpVtbl->Blt(surf->DDSurface, &r, 0, 0, DDBLT_COLORFILL | DDBLT_WAIT, &fx);
        }
    }
    if (locked != 0) {
        d2_LockSurface(surf);
        return 1;
    }
    d2_UnlockSurface(surf);
    return 1;
}
