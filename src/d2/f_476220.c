/* 0x476220, plots a point (pen size 0, 1 or 2) on the paintbox of the current window */

void d2_Plot(int x, int y, int r, int g, int b)
{
    D2PAINTWIN *w = WIN(d2_LastWindow);
    int pen;

    if (w->active == 0)
        return;
    if (w->paint == 0) {
        err_Message("d2_Plot:Window is not a paintbox!");
        return;
    }
    pen = d2_PenSize & 0xff;
    if (x < pen || x > w->w - pen || y < pen || y > w->h - pen)
        return;
    d2_LockSurface(w->paint);
    switch (d2_PenSize & 0xff) {
    case 0:
        d2_PutPixelRGB(x, y, r, g, b, w->paint);
        break;
    case 1:
        d2_PutPixelRGB(x - 1, y, r, g, b, w->paint);
        d2_PutPixelRGB(x + 1, y, r, g, b, w->paint);
        d2_PutPixelRGB(x, y + 1, r, g, b, w->paint);
        d2_PutPixelRGB(x, y - 1, r, g, b, w->paint);
        d2_PutPixelRGB(x, y, r, g, b, w->paint);
        break;
    case 2:
        d2_PutPixelRGB(x - 1, y, r, g, b, w->paint);
        d2_PutPixelRGB(x + 1, y, r, g, b, w->paint);
        d2_PutPixelRGB(x - 2, y, r, g, b, w->paint);
        d2_PutPixelRGB(x + 2, y, r, g, b, w->paint);
        d2_PutPixelRGB(x, y, r, g, b, w->paint);
        d2_PutPixelRGB(x, y + 1, r, g, b, w->paint);
        d2_PutPixelRGB(x, y - 1, r, g, b, w->paint);
        d2_PutPixelRGB(x, y + 2, r, g, b, w->paint);
        d2_PutPixelRGB(x, y - 2, r, g, b, w->paint);
        d2_PutPixelRGB(x + 1, y + 1, r, g, b, w->paint);
        d2_PutPixelRGB(x - 1, y - 1, r, g, b, w->paint);
        d2_PutPixelRGB(x - 1, y + 1, r, g, b, w->paint);
        d2_PutPixelRGB(x + 1, y - 1, r, g, b, w->paint);
        break;
    }
    d2_UnlockSurface(w->paint);
}
