/* 0x416980. Level 8 of each chain is converted from the texture "snow_256_ns" (or from an empty
   256x256 image if it can't be locked), the smaller levels are scaled down from it.
   NONMATCHING: ~140 asm lines. Control flow and frame match; left: the D3_ZERO slots, height stored
   via the ddpf pointer in the mip loops, mul/store order of the 16/32-bit pitch, and in the dither
   loop the slots of n/up/row/down and n being kept in a register (the original reloads it).
   @deep B3 code hunks 231, slots 30: readability done here (Q3 D3_MASK_RED, Q4 flags.all); the reload lead for n was tried (size * size inlined in the random-fill loop) and gave no codegen change; left is allocation/scheduling only - the D3_ZERO slot forms, the ddpf height stores, the 16/32-bit pitch mul/store order and the n/up/row/down slots (control flow and frame verified equal, prior reading in the address comment); no Q failure remains */

/* Software mipmap chains of the snow texture, levels 1x1..256x256 (index = log2 size), built by
   d3t_CreateSnowMipmaps and freed by d3t_FreeSnowMipmaps. */
D3RECT d3_Viewport = {0};  /* 0xfc2930 */
DDSURFACEDESC2 d3t_SnowMip8[9] = {0};       /* 0x14661c0, 8-bit palettized */
D3VECTOR d3_Unk1466620 = {0};  /* 0x1466620, camera target position */
D3VECTOR d3_Unk1466630 = {0};  /* 0x1466630, camera target angles */
DDSURFACEDESC2 d3t_SnowDither[9] = {0};     /* 0x1466640, 8-bit blurred noise */
DDSURFACEDESC2 d3t_SnowMip16[9] = {0};      /* 0x1466aa0 */
DDSURFACEDESC2 d3t_SnowMip32[9] = {0};      /* 0x1466f00 */

