/* 0x4d22d0, like 0x4d2170 but the greatest rating wins (and the threshold 5eb988 must be exceeded); rough draft */

int ai_Unk4d22d0(char *c, int cx, int cy, int r, int *ox, int *oy)
{
    char *sc = *(char **)(ai_Unk5fb5b0 + 0xb0);
    float best = 0.0f;
    float thr = 0.0f;
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
                    if (f > best) {
                        best = f;
                        *ox = col;
                        *oy = row;
                    }
                }
            }
        }
    }
    if (best != ai_Unk5eb788 && thr > ai_Unk5eb988)
        return 1;
    return 0;
}
