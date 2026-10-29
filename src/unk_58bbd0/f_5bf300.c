/* 0x5bf300, the relation-sorted candidate list of a special-amount page (3024 B, ROUGH): with param_2 == 0
   the count of the characters (kind 2 .. 7, alive, not param_1, the relation 0x50c550 >= 75.0 and the world
   +0x108 different); otherwise the same list inserted by the relation into the param_2 records (the count
   at the last slot -3, the 0x5c33c0 result at +0x30). ROUGH: the insertion block is from the disassembly */

unsigned int ai_Unk5bf300(short *param_1, unsigned int param_2, int *param_3, int param_4)
{
    unsigned int n;
    unsigned int i;
    char *p;
    int *rec;
    float f;
    int *q;
    int t;

    n = 0;
    if (param_2 == 0) {
        for (p = (char *)(unsigned)&ai_Chars; p < (char *)(unsigned)((unsigned)ai_Chars + 0x65400); p += 0x21c) {
            if (p == (char *)param_1 || *(short *)p == -1)
                continue;
            if (*(unsigned char *)(p + 2) <= 1 || *(unsigned char *)(p + 2) >= 8)
                continue;
            f = gm_Liking(*param_1, *(short *)p, 1);
            if (f >= 75.0f && U_I(p + 0x108) != U_I((char *)param_1 + 4))
                n++;
        }
        return n;
    }
    for (i = 0; i < param_2 * 0x38 / 4; i++)
        param_3[i] = 0;
    for (p = (char *)(unsigned)&ai_Chars; p < (char *)(unsigned)((unsigned)ai_Chars + 0x65400); p += 0x21c) {
        if (p == (char *)param_1 || *(short *)p == -1)
            continue;
        if (*(unsigned char *)(p + 2) <= 1 || *(unsigned char *)(p + 2) >= 8)
            continue;
        f = gm_Liking(*param_1, *(short *)p, 1);
        if (f < 75.0f || U_I(p + 0x108) == U_I((char *)param_1 + 4))
            continue;
        if (n >= param_2)
            continue;
        if (n > 0 && (float)param_3[param_2 * 0xe - 3] >= f)
            continue;
        rec = param_3 + n * 0xe;
        rec[0] = (int)p;
        rec[0xb] = (*(char *)(p + 9) != 0) + 3;
        param_3[param_2 * 0xe - 3] = (int)f;
        n++;
        /* ROUGH: the insertion bubble (the pairs at -0xb / +3 and the relation slots -3 / +0xe of the
           array are swapped while the next relation is larger) */
        q = param_3 + (n - 1) * 0xe - 3;
        for (i = n - 1; i > 0; i--, q -= 0xe) {
            if (*q < q[0xe]) {
                t = q[-0xb];
                q[-0xb] = q[3];
                q[3] = t;
                *q = q[0xe];
                q[0xe] = (int)f;
            }
        }
    }
    if (n > param_2)
        n = param_2;
    rec = param_3;
    for (i = 0; i < n; i++) {
        ai_Unk5be1d0(rec);
        rec[0xc] = ai_Unk5c33c0(*(unsigned short *)param_1, *(unsigned short *)rec[0],
                                (unsigned int *)param_4, 0);
        rec += 0xe;
    }
    return n;
}
