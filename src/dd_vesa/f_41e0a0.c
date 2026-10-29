/* 0x41e0a0, color-fills rect (NULL = the whole surface) of surf with a DDBLT_COLORFILL Blt, restoring the
   surface and retrying after DDERR_SURFACELOST; returns 1 when the fill went through, 0 otherwise */

char dd_FillSurface(LPDIRECTDRAWSURFACE4 surf, RECT *rect, DWORD color)
{
    DDBLTFX fx;
    HRESULT hr;

    fx.dwSize = sizeof(fx);
    fx.dwFillColor = color;
    while (1) {
        hr = surf->lpVtbl->Blt(surf, rect, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fx);
        if (hr == DD_OK)
            return 1;
        if (hr != DDERR_SURFACELOST)
            break;
        surf->lpVtbl->Restore(surf);
    }
    return 0;
}
