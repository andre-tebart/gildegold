/* 0x43f990, empties the cache */

void d3fl_Flush(void)
{
    unsigned int i;

    for (i = 0; i < d3fl_Count; i++) {
        if (d3fl_Cache[i].tex) {
            if (d3fl_Cache[i].tex->name[0] == '*')
                while (d3fl_Cache[i].tex->refs > 0)
                    tex_Release((D3TEXHANDLE *)d3fl_Cache[i].tex);
            d3fl_Cache[i].tex = 0;
        }
        d3fl_Cache[i].flag = 0;
        d3fl_Cache[i].stamp = 0;
        d3fl_Cache[i].size = 0;
        d3fl_Cache[i].hash = 0;
        D3_ZERO(d3fl_Cache[i].t.tiles, sizeof(d3fl_Cache[i].t.tiles));
    }
    if (d3_Terrain)
        d3fl_Invalidate(d3_Terrain);
    for (i = 0; i < D3_MAX_SCENES; i++)
        if (d3_Scenes[i].map && d3_Scenes[i].map != d3_Terrain)
            d3fl_Invalidate(d3_Scenes[i].map);
}
