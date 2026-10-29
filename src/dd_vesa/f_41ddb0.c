/* 0x41ddb0, locks the surface (DDLOCK_WAIT; on DDERR_SURFACELOST it restores and retries), caches the pixel
   format of the lock in dd_LockBits / dd_LockPitch / dd_Pitch / dd_LockBytes / dd_PitchPixels and returns the
   locked pixels, NULL when the lock failed */

void *dd_LockSurface(LPDIRECTDRAWSURFACE4 surf)
{
    DDSURFACEDESC2 ddsd;
    HRESULT hr;

    ddsd.dwSize = sizeof(ddsd);
    while (1) {
        hr = surf->lpVtbl->Lock(surf, NULL, &ddsd, DDLOCK_WAIT, NULL);
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
