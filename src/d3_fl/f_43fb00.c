/* 0x43fb00, the cache entry for the (size + 1)^2 tiles at x, y */

D3FLCACHE *d3fl_Find(unsigned char *map, int mapsize, int x, int y, unsigned int size, int *phash)
{
    unsigned char tiles[5][5];
    unsigned int i, j;
    int hash;
    D3FLCACHE *c;
    int mask = mapsize - 1;
    unsigned int n = size + 1;

    D3_ZERO(tiles, sizeof(tiles));
    for (j = 0; j < n; j++)
        for (i = 0; i < n; i++)
            tiles[j][i] = map[((x + i) & mask) + ((j + y) & mask) * mapsize];
    hash = util_Hash(tiles, 25) + d3_CurrentScene;
    if (phash)
        *phash = hash;
    for (i = 0, c = d3fl_Cache; i < d3fl_Count; i++, c++)
        if (c->tex && c->size == size && c->hash == hash) {
            d3fl_Cache[i].stamp = d3_FrameCount;
            return &d3fl_Cache[i];
        }
    return 0;
}
