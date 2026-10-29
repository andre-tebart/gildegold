/* 0x436560, applies the render options; the first call sets all states. */

void d3d_SetConfig(D3DCONFIG *c)
{
    int mipfilter;

    if (d3d_StatesSet) {
#line 515
        D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ZENABLE, d3d_UsedCard.info.zmode));
        if (d3d_Config.fillmode != c->fillmode)
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_FILLMODE, c->fillmode));
        if (d3d_Config.mipfilter != c->mipfilter) {
            mipfilter = d3d_UsedCard.info.maxMipFilter;
            if (c->mipfilter < mipfilter)
                mipfilter = c->mipfilter;
#line 524
            D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_MIPFILTER, mipfilter));
        }
        if (d3d_Config.perspective != c->perspective) {
            if ((d3d_UsedCard.info.caps & 4) && c->perspective) {
#line 530
                D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_TEXTUREPERSPECTIVE, TRUE));
            } else {
#line 532
                D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_TEXTUREPERSPECTIVE, FALSE));
            }
        }
        if (d3d_Config.filter != c->filter && (d3d_UsedCard.info.caps & 0x10)) {
            if (c->filter) {
#line 539
                D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_MAGFILTER, D3DTFG_LINEAR));
                D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_MINFILTER, D3DTFG_LINEAR));
            } else {
#line 544
                D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_MAGFILTER, D3DTFG_POINT));
                D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_MINFILTER, D3DTFG_POINT));
            }
        }
        if (d3d_Config.antialias != c->antialias && (d3d_UsedCard.info.caps & 0x40)) {
            if (c->antialias) {
#line 552
                D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ANTIALIAS, D3DANTIALIAS_SORTINDEPENDENT));
            } else {
#line 554
                D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ANTIALIAS, D3DANTIALIAS_NONE));
            }
        }
        if (d3d_Config.dither != c->dither && (d3d_UsedCard.info.caps & 0x80))
#line 558
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_DITHERENABLE, c->dither));
        if (d3d_Config.subpixel != c->subpixel && (d3d_UsedCard.info.flags & 1))
#line 561
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_SUBPIXEL, c->subpixel));
        D3_COPY_AT(offset d3d_Config, c, sizeof(D3DCONFIG));
    } else {
#line 568
        D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ZENABLE, d3d_UsedCard.info.zmode));
        D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ZFUNC, D3DCMP_LESSEQUAL));
        D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_CULLMODE, D3DCULL_NONE));
        D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_COLOROP, D3DTOP_MODULATE));
        D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_FILLMODE, c->fillmode));
#line 574
        D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_COLOROP, D3DTOP_MODULATE));
        D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE));
        D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE));
#line 578
        D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE));
        D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE));
        D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE));
        if ((d3d_UsedCard.info.caps & 4) && c->perspective) {
#line 585
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_TEXTUREPERSPECTIVE, TRUE));
        } else {
#line 587
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_TEXTUREPERSPECTIVE, FALSE));
        }
        mipfilter = d3d_UsedCard.info.maxMipFilter;
        if (c->mipfilter < mipfilter)
            mipfilter = c->mipfilter;
#line 592
        D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_MIPFILTER, mipfilter));
        if ((d3d_UsedCard.info.caps & 0x10) && c->filter) {
#line 596
            D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_MINFILTER, D3DTFG_LINEAR));
            D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_MAGFILTER, D3DTFG_LINEAR));
        } else {
#line 601
            D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_MINFILTER, D3DTFG_POINT));
            D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_MAGFILTER, D3DTFG_POINT));
        }
#line 605
        D3DCHECK(d3d_Device->lpVtbl->SetLightState(d3d_Device, D3DLIGHTSTATE_FOGMODE, D3DFOG_NONE));
        D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_FOGTABLEMODE, D3DFOG_NONE));
        if (c->antialias && (d3d_UsedCard.info.caps & 0x40)) {
#line 609
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ANTIALIAS, D3DANTIALIAS_SORTINDEPENDENT));
        } else {
#line 611
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ANTIALIAS, D3DANTIALIAS_NONE));
        }
        if (c->dither && (d3d_UsedCard.info.caps & 0x80))
#line 614
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_DITHERENABLE, TRUE));
        if (c->subpixel && (d3d_UsedCard.info.flags & 1))
#line 617
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_SUBPIXEL, c->subpixel));
        d3d_StatesSet = 1;
        D3_COPY_AT(offset d3d_Config, c, sizeof(D3DCONFIG));
    }
}
