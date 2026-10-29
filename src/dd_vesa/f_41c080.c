/* 0x41c080, condenses a device description into the flags and limits at info->unk30c.. */

void d3d_GetDeviceInfo(D3DDEVICEDESC *caps, D3DDEVINFO *info)
{
    int sw = info->software;
    int tex;

    D3_ZERO(&info->caps + 1, 0x2c);
    info->software = sw;
    if ((caps->dwFlags & 0x300) && (caps->dwFlags & D3DDD_TRICAPS) && (caps->dpcTriCaps.dwTextureBlendCaps & 0x82))
        tex = 1;
    else
        tex = 0;
    info->unk30c_3 = (caps->dwFlags & 1) && caps->bClipping == 1 ? 1 : 0;
    if (caps->dwFlags & D3DDD_TRICAPS) {
        info->unk30c_5 = ~(caps->dpcTriCaps.dwRasterCaps >> 15);
        if (info->unk30c_5) {
            info->zmode = 1;
            info->unk30d_4 = caps->dpcTriCaps.dwRasterCaps >> 18;
        } else
            info->zmode = 0;
        info->unk30c_7 = caps->dpcTriCaps.dwRasterCaps;
        info->unk30c_6 = caps->dpcTriCaps.dwRasterCaps >> 11;
        /* written as two tests: VC6 merges them into one `test dh,0x50` but keeps the branchy 0/1 result
           (a single `& 0x5000` compiles to setne and shifts the whole register allocation) */
        info->unk30c_0 = (caps->dpcTriCaps.dwShadeCaps & D3DPSHADECAPS_ALPHAGOURAUDBLEND)
                      || (caps->dpcTriCaps.dwShadeCaps & D3DPSHADECAPS_ALPHAFLATBLEND) ? 1 : 0;
        if (!info->unk30c_0)
            info->unk30c_1 = (caps->dpcTriCaps.dwShadeCaps & 0x2000) || (caps->dpcTriCaps.dwRasterCaps & 0x200) ? 1 : 0;
        info->unk30d_3 = !info->unk30c_0 && info->unk30c_1 ? 1 : 0;
        info->unk30d_5 = caps->dpcTriCaps.dwRasterCaps >> 7;
    }
    if (tex) {
        info->unk30c_2 = caps->dpcTriCaps.dwTextureCaps;
        info->unk30c_4 = caps->dpcTriCaps.dwTextureFilterCaps >> 1;
        info->maxMipFilter = 1;
        if (info->unk30c_4) {
            if (caps->dpcTriCaps.dwTextureFilterCaps & 0x20)
                info->maxMipFilter = 3;
            else if (caps->dpcTriCaps.dwTextureFilterCaps & 8)
                info->maxMipFilter = 2;
        } else {
            if (caps->dpcTriCaps.dwTextureFilterCaps & 0x10)
                info->maxMipFilter = 3;
            else if (caps->dpcTriCaps.dwTextureFilterCaps & 4)
                info->maxMipFilter = 2;
        }
        info->unk30d_0 = caps->dpcTriCaps.dwRasterCaps >> 5;
        info->unk30d_1 = caps->wMaxSimultaneousTextures >= 2 && (caps->dwTextureOpCaps & 8) && !info->software ? 1 : 0;
        info->minw = caps->dwMinTextureWidth;
        info->minh = caps->dwMinTextureHeight;
        info->maxw = caps->dwMaxTextureWidth;
        if (!info->maxw)
            info->maxw = 0x400;
        info->maxh = caps->dwMaxTextureHeight;
        if (!info->maxh)
            info->maxh = 0x400;
        if ((caps->dpcTriCaps.dwDestBlendCaps & 0x20) && (caps->dpcTriCaps.dwSrcBlendCaps & 0x10)) {
            info->trans_destblend = 6;
            info->trans_srcblend = 5;
        } else {
            if (caps->dpcTriCaps.dwDestBlendCaps & 0x20)
                info->trans_destblend = 6;
            else if ((caps->dpcTriCaps.dwDestBlendCaps & 2) && (caps->dpcTriCaps.dwSrcBlendCaps & 2))
                info->trans_destblend = 2;
            else if (caps->dpcTriCaps.dwDestBlendCaps & 4)
                info->trans_destblend = 3;
            else if (caps->dpcTriCaps.dwDestBlendCaps & 0x100)
                info->trans_destblend = 9;
            else {
                info->trans_destblend = 1;
                info->unk30c_0 = 0;
            }
            info->trans_srcblend = 2;
        }
        if ((caps->dpcTriCaps.dwDestBlendCaps & 2) && (caps->dpcTriCaps.dwSrcBlendCaps & 2)) {
            info->glow_destblend = 2;
            info->glow_srcblend = 2;
        } else {
            info->glow_destblend = info->trans_destblend;
            info->glow_srcblend = info->trans_srcblend;
        }
    }
}
