/* 0x45ca20 */

void d3s_UpdateObject(D3OBJECT *obj)
{
    unsigned int i;

    if (obj->objectFlags & 4) {
        for (i = 0; i < d3s_NumLights; i++)
            d3s_Update(obj, d3s_Lights[i], (D3TERRAIN *)d3_Terrain);
    }
}
