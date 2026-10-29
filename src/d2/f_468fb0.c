/* 0x468fb0, adds a rectangle (clipped, x made even) to the shapes of a buffer */

void d2_AddShape(int x, int y, int w, int h, D2BUF *buf)
{
    int i;

    if (buf->active && buf->srcSurf) {
        if (x > d2_ClipRight)
            x = d2_ClipRight;
        if (x & 1) {
            x--;
            w = (w + 2) & ~1;
        }
        if (h + y > d2_ClipBottom)
            h = d2_ClipBottom - y;
        if (x + w > d2_ClipRight)
            w = d2_ClipRight - x;
        if (y < d2_ClipTop)
            y = d2_ClipTop;
        if (x < d2_ClipLeft)
            x = d2_ClipLeft;
        if (w < 0)
            w = 1;
        if (h < 0)
            h = 1;
        i = 0;
        while (d2_Shapes[i].owner) {
            if (i >= 0x300)
                return;
            i++;
        }
        if (i >= 0x300)
            return;
        d2_Shapes[i].x = x;
        d2_Shapes[i].y = y;
        d2_Shapes[i].w = w;
        d2_Shapes[i].h = h;
        d2_Shapes[i].owner = buf;
    }
}
