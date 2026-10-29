/* 0x41ebc0, 8-bit nearest-neighbour scaling of src into dst */
void surf_Scale8(LPDDSURFACEDESC2 dst, LPDDSURFACEDESC2 src)
{
    DWORD x, y;
    unsigned char *d, *s;

    for (y = 0; y < dst->dwHeight; y++) {
        d = (unsigned char *)dst->lpSurface + dst->lPitch * y;
        s = (unsigned char *)src->lpSurface + src->lPitch * (src->dwHeight * y / dst->dwHeight);
        for (x = 0; x < dst->dwWidth; x++)
            d[x] = s[src->dwWidth * x / dst->dwWidth];
    }
}
