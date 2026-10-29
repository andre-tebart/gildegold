/* 0x449da0, d3_ForEachObject callback: sets a light to the day time *time (key + fraction) */

char d3_SetLightKeyCB(D3OBJECT *obj, float *time)
{
    float t = *time;
    unsigned int key = (unsigned int)t;

    d3_SetLightKey(obj, key, t - key);
    return 1;
}
