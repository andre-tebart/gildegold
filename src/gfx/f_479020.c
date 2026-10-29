/* 0x479020, locks a surface: its pixels are at bits */

int d2_LockSurface(D2SURF *s)
{
    DDSURFACEDESC2 ddsd;

    if (s == 0)
        return 0;
    if (s->locked != 0)
        return 1;
    if (s->DDSurface != 0) {
        memset(&ddsd, 0, sizeof(ddsd));
        ddsd.dwSize = 0x7c;
        if (dd_Mode == 3) {
#line 1178
            D3DCHECK(s->DDSurface->lpVtbl->Lock( s->DDSurface, NULL, &ddsd, DDLOCK_WAIT, NULL ));
        } else {
#line 1180
            D3DCHECK(s->DDSurface->lpVtbl->Lock( s->DDSurface, NULL, &ddsd, DDLOCK_WAIT | DDLOCK_NOSYSLOCK, NULL ));
        }
        s->bits = (unsigned short *)ddsd.lpSurface;
    }
    s->locked = 1;
    return 1;
}
