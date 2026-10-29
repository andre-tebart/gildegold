/* 0x43ff90 */

void d3fl_FreeMap(D3FLMAP *map)
{
    int i, j;

    for (i = 0; i < 8; i++)
        for (j = 0; j < 3; j++)
            if (map->src[j][i]) {
                mem_Free(map->src[j][i]);
                map->src[j][i] = 0;
            }
    for (j = 0; j < 8; j++)
        for (i = 0; i < 8; i++) {
            mem_Free(map->tiles[j][i].points);
            map->tiles[j][i].points = 0;
            mem_Free(map->tiles[j][i].polys);
            map->tiles[j][i].polys = 0;
            mem_Free(map->tiles[j][i].split);
            map->tiles[j][i].split = 0;
            mem_Free(map->tiles[j][i].bp);
            map->tiles[j][i].bp = 0;
            if (map->tiles[j][i].water) {
                mem_Free(map->tiles[j][i].wregions);
                map->tiles[j][i].wregions = 0;
                mem_Free(map->tiles[j][i].wpoints);
                map->tiles[j][i].wpoints = 0;
                mem_Free(map->tiles[j][i].wpolys);
                map->tiles[j][i].wpolys = 0;
            }
        }
    if (map->height) {
        mem_Free(map->height);
        map->height = 0;
    }
    if (map->types) {
        mem_Free(map->types);
        map->types = 0;
    }
    if (map->water) {
        mem_Free(map->water);
        map->water = 0;
    }
    if (map->light) {
        mem_Free(map->light);
        map->light = 0;
    }
    if (map->lightofs) {
        mem_Free(map->lightofs);
        map->lightofs = 0;
    }
    if (map->regions) {
        for (i = 0; i < map->nregions; i++)
            tex_Release((D3TEXHANDLE *)map->regions[i].tex);
        mem_Free(map->regions);
        map->regions = 0;
    }
    for (i = 0; i < 3; i++)
        if (map->divide[i]) {
            mem_Free(map->divide[i]);
            map->divide[i] = 0;
        }
    d3fl_Flush();
    mem_Free(map);
}
