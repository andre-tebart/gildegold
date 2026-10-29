/* 0x41e740, unlocks the surface, nothing when surf is NULL */

void dd_UnlockSurface(LPDIRECTDRAWSURFACE4 surf)
{
    if (surf)
        surf->lpVtbl->Unlock(surf, NULL);
}
