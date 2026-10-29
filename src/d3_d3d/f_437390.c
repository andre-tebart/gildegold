/* 0x437390, loads an image file into a texture surface of format pf (scaled to the card's limits),
   with mip levels; palettized formats get the image's palette, others a color key from color 0.
   @deep behaviour: every code hunk is shape - the loadddsd/pal/adw stack-slot assignment (loadddsd at [ebp-0xb8] vs [ebp-0x9c], its field stores at +0x48/+0x68 move with it) and the dead-parameter slot reuse (stretcht/tempt in the w/h slots, [+0x1c] vs [+0x20], flipping between the level loop and the i loop), operand order [esi+eax]/[eax+esi] in the name copy, moved stores, tail nops; control flow and the w != ddsd.dwHeight quirk follow the original; readability met */

void d3d_LoadTexture(char *name, DDPIXELFORMAT *pf, LPDIRECTDRAWSURFACE4 *surf, LPDIRECT3DTEXTURE2 *surftext,
                     unsigned char mips, DWORD w, DWORD h)
{
    char msg[512];
    char namebuf[512];
    PALETTEENTRY adw[256];
    BYTE pal[768];
    DDSURFACEDESC2 tempddsd;
    DDSURFACEDESC2 dest;
    DDSURFACEDESC2 ddsd;
    DDSCAPS2 ddscaps2;
    char num[12];
    DDSURFACEDESC2 loadddsd;
    DDCOLORKEY ddk;
    LPDIRECTDRAWPALETTE Palette;
    LPDIRECTDRAWSURFACE4 loadt;
    LPDIRECTDRAWSURFACE4 texture;
    /* stretcht, tempt in blocks */
    DWORD x;
    int levels;
    int m;
    int i;
    char *s, *d;

    ddk.dwColorSpaceLowValue = 0;
    ddk.dwColorSpaceHighValue = 0;
    loadt = 0;
    if (!d3d_Mipmaps)
        mips = 0;
    if (*surf) {
        d3d_RefCount = (*surf)->lpVtbl->Release(*surf);
        *surf = 0;
    }
    if (*surftext) {
        d3d_RefCount = (*surftext)->lpVtbl->Release(*surftext);
        *surftext = 0;
    }
    D3_ZERO(pal, sizeof(pal));
    D3_ZERO(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE;
    ddsd.ddpfPixelFormat = *pf;
    ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
    x = w >> d3d_TexShift;
    if (x > d3d_UsedCard.info.maxw)
        ddsd.dwWidth = d3d_UsedCard.info.maxw;
    else if (x < d3d_UsedCard.info.minw)
        ddsd.dwWidth = d3d_UsedCard.info.minw;
    else
        ddsd.dwWidth = x;
    x = h >> d3d_TexShift;
    if (x > d3d_UsedCard.info.maxh)
        ddsd.dwHeight = d3d_UsedCard.info.maxh;
    else if (x < d3d_UsedCard.info.minh)
        ddsd.dwHeight = d3d_UsedCard.info.minh;
    else
        ddsd.dwHeight = x;
    if (pf->dwFlags & DDPF_PALETTEINDEXED8) {
        D3_ZERO(&loadddsd, sizeof(loadddsd));
        loadddsd.dwSize = sizeof(loadddsd);
        loadddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
        loadddsd.dwHeight = h;
        loadddsd.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN;
        loadddsd.dwWidth = w;
        loadddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
        loadddsd.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_PALETTEINDEXED8;
        loadddsd.ddpfPixelFormat.dwRGBBitCount = 8;
        D3_COPY3(&dest, &loadddsd, sizeof(dest));
#line 872
        D3DCHECK(dd_DD->lpVtbl->CreateSurface(dd_DD, &loadddsd, &loadt, NULL));
        dd_SetSurfaceName(loadt, "loadt8", 0);
        D3DCHECK(loadt->lpVtbl->Lock(loadt, 0, &loadddsd, DDLOCK_WAIT | DDLOCK_NOSYSLOCK, NULL));
        bmp_Load(name, (unsigned char *)loadddsd.lpSurface, pal, 7);
    } else {
        D3_ZERO(&loadddsd, sizeof(loadddsd));
        loadddsd.dwSize = sizeof(loadddsd);
        loadddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
        loadddsd.dwHeight = h;
        loadddsd.dwWidth = w;
        loadddsd.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN;
        loadddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
        loadddsd.ddpfPixelFormat.dwFlags = DDPF_RGB;
        loadddsd.ddpfPixelFormat.dwRGBBitCount = 24;
        loadddsd.ddpfPixelFormat.dwRBitMask = D3D_RGB24_RMASK;
        loadddsd.ddpfPixelFormat.dwGBitMask = D3D_RGB24_GMASK;
        loadddsd.ddpfPixelFormat.dwBBitMask = D3D_RGB24_BMASK;
        D3_COPY3(&dest, &loadddsd, sizeof(dest));
#line 892
        D3DCHECK(dd_DD->lpVtbl->CreateSurface(dd_DD, &loadddsd, &loadt, NULL));
        dd_SetSurfaceName(loadt, "loadt24", 0);
        D3DCHECK(loadt->lpVtbl->Lock(loadt, 0, &loadddsd, DDLOCK_WAIT | DDLOCK_NOSYSLOCK, NULL));
        bmp_Load(name, (unsigned char *)loadddsd.lpSurface, pal, 9);
    }
    if (w != ddsd.dwHeight || h != ddsd.dwHeight) {
        LPDIRECTDRAWSURFACE4 stretcht;

        dest.dwHeight = ddsd.dwHeight;
        dest.dwWidth = ddsd.dwWidth;
        dest.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN;
#line 904
        D3DCHECK(dd_DD->lpVtbl->CreateSurface(dd_DD, &dest, &stretcht, NULL));
        dd_SetSurfaceName(stretcht, "stretcht", 0);
        D3DCHECK(stretcht->lpVtbl->Lock(stretcht, 0, &dest, DDLOCK_WAIT | DDLOCK_NOSYSLOCK | DDLOCK_WRITEONLY, NULL));
        surf_ScaleBlit(&dest, &loadddsd);
        D3DCHECK(loadt->lpVtbl->Unlock(loadt, NULL));
        d3d_RefCount = loadt->lpVtbl->Release(loadt);
        loadt = stretcht;
        D3_COPY3(&loadddsd, &dest, sizeof(loadddsd));
    }
#line 913
    D3DCHECK(loadt->lpVtbl->Unlock(loadt, NULL));
    if ((max(ddsd.dwHeight, ddsd.dwWidth) >> mips) >= d3d_MinMipSize)
        levels = mips + 1;
    else
        for (m = max(ddsd.dwHeight, ddsd.dwWidth), levels = 0; m >= d3d_MinMipSize; m >>= 1)
            levels++;
    if (levels > 1) {
        ddsd.ddsCaps.dwCaps |= DDSCAPS_MIPMAP | DDSCAPS_COMPLEX;
        ddsd.dwFlags |= DDSD_MIPMAPCOUNT;
        ddsd.dwMipMapCount = levels;
    }
    ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
#line 931
    D3DCHECK(dd_DD->lpVtbl->CreateSurface(dd_DD, &ddsd, surf, NULL));
    texture = *surf;
    if (!texture) {
        sprintf(msg, "Could not load create surface for %s", name);
        err_Message(msg);
        return;
    }
    dd_SetSurfaceName(texture, name, 0);
#line 944
    D3DCHECK(texture->lpVtbl->QueryInterface(texture, &IID_IDirect3DTexture2, (LPVOID *)surftext));
    for (i = 0; i < 256; i++) {
        adw[i].peFlags = 0xff;
        adw[i].peRed = pal[i * 3];
        adw[i].peGreen = pal[i * 3 + 1];
        adw[i].peBlue = pal[i * 3 + 2];
    }
    adw[0].peFlags = 0;
    if (pf->dwRGBBitCount <= 8 && (pf->dwFlags & DDPF_PALETTEINDEXED8)) {
        Palette = 0;
#line 957
        D3DCHECK(dd_DD->lpVtbl->CreatePalette(dd_DD, DDPCAPS_8BIT | DDPCAPS_ALLOW256 | DDPCAPS_ALPHA, adw, &Palette, NULL));
        D3DCHECK(texture->lpVtbl->SetPalette(texture, Palette));
        d3d_RefCount = Palette->lpVtbl->Release(Palette);
        Palette = 0;
    } else
        ddk.dwColorSpaceHighValue = ddk.dwColorSpaceLowValue = RGB_MAKE(pal[0], pal[1], pal[2]);
#line 967
    D3DCHECK(loadt->lpVtbl->SetColorKey(loadt, DDCKEY_SRCBLT, &ddk));
    if (pf->dwFlags & DDPF_FOURCC) {
#line 970
        D3DCHECK(texture->lpVtbl->Blt(texture, NULL, loadt, NULL, DDBLT_WAIT, NULL));
    } else {
#line 973
        D3DCHECK(texture->lpVtbl->SetColorKey(texture, DDCKEY_SRCBLT, &ddk));
        D3DCHECK(loadt->lpVtbl->Lock(loadt, NULL, &loadddsd, DDLOCK_WAIT | DDLOCK_NOSYSLOCK | DDLOCK_READONLY, NULL));
        if (!d3d_hr) {
#line 977
            D3DCHECK(texture->lpVtbl->Lock(texture, NULL, &ddsd, DDLOCK_WAIT | DDLOCK_NOSYSLOCK | DDLOCK_WRITEONLY, NULL));
            if (!d3d_hr) {
                surf_Blit(&ddsd, 0, &loadddsd, adw);
#line 981
                D3DCHECK(texture->lpVtbl->Unlock(texture, NULL));
            }
#line 983
            D3DCHECK(loadt->lpVtbl->Unlock(loadt, NULL));
        }
    }
    if (levels > 1) {
        ddscaps2.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;
#line 992
        D3DCHECK(texture->lpVtbl->GetAttachedSurface(texture, &ddscaps2, &texture));
        if (!d3d_hr)
            texture->lpVtbl->Release(texture);
        for (i = 1; i < levels; i++) {
            LPDIRECTDRAWSURFACE4 tempt;

            _itoa(i, num, 10);
            for (s = name, d = namebuf; (*d++ = *s++) != 0; )
                ;
            strcat(namebuf, "_");
            strcat(namebuf, num);
            dd_SetSurfaceName(texture, namebuf, 0);
            D3_COPY3(&tempddsd, &dest, sizeof(tempddsd));
            tempddsd.dwWidth = loadddsd.dwWidth >> i;
            tempddsd.dwHeight = loadddsd.dwHeight >> i;
            tempddsd.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN;
#line 1013
            D3DCHECK(dd_DD->lpVtbl->CreateSurface(dd_DD, &tempddsd, &tempt, NULL));
            dd_SetSurfaceName(tempt, "tempt(mipmap)", 0);
#line 1016
            D3DCHECK(loadt->lpVtbl->Lock(loadt, NULL, &loadddsd, DDLOCK_WAIT | DDLOCK_NOSYSLOCK | DDLOCK_READONLY, NULL));
            if (!d3d_hr) {
#line 1019
                D3DCHECK(tempt->lpVtbl->Lock(tempt, NULL, &tempddsd, DDLOCK_WAIT | DDLOCK_NOSYSLOCK | DDLOCK_WRITEONLY, NULL));
                if (!d3d_hr) {
                    surf_ScaleBlit(&tempddsd, &loadddsd);
#line 1023
                    D3DCHECK(tempt->lpVtbl->Unlock(tempt, NULL));
                }
#line 1025
                D3DCHECK(loadt->lpVtbl->Unlock(loadt, NULL));
            }
#line 1027
            D3DCHECK(tempt->lpVtbl->SetColorKey(tempt, DDCKEY_SRCBLT, &ddk));
            if (ddsd.ddpfPixelFormat.dwFourCC) {
#line 1029
                D3DCHECK(texture->lpVtbl->Blt(texture, NULL, tempt, NULL, DDBLT_WAIT, NULL));
            } else {
#line 1032
                D3DCHECK(texture->lpVtbl->SetColorKey(texture, DDCKEY_SRCBLT, &ddk));
                D3DCHECK(texture->lpVtbl->Lock(texture, NULL, &ddsd, DDLOCK_WAIT | DDLOCK_NOSYSLOCK | DDLOCK_WRITEONLY, NULL));
                if (!d3d_hr) {
#line 1036
                    D3DCHECK(tempt->lpVtbl->Lock(tempt, NULL, &tempddsd, DDLOCK_WAIT | DDLOCK_NOSYSLOCK | DDLOCK_READONLY, NULL));
                    if (!d3d_hr) {
                        surf_Blit(&ddsd, 0, &tempddsd, adw);
#line 1040
                        D3DCHECK(tempt->lpVtbl->Unlock(tempt, NULL));
                    }
#line 1042
                    D3DCHECK(texture->lpVtbl->Unlock(texture, NULL));
                }
            }
            d3d_RefCount = tempt->lpVtbl->Release(tempt);
            tempt = 0;
            if (i < levels - 1) {
                ddscaps2.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;
#line 1054
                D3DCHECK(texture->lpVtbl->GetAttachedSurface(texture, &ddscaps2, &texture));
                if (!d3d_hr)
                    texture->lpVtbl->Release(texture);
            }
        }
    }
    d3d_RefCount = loadt->lpVtbl->Release(loadt);
}
