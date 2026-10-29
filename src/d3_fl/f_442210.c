/* 0x442210, replaces the texture of tile type idx */

int d3fl_ReplaceTexture(D3FLMAP *map, char *name, int idx)
{
    int i;

    map->typenames[idx][0] = d3fl_EmptyName;
    for (i = 0; i < 3; i++)
        if (map->src[i][idx]) {
            mem_Free(map->src[i][idx]);
            map->src[i][idx] = 0;
        }
    i = d3fl_LoadTexture(map, name, idx);
    if (i >= 0) {
        d3fl_Flush();
        d3fl_Invalidate(map);
    }
    return i;
}
