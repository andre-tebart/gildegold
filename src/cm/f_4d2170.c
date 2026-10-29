/* 0x4d2170, the best ground point (*ox, *oy) around the cell (cx, cy) within the rings 0 .. r - 1 of the scene's map: the
   cell types other than 0 / 0xd are rated with 0x4d2010 (limit r * radius * 2), the least value wins; returns 0 when no
   cell was rated. Rough draft. */

int ai_Unk4d2170(char *c, int cx, int cy, int r, int *ox, int *oy)
{
    char *sc = *(char **)(ai_Unk5fb5b0 + 0xb0);
    float best = 1000000000.0f;
    float f;
    int k, row, col, hi, hic, m, cell;

    for (k = 0; k < r; k++) {
        hi = cy + k;
        if (*(int *)(sc + 0x20) - 1 < hi)
            hi = *(int *)(sc + 0x20) - 1;
        for (row = (cy - k < 0) ? 0 : cy - k; row <= hi; row++) {
            m = k - abs(cy - row);
            hic = cx + m;
            if (*(int *)(sc + 0x20) - 1 < hic)
                hic = *(int *)(sc + 0x20) - 1;
            for (col = (cx - m < 0) ? 0 : cx - m; col <= hic; col++) {
                cell = (*(int **)(sc + 0x24))[(*(int *)(sc + 0x20) * row + col) * 7];
                if (cell != 0 && cell != 0xd) {
                    f = ai_Unk4d2010(c, col, row, (float)(r * *(float *)(sc + 0x10) * 2));
                    if (f < best) {
                        best = f;
                        *ox = col;
                        *oy = row;
                    }
                }
            }
        }
    }
    return best != ai_Unk5edc60;
}