#define D3_MASK_RED 0xff0000u    /* red channel of the X8R8G8B8 snow surfaces below */
void d3t_CreateSnowMipmaps(void)
{
    D3TEXHANDLE *snow_tex;
    DDSURFACEDESC2 snow_ddsd;
    LPDIRECTDRAWPALETTE lppal;
    PALETTEENTRY pal[1024];
    D3TEXFLAGS flags;
    float *tmp, *row, *up, *down;
    float s, lo, hi, ofs, scale, t;
    int i, j, k, x, y, size, n, cnt;

    flags.all = 0;
    snow_tex = (D3TEXHANDLE *)tex_Load("snow_256_ns", flags, 0, 0);
    lppal = 0;
    if (snow_tex) {
        tex_SetHiColor((D3TEXTURE *)snow_tex, 1);
        tex_Reload(snow_tex, 0);
        if (snow_tex->surf) {
            D3_ZERO(&snow_ddsd, sizeof(snow_ddsd));
            snow_ddsd.dwSize = sizeof(snow_ddsd);
#line 3749
            D3DCHECK(snow_tex->surf->lpVtbl->Lock(snow_tex->surf, 0, &snow_ddsd, DDLOCK_WAIT|DDLOCK_READONLY, NULL));
            if (!d3d_hr) {
                if (snow_ddsd.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8) {
#line 3758
                    D3DCHECK(snow_tex->surf->lpVtbl->GetPalette(snow_tex->surf, &lppal));
                    if (d3d_hr) {
#line 3762
                        D3DCHECK(snow_tex->surf->lpVtbl->Unlock(snow_tex->surf, NULL));
                        tex_Release(snow_tex);
                        snow_tex = 0;
                    } else {
#line 3768
                        D3DCHECK(lppal->lpVtbl->GetEntries(lppal, 0, 0, 256, pal));
                    }
                }
            } else {
                tex_Release(snow_tex);
                snow_tex = 0;
            }
        } else {
            tex_Release(snow_tex);
            snow_tex = 0;
        }
    }
    if (!snow_tex) {
    snow_ddsd.dwSize = sizeof(snow_ddsd);
    snow_ddsd.dwWidth = 256;
    snow_ddsd.dwHeight = 256;
    snow_ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
    snow_ddsd.ddpfPixelFormat.dwFlags = DDPF_PALETTEINDEXED8;
    snow_ddsd.ddpfPixelFormat.dwFourCC = 0;
    snow_ddsd.ddpfPixelFormat.dwRGBBitCount = 0;
    snow_ddsd.ddpfPixelFormat.dwRBitMask = 0;
    snow_ddsd.ddpfPixelFormat.dwGBitMask = 0;
    snow_ddsd.ddpfPixelFormat.dwBBitMask = 0;
    snow_ddsd.ddpfPixelFormat.dwRGBAlphaBitMask = 0;
    snow_ddsd.lPitch = 256;
    snow_ddsd.lpSurface = mem_Alloc(256 * 256, "d3t:snow_ddsd");
    if (!snow_ddsd.lpSurface)
        return;
    }

    d3t_SnowMip8[8].dwSize = sizeof(DDSURFACEDESC2);
    d3t_SnowMip8[8].dwWidth = 256;
    d3t_SnowMip8[8].dwHeight = 256;
    d3t_SnowMip8[8].ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
    d3t_SnowMip8[8].ddpfPixelFormat.dwFlags = DDPF_PALETTEINDEXED8;
    d3t_SnowMip8[8].ddpfPixelFormat.dwFourCC = 0;
    d3t_SnowMip8[8].ddpfPixelFormat.dwRGBBitCount = 8;
    d3t_SnowMip8[8].ddpfPixelFormat.dwRBitMask = 0;
    d3t_SnowMip8[8].ddpfPixelFormat.dwGBitMask = 0;
    d3t_SnowMip8[8].ddpfPixelFormat.dwBBitMask = 0;
    d3t_SnowMip8[8].ddpfPixelFormat.dwRGBAlphaBitMask = 0;
    d3t_SnowMip8[8].lPitch = 256;
    d3t_SnowMip8[8].lpSurface = mem_Alloc(256 * 256, "d3t:SnowMipMap8");
    if (!d3t_SnowMip8[8].lpSurface)
        return;
    surf_Blit(&d3t_SnowMip8[8], pal, &snow_ddsd, pal);
    for (i = 0; i < 8; i++) {
        size = 1 << i;
        d3t_SnowMip8[i].dwSize = sizeof(DDSURFACEDESC2);
        d3t_SnowMip8[i].dwHeight = size;
        d3t_SnowMip8[i].dwWidth = size;
        d3t_SnowMip8[i].ddpfPixelFormat = d3t_SnowMip8[8].ddpfPixelFormat;
        d3t_SnowMip8[i].lPitch = size;
        d3t_SnowMip8[i].lpSurface = mem_Alloc(size * size, "d3t:SnowMipMap8");
        if (!d3t_SnowMip8[i].lpSurface)
            return;
        surf_ScaleBlit(&d3t_SnowMip8[i], &d3t_SnowMip8[8]);
    }

    d3t_SnowMip16[8].dwSize = sizeof(DDSURFACEDESC2);
    d3t_SnowMip16[8].dwWidth = 256;
    d3t_SnowMip16[8].dwHeight = 256;
    d3t_SnowMip16[8].ddpfPixelFormat = d3d_UsedTexFormat.pf2;
    d3t_SnowMip16[8].lPitch = 256 * 2;
    d3t_SnowMip16[8].lpSurface = mem_Alloc(256 * 256 * 2, "d3t:SnowMipMap16");
    if (!d3t_SnowMip16[8].lpSurface)
        return;
    surf_Blit(&d3t_SnowMip16[8], pal, &snow_ddsd, pal);
    for (i = 0; i < 8; i++) {
        size = 1 << i;
        d3t_SnowMip16[i].dwSize = sizeof(DDSURFACEDESC2);
        d3t_SnowMip16[i].dwHeight = size;
        d3t_SnowMip16[i].dwWidth = size;
        d3t_SnowMip16[i].ddpfPixelFormat = d3t_SnowMip16[8].ddpfPixelFormat;
        d3t_SnowMip16[i].lPitch = size * 2;
        d3t_SnowMip16[i].lpSurface = mem_Alloc(size * d3t_SnowMip16[i].lPitch, "d3t:SnowMipMap16");
        if (!d3t_SnowMip16[i].lpSurface)
            return;
        surf_ScaleBlit(&d3t_SnowMip16[i], &d3t_SnowMip16[8]);
    }

    d3t_SnowMip32[8].dwSize = sizeof(DDSURFACEDESC2);
    d3t_SnowMip32[8].dwWidth = 256;
    d3t_SnowMip32[8].dwHeight = 256;
    d3t_SnowMip32[8].ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
    d3t_SnowMip32[8].ddpfPixelFormat.dwFlags = DDPF_RGB;
    d3t_SnowMip32[8].ddpfPixelFormat.dwFourCC = 0;
    d3t_SnowMip32[8].ddpfPixelFormat.dwRGBBitCount = 32;
    d3t_SnowMip32[8].ddpfPixelFormat.dwRBitMask = D3_MASK_RED;
    d3t_SnowMip32[8].ddpfPixelFormat.dwGBitMask = 0xff00;
    d3t_SnowMip32[8].ddpfPixelFormat.dwBBitMask = 0xff;
    d3t_SnowMip32[8].ddpfPixelFormat.dwRGBAlphaBitMask = 0;
    d3t_SnowMip32[8].lPitch = 256 * 4;
    d3t_SnowMip32[8].lpSurface = mem_Alloc(256 * 256 * 4, "d3t:SnowMipMap32");
    if (!d3t_SnowMip32[8].lpSurface)
        return;
    surf_Blit(&d3t_SnowMip32[8], pal, &snow_ddsd, pal);
    for (i = 0; i < 8; i++) {
        size = 1 << i;
        d3t_SnowMip32[i].dwSize = sizeof(DDSURFACEDESC2);
        d3t_SnowMip32[i].dwHeight = size;
        d3t_SnowMip32[i].dwWidth = size;
        d3t_SnowMip32[i].ddpfPixelFormat = d3t_SnowMip32[8].ddpfPixelFormat;
        d3t_SnowMip32[i].lPitch = size * 4;
        d3t_SnowMip32[i].lpSurface = mem_Alloc(size * d3t_SnowMip32[i].lPitch, "d3t:SnowMipMap32");
        if (!d3t_SnowMip32[i].lpSurface)
            return;
        surf_ScaleBlit(&d3t_SnowMip32[i], &d3t_SnowMip32[8]);
    }

    if (snow_tex) {
#line 3894
        D3DCHECK(snow_tex->surf->lpVtbl->Unlock(snow_tex->surf, NULL));
        tex_Release(snow_tex);
    } else {
        mem_Free(snow_ddsd.lpSurface);
        snow_ddsd.lpSurface = 0;
    }

    /* Dither patterns: random noise, blurred four times with the 4-neighbourhood, stretched to 0..255 */
    tmp = mem_Alloc(256 * 256 * sizeof(float), "d3t:SnowDitherTemp");
    if (!tmp)
        return;
    for (i = 0; i < 9; i++) {
        size = 1 << i;
        n = size * size;
        for (j = 0; j < n; j++)
            tmp[j] = (float)(rand() & 0xff);
        for (k = 0; k < 4; k++) {
            for (y = 0; y < size; y++) {
                up = &tmp[(y - 1) * size];
                row = &tmp[y * size];
                down = &tmp[(y + 1) * size];
                for (x = 0; x < 1 << i; x++) {
                    s = row[x];
                    cnt = 1;
                    if (x > 0) {
                        s += row[x - 1];
                        cnt++;
                    }
                    if (x < size - 1) {
                        s += row[x + 1];
                        cnt++;
                    }
                    if (y > 0) {
                        s += up[x];
                        cnt++;
                    }
                    if (y < size - 1) {
                        s += down[x];
                        cnt++;
                    }
                    row[x] = s / cnt;
                }
            }
        }
        lo = 1e35f;
        hi = -1e35f;
        for (j = 0; j < n; j++) {
            if (tmp[j] < lo)
                lo = tmp[j];
            if (tmp[j] > hi)
                hi = tmp[j];
        }
        ofs = -lo;
        scale = 255.0f / (hi - lo);
        for (j = 0; j < n; j++) {
            t = (tmp[j] + ofs) * scale;
            tmp[j] = min(max(0.0f, t), 255.0f);
        }
        d3t_SnowDither[i].dwSize = sizeof(DDSURFACEDESC2);
        d3t_SnowDither[i].dwHeight = size;
        d3t_SnowDither[i].dwWidth = size;
        d3t_SnowDither[i].ddpfPixelFormat.dwFlags = DDPF_PALETTEINDEXED8;
        d3t_SnowDither[i].ddpfPixelFormat.dwRGBBitCount = 8;
        d3t_SnowDither[i].lPitch = size;
        d3t_SnowDither[i].lpSurface = mem_Alloc(size * size, "d3t:SnowDither");
        if (!d3t_SnowDither[i].lpSurface)
            return;
        for (j = 0; j < n; j++)
            ((unsigned char *)d3t_SnowDither[i].lpSurface)[j] = (unsigned char)tmp[j];
    }
    mem_Free(tmp);
}
