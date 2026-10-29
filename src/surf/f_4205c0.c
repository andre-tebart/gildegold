/* 0x4205c0, copies a same-size src surface into dst: a straight row copy when the pixel formats match
   (or both are 8-bit palette-indexed), otherwise converts 8-bit palette-indexed src through srcpal to
   16-bit dst or 24-bit src to 16-bit dst; writes dst only, returns nothing */

void surf_Blit(DDSURFACEDESC2 *dst, PALETTEENTRY *dstpal, DDSURFACEDESC2 *src, PALETTEENTRY *srcpal)
{
    unsigned int y;
    int n;

    if (src->dwWidth == dst->dwWidth && src->dwHeight == dst->dwHeight) {
        if (src->ddpfPixelFormat.dwRGBBitCount == dst->ddpfPixelFormat.dwRGBBitCount
            || ((src->ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8)
                && (dst->ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8))) {
            if (src->ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8)
                n = src->dwWidth;
            else if (src->ddpfPixelFormat.dwRGBBitCount == 16)
                n = src->dwWidth * 2;
            else if (src->ddpfPixelFormat.dwRGBBitCount == 24)
                n = src->dwWidth * 3;
            else if (src->ddpfPixelFormat.dwRGBBitCount == 32)
                n = src->dwWidth * 4;
            else
                return;
            for (y = 0; y < src->dwHeight; y++)
                SURF_COPY((char *)dst->lpSurface + dst->lPitch * y, (char *)src->lpSurface + src->lPitch * y, n);
        } else if ((src->ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8)
                   && dst->ddpfPixelFormat.dwRGBBitCount == 16) {
            surf_Pal8To16(dst, src, srcpal);
        } else if (src->ddpfPixelFormat.dwRGBBitCount == 24 && dst->ddpfPixelFormat.dwRGBBitCount == 16) {
            surf_24To16(dst, src);
        }
    }
}
