/* 0x45d500, draws the sky: advances every layer's fade and texture offset by the elapsed time, picks the lowest layer that is fully opaque and not additive, and draws it and its successors between BeginScene and EndScene (not while another sky draw is in flight); returns 0 without a device, hardware or sky, else 1 */

char d3sky_Draw(D3SKY *s)
{
    D3SKYLAYER *l, *base;
    int a;

    if (!d3d_Device || !d3_Hardware || !s)
        return 0;
    if (s->num_buffer == -1)
        d3sky_Build(s);
    s->dt = (float)(d3_Time - s->lasttime);
    s->lasttime = d3_Time;
    if (s->layers) {
        for (l = s->layers; l; l = l->next) {
            if ((l->fade = l->fadespeed * s->dt + l->fade) > 1.0) {
                l->fadespeed = 0;
                l->fade = 1.0f;
            }
            a = (int)((float)l->alpha0 + ((float)l->alpha1 - (float)l->alpha0) * l->fade);
            if (a > 0xff)
                a = 0xff;
            l->alpha = a;
            l->offset = (float)fmod(l->speed * s->dt + l->offset, 1.0);
        }
        base = s->layers;
        for (l = base->next; l; l = l->next) {
            if (l->alpha == 0xff && !(l->flags.b[2] & 1) && !(l->flags.b[2] & 2))
                base = l;
        }
        if (base && s->num_buffer > 0 && d3_Busy <= 1) {
            d3_Busy++;
            d3_BeginScene(d3_FogOn, d3_FogColor, 0);
            for (l = base; l; l = l->next)
                d3sky_DrawLayer(s, l);
            d3_EndScene();
            d3_Busy--;
        }
    }
    return 1;
}
