/* 0x43f930, switches mipmapping and reloads the textures of the terrain and of every scene's map */

void d3fl_SetMips(char on)
{
    int i;

    if (on != d3fl_Mips) {
        d3fl_Flush();
        d3fl_Mips = on;
        if (d3_Terrain)
            d3fl_ReloadMap(d3_Terrain);
        for (i = 0; i < D3_MAX_SCENES; i++)
            if (d3_Scenes[i].map && d3_Scenes[i].map != d3_Terrain)
                d3fl_ReloadMap(d3_Scenes[i].map);
    }
}
