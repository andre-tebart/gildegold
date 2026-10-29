/* 0x5d7160, the screenshot into the small screen image ai_ScreenImage (690 B): locks the screen
   surface (d2_ScreenSurf), samples its pixels (+0x1c, pitch +0x10) into a 0x25800 byte 16-bit buffer
   (mem_Alloc "!f3:ScreenShot") at the scale 0.003125 per source pixel - the original keeps one float
   step, width * 0.003125, and advances it over the 0x140 columns and the 0xf0 rows with an x / y
   accumulator (the row and column are the float -> int converts 0x5de518). Then surf_ScaleBlit
   0x420140 shrinks the buffer into the 0x78 x 0xa0 image ai_ScreenImage 0x766320 (pitch 0x140; the
   source desc carries the display masks dd_RMask / dd_GMask / dd_BMask, both descs are 16 bpp) and the
   buffer is freed. Void; when the surface does not lock it returns early. */

void ai_Screenshot(void)
{
    DDSURFACEDESC2 src, dst;
    unsigned short *buf;
    float step, xf;
    int i = 0, x, y;
    float yf = 0.0f;

    step = (float)d2_ScreenW * 0.003125f;
    if (d2_LockSurface(d2_ScreenSurf) == 0)
        return;
    buf = mem_Alloc(0x25800, "!f3:ScreenShot");
    for (y = 0xf0; y != 0; y--) {
        int row = (int)yf * d2_ScreenSurf->pitch;
        xf = 0.0f;
        for (x = 0x140; x != 0; x--) {
            int col = (int)xf;
            xf += step;
            buf[i++] = d2_ScreenSurf->bits[col + row];
        }
        yf += step;
    }
    d2_UnlockSurface(d2_ScreenSurf);
    D3_ZERO(&src, sizeof(src));
    src.dwSize = sizeof(src);
    src.dwHeight = 0xf0;
    src.dwWidth = 0x140;
    src.lPitch = 0x280;                 /* 0x140 pixels of 16 bits */
    src.lpSurface = buf;
    src.ddpfPixelFormat.dwRGBBitCount = 0x10;
    src.ddpfPixelFormat.dwRBitMask = dd_RMask;
    src.ddpfPixelFormat.dwGBitMask = dd_GMask;
    src.ddpfPixelFormat.dwBBitMask = dd_BMask;
    D3_ZERO(&dst, sizeof(dst));
    dst.dwSize = sizeof(dst);
    dst.dwHeight = 0x78;
    dst.dwWidth = 0xa0;
    dst.lPitch = 0x140;                 /* 0xa0 pixels of 16 bits */
    dst.lpSurface = ai_ScreenImage;
    dst.ddpfPixelFormat.dwRGBBitCount = 0x10;
    surf_ScaleBlit(&dst, &src);
    mem_Free(buf);
}
