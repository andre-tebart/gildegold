/* 0x45d960, sets the scroll speed of the layer (the caller's units scaled by -1e-06 into the layer's per-ms speed); 0 stops it */

void d3sky_SetLayerSpeed(D3SKY *s, D3SKYLAYER *l, float speed)
{
    if (s && l) {
        if (speed != 0.0f)
            l->speed = speed * -1e-06f;
        else
            l->speed = 0;
    }
}
