/* 0x412000, value range of row y between columns x0 and x1 */

typedef struct D3TMAP {             /* square 8-bit height map */
    int size;
    char pad04[0xc];
    unsigned char *data;            /* 0x10 */
    char pad14[0x7c];
    D3VECTOR org;                   /* 0x90, world position of cell 0 */
    char pad9c[0x4];
    float cellx;                    /* 0xa0 */
    char pada4[0x14];
    float cellz;                    /* 0xb8 */
    char padbc[0x8];
    float scale;                    /* 0xc4, height per unit */
} D3TMAP;

int d3t_GetRowRange(D3TMAP *map, int *lo, int *hi, int x0, int x1, int y)
{
    unsigned char *p;
    int t;

    if (y >= 0 && y < map->size) {
        if (x1 < x0) {
            t = x0;
            x0 = x1;
            x1 = t;
        }
        if (x0 < map->size && x1 >= 0) {
            x0 = max(0, x0);
            x1 = min(x1, map->size - 1);
            p = map->data + map->size * y + x0;
            for (; x0 <= x1; x0++, p++) {
                *lo = min(*lo, *p);
                *hi = max(*hi, *p);
            }
            return 0;
        }
    }
    return 1;
}
