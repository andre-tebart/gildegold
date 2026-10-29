/* 0x41e100, clears rect (NULL = all) of the drawing surface, once per frame */

void dd_Clear(RECT *rect, int r, int g, int b)
{
    unsigned char *p;
    int w;

    if (dd_Cleared)
        return;
    switch (dd_Mode) {
    case DD_WINDOWMEM:
    case DD_FULLMEM:
        if (!dd_MemBuff)
            return;
        if (!rect) {
            DD_ZERO(dd_MemBuff, dd_MemSize);
            break;
        }
        p = (unsigned char *)dd_MemBuff + rect->left * dd_BytesPerPixel + rect->top * dd_MemPitch;
        w = (rect->right - rect->left) * dd_BytesPerPixel;
        for (b = rect->top; b < rect->bottom; b++) {
            DD_ZERO(p, w);
            p += dd_MemPitch;
        }
        break;
    case DD_GDI:
        if (!dd_MemBuff)
            return;
        if (!rect) {
            DD_ZERO(dd_MemBuff, dd_MemSize);
            break;
        }
        if (!dd_MemBottom) {
            p = (unsigned char *)dd_MemBuff + rect->left * dd_BytesPerPixel + rect->top * dd_MemPitch;
            w = (rect->right - rect->left) * dd_BytesPerPixel;
            for (b = rect->top; b < rect->bottom; b++) {
                DD_ZERO(p, w);
                p += dd_MemPitch;
            }
        } else {
            p = (unsigned char *)dd_MemBottom + rect->left * dd_BytesPerPixel - rect->top * dd_MemPitch;
            w = (rect->right - rect->left) * dd_BytesPerPixel;
            for (b = rect->top; b < rect->bottom; b++) {
                DD_ZERO(p, w);
                p -= dd_MemPitch;
            }
        }
        break;
    case DD_WINDOW:
    case DD_FULLSCREEN:
        if (!dd_BackBuffer)
            return;
        dd_FillSurface(dd_BackBuffer, rect, dd_RGB(r, g, b));
        break;
    }
    dd_Cleared = 1;
}
