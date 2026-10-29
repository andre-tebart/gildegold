/* 0x441ea0, allocates the light and divide maps and the tiles, then computes the divide maps */

void d3fl_Inflate(D3FLMAP *map)
{
    int i, j;

    if (!map->light)
        map->light = mem_Alloc(map->size * map->size + 1, "d3fl:Inflate(f->light)");
    for (i = 0; i < 3; i++)
        if (!map->divide[i]) {
            int n = map->size >> i;

            map->divide[i] = mem_Alloc(n * n + 1, "d3fl:Inflate(f->divide)");
        }
    for (j = 0; j < 8; j++)
        for (i = 0; i < 8; i++)
            d3fl_InitTile(&map->tiles[j][i], map->n, map->shift);
    d3fl_MakeDivide(map);
    map->dirty = 1;
}
