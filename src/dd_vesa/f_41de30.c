/* 0x41de30 */

void *dd_LockSurfaceFlags(LPDIRECTDRAWSURFACE4 surf, DWORD flags)
{
    DDSURFACEDESC2 ddsd;
    HRESULT hr;

    ddsd.dwSize = sizeof(ddsd);
    while (1) {
        hr = surf->lpVtbl->Lock(surf, NULL, &ddsd, flags | DDLOCK_NOSYSLOCK | DDLOCK_WAIT, NULL);
        if (hr == DD_OK) {
            dd_LockBits = ddsd.ddpfPixelFormat.dwRGBBitCount;
            dd_LockPitch = ddsd.lPitch;
            dd_Pitch = ddsd.lPitch;
            dd_LockBytes = ddsd.ddpfPixelFormat.dwRGBBitCount >> 3;
            dd_PitchPixels = (DWORD)ddsd.lPitch / dd_BytesPerPixel;
            return ddsd.lpSurface;
        }
        if (hr != DDERR_SURFACELOST)
            return NULL;
        if (surf->lpVtbl->Restore(surf) != DD_OK)
            return NULL;
    }
}
