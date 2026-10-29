/* 0x414590, per frame: eases count and wind toward their targets over time30..time34 / time38..time3c,
   advances the drops (d3t_UpdateRain) and draws every fully visible drop as a line from head to tail
   (D3DPT_LINELIST, at most 128 vertices per batch) tinted by a count-based fade; no-op without drops. */

void d3t_DrawRain(D3TRAIN *rain)
{
    D3DTLVERTEX VertBuff[128];
    int i, n, total, left, done;
    float dt, fade, depth;
    D3DCOLOR color;
    D3TRAINDROP *drop;

    if (!rain)
        return;
    if (rain->time30 != rain->time34) {
        if (d3_Time >= (unsigned long)rain->time34) {
            rain->count = rain->newcount;
        } else {
            total = rain->time34 - rain->time30;
            left = rain->time34 - d3_Time;
            rain->count = (total - left) * rain->newcount / total + rain->oldcount * left / total;
        }
    }
    if (rain->time38 != rain->time3c) {
        if (d3_Time >= (unsigned long)rain->time3c) {
            rain->windx = rain->newwindx;
            rain->windz = rain->newwindz;
        } else {
            total = rain->time3c - rain->time38;
            left = rain->time3c - d3_Time;
            done = total - left;
            rain->windx = done * rain->newwindx / total + left * rain->oldwindx / total;
            rain->windz = done * rain->newwindz / total + left * rain->oldwindz / total;
        }
    }
    dt = (d3_Time - rain->time2c) * 0.1f;
    rain->time2c = d3_Time;
    if (!rain->count)
        return;

    d3t_UpdateRain(rain, dt);
    d3_BeginScene(0, 0, 1);
    d3_SetBlendState(1, 0, 0);
    fade = 1.0f - rain->count * 0.0005f;
    color = RGBA_MAKE((int)(96.0f * fade), (int)(128.0f * fade), (int)(128.0f * fade), 0x80);

    n = 0;
    drop = rain->drops;
    for (i = 0; i < rain->count; drop++, i++) {
        if (drop->sx1 >= d3_Viewport.left && drop->sx1 < d3_Viewport.right &&
            drop->sx2 >= d3_Viewport.left && drop->sx2 < d3_Viewport.right &&
            drop->sy1 >= d3_Viewport.top && drop->sy1 < d3_Viewport.bottom &&
            drop->sy2 >= d3_Viewport.top && drop->sy2 < d3_Viewport.bottom) {
            depth = (1.0f - drop->pos.z) * 0.025f;
            VertBuff[n].sx = drop->sx1;
            VertBuff[n].sy = drop->sy1;
            VertBuff[n].sz = depth;
            VertBuff[n].rhw = 1.0f;
            VertBuff[n].tu = 0;
            VertBuff[n].tv = 0;
            VertBuff[n].color = color;
            VertBuff[n].specular = 0;
            n++;
            VertBuff[n].sx = drop->sx2;
            VertBuff[n].sy = drop->sy2;
            VertBuff[n].sz = depth;
            VertBuff[n].rhw = 1.0f;
            VertBuff[n].tu = 0;
            VertBuff[n].tv = 0;
            VertBuff[n].color = color;
            VertBuff[n].specular = 0;
            n++;
        }
        if (n >= 128) {
#line 2343
            D3DCHECK(d3d_Device->lpVtbl->DrawPrimitive(d3d_Device, D3DPT_LINELIST, D3DFVF_TLVERTEX, VertBuff, n, D3DDP_DONOTLIGHT|D3DDP_DONOTCLIP|D3DDP_DONOTUPDATEEXTENTS));
            n = 0;
        }
    }
    if (n) {
#line 2348
        D3DCHECK(d3d_Device->lpVtbl->DrawPrimitive(d3d_Device, D3DPT_LINELIST, D3DFVF_TLVERTEX, VertBuff, n, D3DDP_DONOTLIGHT|D3DDP_DONOTCLIP|D3DDP_DONOTUPDATEEXTENTS));
    }
    d3_EndScene();
}
