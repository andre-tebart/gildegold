/* 0x420140 */

void surf_ScaleBlit(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src)
{
    int bpp;
    unsigned int pal8, y;
    int n;

    bpp = src->ddpfPixelFormat.dwRGBBitCount;
    if (bpp == dst->ddpfPixelFormat.dwRGBBitCount
        && ((pal8 = src->ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8) || bpp == 16 || bpp == 24 || bpp == 32)) {
        if (dst->dwWidth < src->dwWidth) {
            if (pal8) {
                surf_Scale8(dst, src);
                return;
            }
            if (bpp == 16) {
                surf_Shrink16(dst, src);
                return;
            }
            if (bpp == 24) {
                surf_Shrink24(dst, src);
                return;
            }
            if (bpp == 32) {
                surf_Down32(dst, src);
                return;
            }
        } else if (dst->dwWidth == src->dwWidth) {
            if (pal8)
                n = src->dwWidth;
            else if (bpp == 16)
                n = src->dwWidth * 2;
            else if (bpp == 24)
                n = src->dwWidth * 3;
            else if (bpp == 32)
                n = src->dwWidth * 4;
            for (y = 0; y < dst->dwWidth; y++)
                SURF_COPY((char *)dst->lpSurface + dst->lPitch * y, (char *)src->lpSurface + src->lPitch * y, n);
        } else {
            if (pal8) {
                surf_Up8(dst, src);
                return;
            }
            if (bpp == 16) {
                surf_Up16(dst, src);
                return;
            }
            if (bpp == 24) {
                surf_Up24(dst, src);
                return;
            }
            if (bpp == 32)
                surf_Up32(dst, src);
        }
    }
}
