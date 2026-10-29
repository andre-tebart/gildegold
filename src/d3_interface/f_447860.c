/* 0x447860, precaches the textures of all objects (inside Begin/EndScene) and prerenders the map */

void d3if_Prerender(char map)
{
    unsigned int i;

    for (i = 0; i < d3_TexCount; i++)
        d3_Textures[i].flags &= 0x7f;
    d3d_Device->lpVtbl->BeginScene(d3d_Device);
    d3_ForEachObject2(d3_DefaultScene, 0, 0x40, d3if_PrecacheCB);
    if (d3_Terrain && map)
        d3fl_Prerender(d3_Terrain);
    d3d_Device->lpVtbl->EndScene(d3d_Device);
}
