/* 0x4371d0, fills the attached mip levels by scaling each level down into the next. */

void d3d_BuildMipmaps(LPDIRECTDRAWSURFACE4 tex, DDSURFACEDESC2 *ddsd)
{
    DDSURFACEDESC2 ddsd_back;
    DDSURFACEDESC2 ddsd2;
    DDSCAPS2 ddscaps2;
    LPDIRECTDRAWSURFACE4 tex2;
    char locked = 0;

    if (tex) {
        ddscaps2.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;
        if (tex->lpVtbl->GetAttachedSurface(tex, &ddscaps2, &tex2) == DD_OK) {
            if (!ddsd) {
                D3_ZERO(&ddsd_back, sizeof(ddsd_back));
                ddsd_back.dwSize = sizeof(ddsd_back);
#line 782
                D3DCHECK(tex->lpVtbl->Lock(tex, 0, &ddsd_back, DDLOCK_WAIT, NULL));
                ddsd = &ddsd_back;
                locked = 1;
            }
            D3_ZERO(&ddsd2, sizeof(ddsd2));
            ddsd2.dwSize = sizeof(ddsd2);
#line 790
            D3DCHECK(tex2->lpVtbl->Lock(tex2, 0, &ddsd2, DDLOCK_WAIT, NULL));
            surf_ScaleBlit(&ddsd2, ddsd);
            d3d_BuildMipmaps(tex2, &ddsd2);
#line 795
            D3DCHECK(tex2->lpVtbl->Unlock(tex2, NULL));
            if (locked) {
#line 798
                D3DCHECK(tex->lpVtbl->Unlock(tex, NULL));
            }
        }
    }
}
