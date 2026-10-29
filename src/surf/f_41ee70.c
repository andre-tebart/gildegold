/* 0x41ee70, 24-bit box filter shrinking src into dst */

void surf_Shrink24(LPDDSURFACEDESC2 dst, LPDDSURFACEDESC2 src)
{
    DWORD x, y, x0, x1, y0, y1, i, j, n, c0, c1, c2, w;
    unsigned char *d, *s;

    y1 = 0;
    for (y = 0; y < dst->dwHeight; y++) {
        d = (unsigned char *)dst->lpSurface + dst->lPitch * y;
        y0 = y1;
        y1 = src->dwHeight * (y + 1) / dst->dwHeight;
        x1 = 0;
        for (x = 0; x < dst->dwWidth; x++) {
            x0 = x1;
            x1 = src->dwWidth * (x + 1) / dst->dwWidth;
            w = x1 - x0;
            n = c2 = c1 = c0 = 0;
            for (j = 0; j < y1 - y0; j++) {
                s = (unsigned char *)src->lpSurface + src->lPitch * (y0 + j);
                for (i = 0; i < w; i++) {
                    c0 += s[(x0 + i) * 3];
                    c1 += s[(x0 + i) * 3 + 1];
                    c2 += s[(x0 + i) * 3 + 2];
                    n++;
                }
            }
            d[x * 3] = (unsigned char)(c0 / n);
            d[x * 3 + 1] = (unsigned char)(c1 / n);
            d[x * 3 + 2] = (unsigned char)(c2 / n);
        }
    }
}
