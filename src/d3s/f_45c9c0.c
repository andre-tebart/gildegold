/* 0x45c9c0, d3_ForEachObject callback: collects up to 4 shadow casting lights */

char d3s_LightCB(D3OBJECT *obj, void *ctx)
{
    if (obj->objectFlags & 4)
        d3s_Lights[d3s_NumLights++] = obj;
    return d3s_NumLights < 4;
}
