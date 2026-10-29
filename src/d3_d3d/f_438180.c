/* 0x438180, gives a palettized surface and its mip levels the palette and the color key. */

void d3d_SetSurfacePalette(DDSURFACEDESC2 *desc, LPDIRECTDRAWSURFACE4 surf, DWORD n, BYTE *pal)
{
    PALETTEENTRY adw[256];
    DDSURFACEDESC2 ddsd_back;
    DDSCAPS2 ddscaps2;
    DDCOLORKEY ddk;
    LPDIRECTDRAWPALETTE Palette;
    DWORD i;

    ddk.dwColorSpaceLowValue = 0;
    ddk.dwColorSpaceHighValue = 0;
    if (surf) {
        if (!desc) {
            ddsd_back.dwSize = sizeof(ddsd_back);
#line 1089
            D3DCHECK(surf->lpVtbl->GetSurfaceDesc(surf, &ddsd_back));
            desc = &ddsd_back;
        }
        if (n && desc->ddpfPixelFormat.dwRGBBitCount <= 8 && (desc->ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8)) {
            D3_ZERO(adw, sizeof(adw));
            for (i = 0; i < n; i++) {
                adw[i].peFlags = 0xff;
                adw[i].peRed = pal[i * 3];
                adw[i].peGreen = pal[i * 3 + 1];
                adw[i].peBlue = pal[i * 3 + 2];
            }
            adw[0].peFlags = 0;
#line 1106
            D3DCHECK(dd_DD->lpVtbl->CreatePalette(dd_DD, DDPCAPS_8BIT | DDPCAPS_ALLOW256 | DDPCAPS_ALPHA, adw, &Palette, NULL));
            D3DCHECK(surf->lpVtbl->SetPalette(surf, Palette));
            d3d_RefCount = Palette->lpVtbl->Release(Palette);
        }
        while (surf) {
#line 1116
            D3DCHECK(surf->lpVtbl->SetColorKey(surf, DDCKEY_SRCBLT, NULL));
            if (desc->ddpfPixelFormat.dwRGBBitCount <= 8 && (desc->ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8)) {
                if (n) {
#line 1120
                    D3DCHECK(surf->lpVtbl->SetColorKey(surf, DDCKEY_SRCBLT, &ddk));
                }
            } else {
                ddk.dwColorSpaceLowValue = 0;
                ddk.dwColorSpaceHighValue = 0;
#line 1126
                D3DCHECK(surf->lpVtbl->SetColorKey(surf, DDCKEY_SRCBLT, &ddk));
            }
            ddscaps2.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;
            if (surf->lpVtbl->GetAttachedSurface(surf, &ddscaps2, &surf) != DD_OK)
                return;
            surf->lpVtbl->Release(surf);
        }
    }
}
