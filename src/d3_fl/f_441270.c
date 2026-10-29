/* 0x441270, 1 if all map points of the (n + 1)^2 square at x, y have the same tile type */

char d3fl_SameType(D3FLMAP *map, int x, int y, int n)
{
    int i, j;
    unsigned char t = 0xff;

    for (j = y; j <= y + n; j++)
        for (i = x; i <= x + n; i++) {
            int k = (i & map->mask) + (j & map->mask) * map->size;

            if (t == 0xff)
                t = map->types[k];
            else if (t != map->types[k])
                return 0;
        }
    return 1;
}
