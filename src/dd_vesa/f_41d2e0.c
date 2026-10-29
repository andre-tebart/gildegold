/* 0x41d2e0, sets up the display: mode DD_GDI..DD_FULLMEM, xres x yres at bpp bits @deep B3: 84 code hunks left, all from the constant-8 / constant-0 scratch web (instr 328: original cmp [ebp+0x10],0x9 / jl vs ours mov eax,8 / cmp / jle - the `>= 9` spelling chosen after the recorded `> 8` experiment matches 327 instrs byte for byte but swaps eax/ecx from `if (biHeight > 0)` on); the fix is IR order / temp count, not a meaning error - see notes/units/dd_vesa.md */

/* CPUID feature bit 23 (MMX), 0 if the CPU has no CPUID */
__inline char dd_CpuHasMMX(void)
{
    __asm {
        pushfd
        pop ecx
        mov eax, ecx
        xor eax, 0x200000
        push eax
        popfd
        pushfd
        pop eax
        xor eax, ecx
        test eax, 0x200000
        mov eax, 0
        je nocpuid
        inc eax
        cpuid
        test edx, 0x800000
        setne al
    nocpuid:
    }
}

char dd_SetMode(int xres, int yres, int bpp, int mode, HWND wnd, int unk6, char triple)
{
    DDSURFACEDESC2 ddsd;
    BITMAPINFO bmi;
    DDPIXELFORMAT ddpf;
    DDSCAPS2 ddscaps;
    HDC dc;
    DWORD r, g, b;
    unsigned char rb, gb, bb;
    int i;

    if (!dd_DD)
        return 0;
    dd_Mode = mode;
    dd_Surface = NULL;
    dd_Unk1464fc0 = 0;
    dd_MemBuff = NULL;
    dd_MemBottom = NULL;
    dd_Width = xres;
    dd_Height = yres;
    dd_RShift = 0;
    dd_GShift = 0;
    dd_BShift = 0;
    dd_BitPlanes = bpp;
    dd_Window = wnd;
    for (i = bpp; i & 7; i++)
        ;
    dd_Cleared = 0;
    dd_ZCleared = 0;
    dd_BytesPerPixel = i / 8;
    dd_MemPitch = dd_BytesPerPixel * xres;
    dd_MemSize = dd_MemPitch * yres;
    dd_HasMMX = dd_CpuHasMMX();
    dd_Has800x600 = 0;
    dd_Has1024x768 = 0;
    dd_Has1152x864 = 0;
    if (!dd_HasMMX)
        return 0;
    d3_InitKeyCodes(dd_Unk1464fe0);
    dd_TripleBuffer = 0;
    if (dd_Mode == DD_FULLMEM || dd_Mode == DD_WINDOWMEM) {
        dd_MemBuff = mem_Alloc(dd_MemSize, "dd:membuff");
        DD_ZERO(dd_MemBuff, dd_MemSize);
    }
    switch (dd_Mode) {
    case DD_FULLSCREEN:
    case DD_FULLMEM:
#line 1283
        D3DCHECK(dd_DD->lpVtbl->EnumDisplayModes(dd_DD, DDEDM_STANDARDVGAMODES, NULL, (LPVOID)bpp, dd_EnumVideoModesCallback));
        D3DCHECK(dd_DD->lpVtbl->SetCooperativeLevel(dd_DD, dd_Window, DDSCL_ALLOWREBOOT | DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_FPUSETUP));
        if (d3d_hr) {
            dd_Exit();
            return 0;
        }
#line 1291
        D3DCHECK(dd_DD->lpVtbl->SetDisplayMode(dd_DD, xres, yres, bpp, 0, DDSDM_STANDARDVGAMODE));
        if (d3d_hr) {
            dd_Exit();
            return 0;
        }
        D3_ZERO(&ddsd, sizeof(ddsd));
        ddsd.dwSize = sizeof(ddsd);
        ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
        ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX | DDSCAPS_3DDEVICE;
        if (triple) {
            ddsd.dwBackBufferCount = 2;
            if (d3d_hr = dd_DD->lpVtbl->CreateSurface(dd_DD, &ddsd, &dd_FrontBuffer, NULL)) {
                ddsd.dwBackBufferCount = 1;
#line 1308
                D3DCHECK(dd_DD->lpVtbl->CreateSurface(dd_DD, &ddsd, &dd_FrontBuffer, NULL));
                if (d3d_hr) {
                    dd_Exit();
                    return 0;
                }
            } else
                dd_TripleBuffer = 1;
        } else {
            ddsd.dwBackBufferCount = 1;
#line 1321
            D3DCHECK(dd_DD->lpVtbl->CreateSurface(dd_DD, &ddsd, &dd_FrontBuffer, NULL));
            if (d3d_hr) {
                dd_Exit();
                return 0;
            }
        }
        dd_SetSurfaceName(dd_FrontBuffer, "dd_FrontBuffer", 0);
        ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
#line 1331
        D3DCHECK(dd_FrontBuffer->lpVtbl->GetAttachedSurface(dd_FrontBuffer, &ddscaps, &dd_BackBuffer));
        if (d3d_hr) {
            dd_Exit();
            return 0;
        }
        if (!dd_Restore()) {
            dd_Exit();
            err_Message("Could not restore dd_FrontBuffer!");
            return 0;
        }
        dd_SetSurfaceName(dd_BackBuffer, "dd_BackBuffer", 0);
        break;
    case DD_WINDOW:
    case DD_WINDOWMEM:
#line 1348
        D3DCHECK(dd_DD->lpVtbl->EnumDisplayModes(dd_DD, DDEDM_STANDARDVGAMODES, NULL, NULL, dd_EnumVideoModesCallback));
        D3DCHECK(dd_DD->lpVtbl->SetCooperativeLevel(dd_DD, dd_Window, DDSCL_NORMAL | DDSCL_NOWINDOWCHANGES | DDSCL_FPUSETUP));
        if (d3d_hr) {
            dd_Exit();
            return 0;
        }
        D3_ZERO(&ddsd, sizeof(ddsd));
        ddsd.dwSize = sizeof(ddsd);
        ddsd.dwFlags = DDSD_CAPS;
        ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
#line 1360
        D3DCHECK(dd_DD->lpVtbl->CreateSurface(dd_DD, &ddsd, &dd_FrontBuffer, NULL));
        if (d3d_hr) {
            dd_Exit();
            return 0;
        }
        dd_SetSurfaceName(dd_FrontBuffer, "dd_FrontBuffer", 0);
#line 1367
        D3DCHECK(dd_DD->lpVtbl->CreateClipper(dd_DD, 0, &lpClipper, NULL));
        if (d3d_hr) {
            dd_Exit();
            return 0;
        }
#line 1373
        D3DCHECK(lpClipper->lpVtbl->SetHWnd(lpClipper, 0, dd_Window));
        D3DCHECK(dd_FrontBuffer->lpVtbl->SetClipper(dd_FrontBuffer, lpClipper));
        D3_ZERO(&ddsd, sizeof(ddsd));
        ddsd.dwSize = sizeof(ddsd);
        ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
        ddsd.dwWidth = xres;
#ifdef REA_FIX_16BPP
        /* REA_FIX_16BPP=1 (tools/build.py): request a 16 bit back buffer, the format the 2D
           blitters draw, instead of the desktop's 32 bit format that an offscreen surface without
           DDSD_PIXELFORMAT picks up in windowed mode (the fullscreen path gets 16 bit from SetDisplayMode) */
        ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
        ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
        ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB;
        ddsd.ddpfPixelFormat.dwRGBBitCount = 16;
        ddsd.ddpfPixelFormat.dwRBitMask = 0xf800;
        ddsd.ddpfPixelFormat.dwGBitMask = 0x07e0;
        ddsd.ddpfPixelFormat.dwBBitMask = 0x001f;
#else
        ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
#endif
        ddsd.dwHeight = yres;
#line 1392
        D3DCHECK(dd_DD->lpVtbl->CreateSurface(dd_DD, &ddsd, &dd_BackBuffer, NULL));
        if (d3d_hr) {
            dd_Exit();
            return 0;
        }
        if (!dd_Restore()) {
            dd_Exit();
            err_Message("Could not restore dd_FrontBuffer(window)!");
            return 0;
        }
        dd_SetSurfaceName(dd_BackBuffer, "dd_BackBuffer", 0);
        break;
    case DD_GDI:
        dc = GetDC(dd_Window);
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = xres;
        bmi.bmiHeader.biHeight = yres;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = bpp;
        bmi.bmiHeader.biCompression = BI_RGB;
        bmi.bmiHeader.biXPelsPerMeter = 0;
        bmi.bmiHeader.biYPelsPerMeter = 0;
        bmi.bmiHeader.biClrUsed = 0;
        bmi.bmiHeader.biClrImportant = 0;
        dd_Bitmap = CreateDIBSection(dc, &bmi, DIB_RGB_COLORS, &dd_MemBuff, NULL, 0);
        if (!dd_Bitmap)
            return 0;
        ReleaseDC(dd_Window, dc);
        D3_ZERO_AT(dd_MemBuff, bpp / 8 * xres * yres);
        if (bmi.bmiHeader.biHeight > 0)
            dd_MemBottom = (unsigned char *)dd_MemBuff + (yres - 1) * dd_MemPitch;
        else
            dd_MemBottom = NULL;
        if (bpp != 16) {
            if (bpp == 24 || bpp == 32) {
                dd_RShift = 16;
                dd_GShift = 8;
                dd_BShift = 0;
                dd_RLoss = 8;
                dd_GLoss = 8;
                dd_BLoss = 8;
                dd_BBits = 8;
                dd_GBits = 8;
                dd_RBits = 8;
                dd_RMask = ((unsigned)&txt_Keys + 0x81c0);
                dd_GMask = 0xff00;
                dd_BMask = 0xff;
            }
        } else {
            dd_BShift = 0;
            dd_RShift = 10;
            dd_GShift = 5;
            dd_RLoss = 3;
            dd_GLoss = 3;
            dd_BLoss = 3;
            dd_BBits = 5;
            dd_GBits = 5;
            dd_RBits = 5;
            dd_RMask = 0x7c00;
            dd_GMask = 0x3e0;
            dd_BMask = 0x1f;
        }
        break;
    }
    /* >= 9, not > 8: the immediate keeps VC6's constant-8 web out of this compare and matches
       everything above it (56 bytes / 35 asm lines closer); the compare itself still differs. */
    if (bpp >= 9 && dd_Mode != DD_GDI) {
        D3_ZERO(&ddpf, sizeof(ddpf));
        ddpf.dwSize = sizeof(ddpf);
#line 1463
        D3DCHECK(dd_FrontBuffer->lpVtbl->GetPixelFormat(dd_BackBuffer, &ddpf));
        dd_RMask = r = ddpf.dwRBitMask;
        dd_GMask = g = ddpf.dwGBitMask;
        dd_BMask = b = ddpf.dwBBitMask;
        for (i = 0; i < bpp; i++) {
            if ((r & 1) != 1) {
                r >>= 1;
                dd_RShift++;
            }
            if ((g & 1) != 1) {
                g >>= 1;
                dd_GShift++;
            }
            if ((b & 1) != 1) {
                b >>= 1;
                dd_BShift++;
            }
        }
        rb = 0;
        gb = 0;
        bb = 0;
        for (i = 0; i < bpp; i++) {
            if ((r & 1) == 1) {
                r >>= 1;
                rb++;
            }
            if ((g & 1) == 1) {
                g >>= 1;
                gb++;
            }
            if ((b & 1) == 1) {
                b >>= 1;
                bb++;
            }
        }
        dd_RBits = rb;
        dd_RLoss = 8 - rb;
        dd_GBits = gb;
        dd_GLoss = 8 - gb;
        dd_BBits = bb;
        dd_BLoss = 8 - bb;
    }
    dd_PixelFormat = 888 - dd_RLoss * 100 - dd_GLoss * 10 - dd_BLoss;
    dd_Pitch = 0;
    dd_PitchPixels = 0;
    dd_Unk14635a8 = ((1 << (7 - dd_BLoss)) - 1 << dd_BShift) + ((1 << (7 - dd_GLoss)) - 1 << dd_GShift)
                  + ((1 << (7 - dd_RLoss)) - 1 << dd_RShift);
    dd_Unk1464fb8 = dd_Unk14635a8 << 16 | dd_Unk14635a8;
    dd_Unk14635a8 |= 0xffff0000;
    dd_Clear(NULL, 0, 0, 0);
    dd_SetClipRect(0, 0, xres - 1, yres - 1);
    return 1;
}
