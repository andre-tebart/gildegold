/* 0x441f50, allocates the divide and light maps of a freshly loaded map and resets its tiles */

/* ---- 0x441f50..0x4439e0 (worker map) ---- */

void d3fl_InitMap(D3FLMAP *map)
{
    int i, j;

    map->watertype = 0xff;
    map->divide[0] = mem_Alloc(map->size * map->size, "d3_fl:Divide0");
    map->divide[1] = mem_Alloc((unsigned int)(map->size * map->size) >> 2, "d3_fl:Divide1");
    map->divide[2] = mem_Alloc((unsigned int)(map->size * map->size) >> 4, "d3_fl:Divide2");
    map->light = mem_Alloc(map->size * map->size, "d3_fl:Light");
    if (!map->lightofs)
        map->lightofs = mem_Alloc(map->size * map->size * 4, "d3_fl:LightOffset");
    for (j = 0; j < 8; j++)
        for (i = 0; i < 8; i++) {
            D3_ZERO(&map->tiles[j][i], sizeof(D3FLTILE));
            d3fl_InitTile(&map->tiles[j][i], map->n, map->shift);
        }
    d3fl_MakeDivide(map);
    d3fl_MakeLight(map);
    map->dirty = 1;
}
