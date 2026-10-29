/* 0x41f220 */

void surf_Up8(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src)
{
    unsigned int y, x;
    unsigned char *s, *d;

    for (y = 0; y < src->dwHeight; y++) {
        s = (unsigned char *)src->lpSurface + src->lPitch * y;
        d = (unsigned char *)dst->lpSurface + dst->dwHeight * y / src->dwHeight * dst->lPitch;
        for (x = 0; x < src->dwWidth; x++)
            d[dst->dwWidth * x / src->dwWidth] = s[x];
    }
}
