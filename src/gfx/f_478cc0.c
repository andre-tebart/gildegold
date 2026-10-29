/* 0x478cc0, creates a surface of the description desc (0x40 bytes); flags 1: depth of the screen, 2: a DirectDraw
   surface, 4: wraps an existing one, 8: system memory
   @deep B3 3 hunk areas / 54 differing instructions (393/752 bytes), all codegen; meaning read against 0x478cc0..0x478fb1 (mem_Alloc(0x40), *s=*desc, flags&4 -> external, flags&1 -> dd_BitPlanes, own-bit alloc, wrap description, flags&2 CreateSurface path with the format switch, D3DCHECK, SetColorKey, second GetSurfaceDesc, clip/pitch tail): no meaning error. (1) the ddsCaps expression in both branches: original emits movsx eax,bl; and eax,8; shl eax,8; or al,0x40 (sign-extends the low flags byte BEFORE the mask), ours folds the cast into and ebx,8 / shl ebx,8 / or ebx,0x40; (2) the else branch's OR wants 0x40 materialised in ecx (or eax,ecx), ours or ebx,0x40; (3) tail order: original stores pitch then left/top=0 after the idiv and pops edi/esi after the last desc->h store, ours hoists top=0 before the pitch store and pops early. Tried, one build each, all reverted (identical code or worse): signed char flags parameter (frame 0x108 vs 0x104, byte reload, REGRESSED d2_CopySurface), unsigned int flags (identical), char local f=(char)flags used as (f&8) (identical - VC6 propagates the cast), f=flags implicit (identical). The old d2q note recorded the same movsx and the char/signed char/unsigned char param + int local + shift cast attempts. Making VC6 keep the char conversion needs variant rounds. Readability done: Q2 unk38 -> D2SURF.external (flags&4 sets it, it skips the bit alloc and the free; the screen surface is passed flags 4). */

#line 1 "D:\\develop\\Units\\d2.v1\\GFX.C"

D2SURF *d2_MakeSurface(D2SURF *desc, int flags)
{
    LPDIRECTDRAWSURFACE4 dd_DDSOne;
    DDCOLORKEY ck;
    DDSURFACEDESC2 ddsd;
    DDSURFACEDESC2 sd;
    D2SURF *s;

    ck.dwColorSpaceLowValue = 0;
    ck.dwColorSpaceHighValue = 0;
    s = (D2SURF *)mem_Alloc(0x40, "gfx:gfx_CreateSurface:s");
    if (s == 0)
        return 0;
    *s = *desc;
    if (flags & 4)
        s->external = 1;
    if (flags & 1)
        s->bpp = dd_BitPlanes;
    if (s->DDSurface == 0) {
        if (s->external == 0) {
            if (!(flags & 2)) {
                s->rowbytes = (s->bpp >> 3) * s->w;
                s->bits = (unsigned short *)mem_Alloc(s->h * s->rowbytes, "gfx:gfx_CreateSurface:s->data");
            }
        }
    }
    if (s->external != 0 && s->DDSurface != 0) {
        sd.dwSize = 0x7c;
        s->DDSurface->lpVtbl->GetSurfaceDesc(s->DDSurface, &sd);
        s->h = sd.dwHeight;
        s->w = sd.dwWidth;
        s->bpp = (unsigned char)sd.ddpfPixelFormat.dwRGBBitCount;
        s->rowbytes = sd.lPitch;
        s->bits = 0;
    }
    if ((flags & 2) && s->external == 0) {
        memset(&ddsd, 0, sizeof(ddsd));
        ddsd.dwSize = 0x7c;
        if (flags & 1) {
            dd_BackBuffer->lpVtbl->GetSurfaceDesc(dd_BackBuffer, &ddsd);
            ddsd.dwFlags = 0x1007;
            ddsd.ddsCaps.dwCaps = ((char)flags & 8) << 8 | 0x40;
            ddsd.dwHeight = s->h;
            ddsd.dwWidth = s->w;
        } else {
            ddsd.dwFlags = 0x1007;
            ddsd.ddsCaps.dwCaps = ((char)flags & 8) << 8 | 0x40;
            ddsd.dwHeight = s->h;
            ddsd.dwWidth = s->w;
            ddsd.ddpfPixelFormat.dwSize = 0x20;
            switch (s->bpp) {
            case 8:
                ddsd.ddpfPixelFormat.dwFlags = 0x60;
                break;
            case 0xf:
                ddsd.ddpfPixelFormat.dwFlags = 0x40;
                ddsd.ddpfPixelFormat.dwRBitMask = 0x7c00;
                ddsd.ddpfPixelFormat.dwGBitMask = 0x3e0;
                ddsd.ddpfPixelFormat.dwBBitMask = 0x1f;
                ddsd.ddpfPixelFormat.dwRGBAlphaBitMask = 0;
                break;
            case 0x10:
                ddsd.ddpfPixelFormat.dwFlags = 0x40;
                ddsd.ddpfPixelFormat.dwRBitMask = 0xf800;
                ddsd.ddpfPixelFormat.dwGBitMask = 0x7e0;
                ddsd.ddpfPixelFormat.dwBBitMask = 0x1f;
                ddsd.ddpfPixelFormat.dwRGBAlphaBitMask = 0;
                break;
            }
            ddsd.ddpfPixelFormat.dwRGBBitCount = s->bpp;
        }
#line 1087
        D3DCHECK(dd_DD->lpVtbl->CreateSurface( dd_DD, &ddsd, &dd_DDSOne, NULL ));
        dd_SetSurfaceName(dd_DDSOne, "gfx_CreateSurface", 0);
        s->DDSurface = dd_DDSOne;
        dd_DDSOne->lpVtbl->SetColorKey(dd_DDSOne, 8, &ck);
        if (dd_DDSOne == 0) {
            mem_Free(s);
            return 0;
        }
        sd.dwSize = 0x7c;
        s->DDSurface->lpVtbl->GetSurfaceDesc(s->DDSurface, &sd);
        s->h = sd.dwHeight;
        s->w = sd.dwWidth;
        s->bpp = (unsigned char)sd.ddpfPixelFormat.dwRGBBitCount;
        s->rowbytes = sd.lPitch;
    }
    s->left = 0;
    s->pitch = s->rowbytes / (s->bpp >> 3);
    s->top = 0;
    s->right = desc->w;
    s->bottom = desc->h;
    return s;
}
