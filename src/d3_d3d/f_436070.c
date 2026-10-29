/* 0x436070, picks the z buffer format closest to the wanted depth. */

HRESULT CALLBACK d3d_EnumZBufferFormatsCallback(LPDDPIXELFORMAT pf, LPVOID lpctx)
{
    D3DZBUFCTX *ctx = lpctx;
    DWORD bits = pf->dwZBufferBitDepth;

    if (pf->dwFlags == DDPF_ZBUFFER
        && (!ctx->pf.dwZBufferBitDepth
            || bits >= ctx->bpp && bits - ctx->bpp <= ctx->pf.dwZBufferBitDepth - ctx->bpp))
        D3_COPY(&ctx->pf, pf, sizeof(DDPIXELFORMAT));
    return DDENUMRET_OK;
}
