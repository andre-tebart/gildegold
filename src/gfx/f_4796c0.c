/* 0x4796c0, clears the pixels of surf to black: a memset for a system memory surface, a colour fill Blt for a
   DirectDraw one; returns 1 (0 only for a NULL surf) */

int d2_ClearSurface(D2SURF *surf)
{
    int locked;
    DDBLTFX fx;

    if (surf == 0)
        return 0;
    locked = surf->locked;
    if (surf->DDSurface == 0) {
        if (locked != 0)
            d2_LockSurface(surf);
        memset(surf->bits, 0, surf->rowbytes * surf->h);
    } else {
        if (locked != 0)
            d2_UnlockSurface(surf);
        memset(&fx, 0, sizeof(fx));
        fx.dwFillColor = 0;
        fx.dwSize = 100;
        surf->DDSurface->lpVtbl->Blt(surf->DDSurface, 0, 0, 0, DDBLT_COLORFILL | DDBLT_WAIT, &fx);
    }
    if (locked != 0) {
        d2_LockSurface(surf);
        return 1;
    }
    d2_UnlockSurface(surf);
    return 1;
}
