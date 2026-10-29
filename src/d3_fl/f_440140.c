/* 0x440140, marks the map dirty and every tile's lod invalid (-1), so the next use recomputes them */

void d3fl_Invalidate(D3FLMAP *map)
{
    int i, j;

    map->dirty = 1;
    for (j = 0; j < 8; j++)
        for (i = 0; i < 8; i++)
            map->tiles[j][i].lod = -1;
}
