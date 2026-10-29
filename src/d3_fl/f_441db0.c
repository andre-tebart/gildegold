/* 0x441db0, frees the light, divide and tile buffers of a map */

void d3fl_FreeTiles(D3FLMAP *map)
{
    int i, j;

    if (map->light) {
        mem_Free(map->light);
        map->light = 0;
    }
    for (i = 0; i < 3; i++)
        if (map->divide[i]) {
            mem_Free(map->divide[i]);
            map->divide[i] = 0;
        }
    for (j = 0; j < 8; j++)
        for (i = 0; i < 8; i++) {
            map->tiles[j][i].lod = 0;
            map->tiles[j][i].lodLevel = 0;
            map->tiles[j][i].npolys = 0;
            map->tiles[j][i].npoints = 0;
            if (map->tiles[j][i].points) {
                mem_Free(map->tiles[j][i].points);
                map->tiles[j][i].points = 0;
            }
            if (map->tiles[j][i].polys) {
                mem_Free(map->tiles[j][i].polys);
                map->tiles[j][i].polys = 0;
            }
            if (map->tiles[j][i].split) {
                mem_Free(map->tiles[j][i].split);
                map->tiles[j][i].split = 0;
            }
            if (map->tiles[j][i].bp) {
                mem_Free(map->tiles[j][i].bp);
                map->tiles[j][i].bp = 0;
            }
            if (map->tiles[j][i].water) {
                mem_Free(map->tiles[j][i].wregions);
                map->tiles[j][i].wregions = 0;
                mem_Free(map->tiles[j][i].wpoints);
                map->tiles[j][i].wpoints = 0;
                mem_Free(map->tiles[j][i].wpolys);
                map->tiles[j][i].wpolys = 0;
            }
        }
}
