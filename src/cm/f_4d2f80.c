/* 0x4d2f80, gives the characters of a side (the ids +0x80 / +0xc0 of the fight record) their formation: per character a
   random roll against the row 0x4d2c00 picks the type 0 .. 5, the character with the best rating 0x4d2e30 for it takes it
   (+0x1c4) unless it already has one and a second roll fails; then 0x4d3120 sets up each. Rough draft. */

void ai_Unk4d2f80(int side)
{
    char *rec = ai_Unk6f0d04;
    char *c[16];
    int *arr;
    float *row;
    char *rec2 = 0;
    char *sel;
    float r, best, w;
    int cnt = 0, i, j, k, n = 0, selidx = 0;

    row = ai_Unk4d2c00(side);
    for (i = 0; i < (unsigned char)rec[0x30]; i++) {
        if (*(int *)(rec + 0x34 + i * 4) == *(int *)((char *)side + 4)) {
            rec2 = ai_Unk92cae0[i];
            break;
        }
    }
    if (side == ai_Unk6f0cec)
        arr = (int *)(rec + 0xc0);
    else
        arr = (int *)(rec + 0x80);
    for (i = 0; i < 16; i++) {
        if (arr[i] != -1) {
            c[i] = ai_Unk4ce130(arr[i]);
            n++;
        } else
            c[i] = 0;
    }
    k = 0;
    for (cnt = n; cnt > 0; cnt--) {
        r = util_RandomF();
        for (j = 0; j < 6; j++) {
            if (r <= row[j]) {
                k = j;
                break;
            }
        }
        best = -1.0f;
        for (i = 0; i < 16; i++) {
            if (c[i] != 0) {
                w = ai_Unk4d2e30(c[i], k);
                if (w > best) {
                    best = w;
                    selidx = i;
                }
            }
        }
        sel = c[selidx];
        if (sel[0x1c4] == 0 || ai_Unk4e5360() > ai_Unk5eb9b0) {
            sel[0x1c4] = (char)k;
            c[selidx] = 0;
        }
    }
    for (i = 0; i < 16; i++) {
        if (arr[i] != -1)
            ai_Unk4d3120(rec2, ai_Unk4ce130(arr[i]));
    }
}
