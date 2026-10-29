/* 0x4387b0, d3.h: d3_SetBlendState; alpha blending (balpha), additive blending (glow), color key. */

void d3_SetBlendState(unsigned char balpha, unsigned char glow, unsigned char colorkey)
{
    if (balpha || glow) {
        if (!d3d_balpha) {
#line 1285
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ZBIAS, 16));
            if (d3d_zbuffer) {
#line 1287
                D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ZWRITEENABLE, FALSE));
            }
#line 1288
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ALPHABLENDENABLE, TRUE));
            if (d3d_UsedCard.info.flags & 8) {
#line 1290
                D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_STIPPLEDALPHA, TRUE));
            }
            d3d_balpha = 1;
        }
        if (glow) {
            if (!d3d_glow) {
#line 1295
                D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_DESTBLEND, d3d_UsedCard.info.glow_destblend));
                D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_SRCBLEND, d3d_UsedCard.info.glow_srcblend));
                if (d3d_fog) {
#line 1298
                    D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_FOGENABLE, FALSE));
                }
                d3d_glow = 1;
            }
        } else {
#line 1303
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_DESTBLEND, d3d_UsedCard.info.trans_destblend));
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_SRCBLEND, d3d_UsedCard.info.trans_srcblend));
            if (d3d_fog) {
#line 1306
                D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_FOGENABLE, TRUE));
            }
            d3d_glow = 0;
        }
    } else if (d3d_balpha) {
#line 1312
        D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ZBIAS, 0));
        if (d3d_zbuffer) {
#line 1314
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ZWRITEENABLE, TRUE));
        }
#line 1315
        D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ALPHABLENDENABLE, FALSE));
        if (d3d_UsedCard.info.flags & 8) {
#line 1317
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_STIPPLEDALPHA, FALSE));
        }
#line 1318
        D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_DESTBLEND, D3DBLEND_ZERO));
        D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE));
        d3d_balpha = 0;
        d3d_glow = 0;
    }
    if (colorkey != d3d_colorkey) {
#line 1324
        D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_COLORKEYENABLE, colorkey));
        d3d_colorkey = colorkey;
    }
    if ((colorkey && !d3d_balpha) != d3d_alphatest) {
#line 1329
        D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ALPHATESTENABLE, colorkey && !d3d_balpha));
        d3d_alphatest = colorkey && !d3d_balpha;
    }
}
