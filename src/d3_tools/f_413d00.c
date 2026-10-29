/* 0x413d00, frees the rain system: its drop list (D3TRAIN.drops) and the D3TRAIN itself;
   null members are skipped. */

void d3t_DeleteRain(D3TRAIN *rain)
{
    if (rain) {
        if (rain->drops) {
            mem_Free(rain->drops);
            rain->drops = 0;
        }
        mem_Free(rain);
    }
}
