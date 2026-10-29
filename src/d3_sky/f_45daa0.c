/* 0x45daa0 */

void d3sky_Destroy(D3SKY *s)
{
    if (s) {
        while (s->layers)
            d3sky_DeleteLayer(s, s->layers);
        mem_Free(s);
    }
}
