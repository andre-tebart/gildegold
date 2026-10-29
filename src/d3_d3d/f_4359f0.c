/* 0x4359f0, picks the texture formats closest to the wanted bit depths. */
HRESULT CALLBACK d3d_EnumTextureFormatsCallback(LPDDPIXELFORMAT pf, LPVOID lpctx)
{
    D3DTEXFMTCTX *ctx = lpctx;
    DWORD bpp = pf->dwRGBBitCount;
    char r = 0, g = 0, b = 0;
    DWORD m;
    int i;

    if (!ctx->found && (pf->dwFlags & DDPF_FOURCC) && ctx->fourcc && pf->dwFourCC == ctx->fourcc) {
        ctx->found = 1;
        ctx->bpp = pf->dwRGBBitCount;
        D3_COPY(&ctx->pf, pf, sizeof(DDPIXELFORMAT));
    }
    if (!(pf->dwFlags & (DDPF_ALPHAPIXELS | DDPF_ALPHA)) && bpp >= 8) {
        if (bpp == 8) {
            if (!(pf->dwFlags & DDPF_PALETTEINDEXED8))
                return DDENUMRET_OK;
        } else if (bpp > 8 && !(pf->dwFlags & DDPF_RGB))
            return DDENUMRET_OK;
        if (!ctx->found
            && (!ctx->pf.dwRGBBitCount
                || bpp >= ctx->bpp && abs(bpp - ctx->bpp) < abs(ctx->pf.dwRGBBitCount - ctx->bpp)
                || bpp == ctx->pf.dwRGBBitCount
                   && pf->dwRBitMask + pf->dwGBitMask + pf->dwBBitMask
                      > ctx->pf.dwRBitMask + ctx->pf.dwGBitMask + ctx->pf.dwBBitMask))
            D3_COPY(&ctx->pf, pf, sizeof(DDPIXELFORMAT));
        for (i = 0, m = pf->dwRBitMask; i < 32; i++, m >>= 1)
            if (m & 1)
                r++;
        for (i = 0, m = pf->dwGBitMask; i < 32; i++, m >>= 1)
            if (m & 1)
                g++;
        for (i = 0, m = pf->dwBBitMask; i < 32; i++, m >>= 1)
            if (m & 1)
                b++;
        if (!ctx->pf2.dwRGBBitCount
            || bpp >= ctx->bpp2 && abs(bpp - ctx->bpp2) < abs(ctx->pf2.dwRGBBitCount - ctx->bpp2)
            || bpp == ctx->pf2.dwRGBBitCount
               && pf->dwRBitMask + pf->dwGBitMask + pf->dwBBitMask
                  > ctx->pf2.dwRBitMask + ctx->pf2.dwGBitMask + ctx->pf2.dwBBitMask
               && !d3d_UsedTexFormat.exact) {
            if (r == dd_RBits && g == dd_GBits && b == dd_BBits)
                d3d_UsedTexFormat.exact = 1;
            D3_COPY(&ctx->pf2, pf, sizeof(DDPIXELFORMAT));
        }
    }
    return DDENUMRET_OK;
}
