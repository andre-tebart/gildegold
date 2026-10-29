/* 0x441300, the divide maps: per map quad, which diagonal splits it (bit 7) and whether all its
   corners have the same tile type (bit 6); then the height range of each tile
   @deep B: code 497 / regs 2 / fill 1 - semantics verified in round 4 (level loops 0/1/2, d3fl_PrepareWater, tile height range; readable): the remaining code hunks are register allocation (map in ebp in the original, edi in ours), the stack slots, and the (abs <= abs) * 2 | SameType << 6 expression, where the original builds 2*x in a byte (add al,al) and shifts once after the OR while ours distributes the shift (shl al,7 / shl,6) - the same value. Tried byte/int temp, d |= ...; d <<= 6, * 64, +, ? 2 : 0, (x << 1 | f) << 6, casts (round-4 note): all same operation other spelling, needs vargen */

void d3fl_MakeDivide(D3FLMAP *map)
{
    unsigned char step[3] = { 1, 2, 4 };
    unsigned int mask = map->mask;
    int size = map->size;
    int x, y, l, s, n, i, j;
    unsigned int x1, y1;
    unsigned char *h;
    unsigned char mn, mx, v;
    int k, d; unsigned char c;

    for (y = 0; y < size; y++) {
        y1 = (y + 1) & mask;
        for (x = 0; x < size; x++) {
            h = map->height;
            x1 = (x + 1) & mask;
            map->divide[0][y * size + x] = ((abs(h[y * size + x] - h[y1 * size + x1]) <= abs(h[y * size + x1] - h[y1 * size + x])) * 2
                                            | d3fl_SameType(map, x, y, 1)) << 6;
        }
    }
    for (l = 1; l < 3; l++) {
        s = step[l];
        n = map->size / s;
        for (y = 0; y < n; y++) {
            y1 = (y * s + s) & mask;
            for (x = 0; x < n; x++) {
                size = map->size;
                x1 = (x * s + s) & mask;
                h = map->height;
                map->divide[l][y * n + x] = ((abs(h[(y * size + x) * s] - h[y1 * size + x1]) <= abs(h[y * s * size + x1] - h[y1 * size + x * s])) * 2
                                             | d3fl_SameType(map, x * s, y * s, s)) << 6;
            }
        }
    }
    d3fl_PrepareWater(map);
    for (j = 0; j < 8; j++)
        for (i = 0; i < 8; i++) {
            mx = 0;
            mn = 0xff;
            for (y = 0; y <= map->n; y++)
                for (x = 0; x <= map->n; x++) {
                    k = ((map->n * i + x) & mask) + ((map->n * j + y) & mask) * map->size;
                    v = map->height[k];
                    mx = max(mx, v);
                    mn = min(mn, v);
                    if (map->water && (v = map->water[k]) > 0) {
                        mx = max(mx, v);
                        mn = min(mn, v);
                    }
                }
            map->tiles[j][i].hmax = mx;
            map->tiles[j][i].hmin = mn;
            map->tiles[j][i].lod = -1;
        }
    map->dirty = 1;
}
