/* 0x4bd340, path_Find from cell x / z to tx / tz (both < 0: the nearest road cell, type 6 / 0xb); 1 if found @deep behaviour: far draft, 234 code hunks - the same operations in another allocation (ebx vs edi for the zero, the parameters read at different frame slots, 0x18 vs 0xc locals) and reordered compares; the nearest-road cell scan and the path_Find call were read and match in meaning */

int rd_PathToRoad(G15ROAD *r, int x, int z, int tx, int tz)
{
    int n;
    int i, j;
    int *c;
    float d, best;
    int row;

    if (tx < 0 || tz < 0) {
        n = r->grid->size;
        best = (float)(n * n * 2);
        tx = -1;
        tz = -1;
        if (n < 1)
            return 0;
        row = 0;
        for (i = 0; i < n; i++) {
            c = (int *)((char *)r->grid->cells + row);
            for (j = 0; j < n; j++) {
                if ((*c == 6 || *c == 0xb)
                    && (d = (float)((i - z) * (i - z) + (j - x) * (j - x))) < best) {
                    tx = j;
                    tz = i;
                    best = d;
                }
                c += 7;
            }
            row += n * 0x1c;
        }
    }
    if (tx > -1 && tz > -1) {
        r->max = 0x400;
        r->pts = r->path;
        r->n = 0;
        return path_Find(&r->n, r->grid, x, z, tx, tz, 3) != 0xffff;
    }
    return 0;
}
