/* 0x41e390, shows the finished frame */

void dd_Flip(void)
{
    DWORD flags = DDFLIP_WAIT;
    void *p;
    RECT rc;
    POINT pt;
    RECT src;
    HRESULT hr;
    HDC dc, mem;
    HGDIOBJ old;

#ifdef REA_FRAME_DUMP
    /* debug build (dd_vesa.h): while dump.trigger holds a count > 0, dump the finished frame and count it down */
    {
        FILE *rd_t;
        int rd_n;
        REA_DUMP_TRIGGER(rd_n);
        if (rd_n > 0) {
            REA_DUMP_BACKBUFFER("final");
            rd_t = fopen("dump.trigger", "w");
            if (rd_t) {
                fprintf(rd_t, "%d\n", rd_n - 1);
                fclose(rd_t);
            }
        }
    }
#endif
    if (!dd_TripleBuffer)
        flags = DDFLIP_WAIT | DDFLIP_NOVSYNC;
    dd_Cleared = 0;
    dd_ZCleared = 0;
    switch (dd_Mode) {
    case DD_FULLMEM:
        p = dd_LockSurface(dd_BackBuffer);
        if (p) {
            DD_COPY(p, dd_MemBuff, dd_MemSize);
            dd_UnlockSurface(dd_BackBuffer);
            while (1) {
                hr = dd_FrontBuffer->lpVtbl->Flip(dd_FrontBuffer, NULL, flags);
                if (hr == DD_OK || hr != DDERR_SURFACELOST)
                    break;
                if (!dd_Restore())
                    return;
            }
        }
        break;
    case DD_WINDOWMEM:
        p = dd_LockSurface(dd_BackBuffer);
        if (p) {
            DD_COPY(p, dd_MemBuff, dd_MemSize);
            dd_UnlockSurface(dd_BackBuffer);
            GetClientRect(dd_Window, &rc);
            pt.y = 0;
            pt.x = 0;
            ClientToScreen(dd_Window, &pt);
            OffsetRect(&rc, pt.x, pt.y);
            while (1) {
                hr = dd_FrontBuffer->lpVtbl->Blt(dd_FrontBuffer, &rc, dd_BackBuffer, NULL, DDBLT_WAIT, NULL);
                if (hr == DD_OK)
                    return;
                if (hr != DDERR_SURFACELOST)
                    return;
                if (!dd_Restore())
                    return;
            }
        }
    case DD_FULLSCREEN:
        while (1) {
            hr = dd_FrontBuffer->lpVtbl->Flip(dd_FrontBuffer, NULL, flags);
            if (hr == DD_OK) {
                dd_Page = !dd_Page;
                return;
            }
            if (hr != DDERR_SURFACELOST)
                return;
            if (!dd_Restore())
                return;
        }
    case DD_WINDOW:
        GetClientRect(dd_Window, &rc);
        pt.y = 0;
        pt.x = 0;
        ClientToScreen(dd_Window, &pt);
        OffsetRect(&rc, pt.x, pt.y);
        src.bottom = dd_Height;
        src.right = dd_Width;
        src.top = 0;
        src.left = 0;
        dd_FrontBuffer->lpVtbl->Blt(dd_FrontBuffer, &rc, dd_BackBuffer, &src, DDBLT_WAIT | DDBLT_ASYNC, NULL);
        break;
    case DD_GDI:
        dc = GetDC(dd_Window);
        mem = CreateCompatibleDC(dc);
        old = SelectObject(mem, dd_Bitmap);
        BitBlt(dc, 0, 0, dd_Width, dd_Height, mem, 0, 0, SRCCOPY);
        ReleaseDC(dd_Window, dc);
        SelectObject(mem, old);
        DeleteDC(mem);
        break;
    }
}
