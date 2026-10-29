/* 0x45d9a0, fades the layer to alpha in time ms */

void d3sky_FadeLayer(D3SKY *s, D3SKYLAYER *l, unsigned char alpha, float time)
{
    if (s && l) {
        /* alpha0 is stored in both arms and the statement order is needed for VC6's registers */
        if (time != 0.0f) {
            l->alpha0 = l->alpha;
            l->fadespeed = 1.0f / time;
            l->alpha1 = alpha;
            l->fade = 0;
        } else {
            l->alpha0 = l->alpha;
            l->fade = 1.0f;
            l->fadespeed = 0;
            l->alpha1 = l->alpha;
        }
    }
}
