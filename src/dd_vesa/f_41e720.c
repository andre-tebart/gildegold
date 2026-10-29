/* 0x41e720, releases the surface (dd_Ref = the Release result, the last one is the ComRefCount) */

void dd_ReleaseSurface(LPDIRECTDRAWSURFACE4 surf)
{
    if (surf)
        dd_Ref = surf->lpVtbl->Release(surf);
}
