/* 0x415380, per frame: eases count and wind toward their targets over time60..time64 / time68..time6c,
   advances the flakes (d3t_UpdateSnow) and draws every visible flake as a two-triangle textured quad
   (D3DPT_TRIANGLELIST, at most 192 vertices per batch) tinted 0x50646464; no-op without a snow system. */

void d3t_DrawSnow(D3TSNOW *Snow)
{
    D3DTLVERTEX VertBuff[192];
    int i, n, total, left, done;
    float dt, depth;
    D3TSNOWFLAKE *flake;

    if (!Snow)
        return;
    if (Snow->time60 != Snow->time64) {
        if (d3_Time >= (unsigned long)Snow->time64) {
            Snow->count = Snow->newcount;
        } else {
            total = Snow->time64 - Snow->time60;
            left = Snow->time64 - d3_Time;
            Snow->count = (total - left) * Snow->newcount / total + Snow->oldcount * left / total;
        }
    }
    if (Snow->time68 != Snow->time6c) {
        if (d3_Time >= (unsigned long)Snow->time6c) {
            Snow->windx = Snow->newwindx;
            Snow->windz = Snow->newwindz;
        } else {
            total = Snow->time6c - Snow->time68;
            left = Snow->time6c - d3_Time;
            done = total - left;
            Snow->windx = done * Snow->newwindx / total + left * Snow->oldwindx / total;
            Snow->windz = done * Snow->newwindz / total + left * Snow->oldwindz / total;
        }
    }
    dt = (d3_Time - Snow->time5c) * 0.1f;
    Snow->time5c = d3_Time;

    d3t_UpdateSnow(Snow, dt);
    d3_BeginScene(0, 0, 1);
    d3_SetBlendState(1, 1, 0);
#line 2942
    D3DCHECK(d3d_Device->lpVtbl->SetTexture(d3d_Device, 0, Snow->Texture->surftext));

    n = 0;
    flake = Snow->flakes;
    for (i = 0; i < Snow->count; flake++, i++) {
        if (flake->sx1 >= d3_Viewport.left && flake->sx2 < d3_Viewport.right &&
            flake->sy1 >= d3_Viewport.top && flake->sy2 < d3_Viewport.bottom) {
            depth = (1.0f - flake->pos.z) * 0.025f;
            VertBuff[n].sx = (flake->sx2 + flake->sx1) * 0.5f;
            VertBuff[n].sy = flake->sy1;
            VertBuff[n].sz = depth;
            VertBuff[n].rhw = 1.0f;
            VertBuff[n].tu = 0.5f;
            VertBuff[n].tv = 0;
            VertBuff[n].color = 0x50646464;
            VertBuff[n].specular = 0;
            n++;
            VertBuff[n].sx = flake->sx2;
            VertBuff[n].sy = flake->sy2;
            VertBuff[n].sz = depth;
            VertBuff[n].rhw = 1.0f;
            VertBuff[n].tu = 1.0f;
            VertBuff[n].tv = 1.0f;
            VertBuff[n].color = 0x50646464;
            VertBuff[n].specular = 0;
            n++;
            VertBuff[n].sx = flake->sx1;
            VertBuff[n].sy = flake->sy2;
            VertBuff[n].sz = depth;
            VertBuff[n].rhw = 1.0f;
            VertBuff[n].tu = 0;
            VertBuff[n].tv = 1.0f;
            VertBuff[n].color = 0x50646464;
            VertBuff[n].specular = 0;
            n++;
        }
        if (n >= 192) {
#line 2984
            D3DCHECK(d3d_Device->lpVtbl->DrawPrimitive(d3d_Device, D3DPT_TRIANGLELIST, D3DFVF_TLVERTEX, VertBuff, n, D3DDP_DONOTLIGHT|D3DDP_DONOTCLIP|D3DDP_DONOTUPDATEEXTENTS));
            n = 0;
        }
    }
    if (n) {
#line 2989
        D3DCHECK(d3d_Device->lpVtbl->DrawPrimitive(d3d_Device, D3DPT_TRIANGLELIST, D3DFVF_TLVERTEX, VertBuff, n, D3DDP_DONOTLIGHT|D3DDP_DONOTCLIP|D3DDP_DONOTUPDATEEXTENTS));
    }
    d3_EndScene();
}
