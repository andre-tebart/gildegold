/* 0x479120, fills desc with the description of the DirectDraw surface of s */

int d2_GetSurfaceDesc(D2SURF *s, DDSURFACEDESC2 *desc)
{
    if (s->DDSurface == 0)
        return 0;
    desc->dwSize = 0x7c;
    return s->DDSurface->lpVtbl->GetSurfaceDesc(s->DDSurface, desc) == 0;
}
