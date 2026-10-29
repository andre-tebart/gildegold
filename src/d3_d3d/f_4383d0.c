/* 0x4383d0, creates a (dynamic) texture surface of size x size with mip levels down to 1 >> mips. */

void d3d_CreateTexture(DWORD size, DDPIXELFORMAT *pf, LPDIRECTDRAWSURFACE4 *surf, LPDIRECT3DTEXTURE2 *surftext,
                       DWORD n, BYTE *pal, unsigned char mips)
{
    DDSURFACEDESC2 ddsd;
    int levels;
    int m;

    if (*surf) {
        d3d_RefCount = (*surf)->lpVtbl->Release(*surf);
        *surf = 0;
    }
    if (*surftext) {
        d3d_RefCount = (*surftext)->lpVtbl->Release(*surftext);
        *surftext = 0;
    }
    if (!d3d_Mipmaps)
        mips = 0;
    if (pf->dwFlags & DDPF_FOURCC)
        err_Message("Bad dynamic pixel format...");
    D3_ZERO(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    if (size > d3d_UsedCard.info.maxw) {
        ddsd.dwWidth = d3d_UsedCard.info.maxw;
        ddsd.dwHeight = d3d_UsedCard.info.maxw;
    } else if (size < d3d_UsedCard.info.minw) {
        ddsd.dwWidth = d3d_UsedCard.info.minw;
        ddsd.dwHeight = d3d_UsedCard.info.minw;
    } else {
        ddsd.dwWidth = size;
        ddsd.dwHeight = size;
    }
    ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
    ddsd.ddpfPixelFormat = *pf;
    ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE;
    if ((int)(max(ddsd.dwHeight, ddsd.dwWidth) >> mips) > 0)
        levels = mips + 1;
    else
        for (m = max(ddsd.dwHeight, ddsd.dwWidth), levels = 0; m > 0; m >>= 1)
            levels++;
    if (levels > 1) {
        ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP | DDSCAPS_COMPLEX;
        ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT;
        ddsd.dwMipMapCount = levels;
        ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
    } else
        ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE | DDSCAPS2_HINTDYNAMIC;
#line 1208
    D3DCHECK(dd_DD->lpVtbl->CreateSurface(dd_DD, &ddsd, surf, NULL));
    dd_SetSurfaceName(*surf, "texture(createtexture(dyn))", 0);
    d3d_SetSurfacePalette(&ddsd, *surf, n, pal);
    D3DCHECK((*surf)->lpVtbl->QueryInterface(*surf, &IID_IDirect3DTexture2, (LPVOID *) surftext));
}
