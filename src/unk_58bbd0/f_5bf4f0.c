/* 0x5bf4f0, the relation-sorted candidate list of a special-amount page, second variant (501 B, ROUGH): the
   characters 0x770b80 + i * 0x21c of the kinds 2 .. 7 (alive, not param_1); with param_2 == 0 the count is
   the relation 0x50c550 > the float 0x5ed0fc (or the same world +0x210); a count fills the records (0xe
   dwords, the relation slot +0x2c starts at 100) by insertion, a same-world character entering with the
   constant 0x5eb788 and the relation of the others converted by 0x5de518; the 0x5be1d0 init and the
   0x5c33c0 result at +0x30 close. ROUGH: the exact compare directions are from the disassembly */

unsigned int ai_Unk5bf4f0(short *param_1, unsigned int param_2, int *param_3, int param_4)
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
            if (*(unsigned char *)(p + 2) < 2 || *(unsigned char *)(p + 2) > 7)
                continue;
            f = gm_Liking(*(short *)p, *(short *)param_1, 1);
            if (f > *(float *)(unsigned)&full_rdata_5ed0fc || U_I(p + 0x210) == U_I((char *)param_1 + 4))
                n++;
        }
        return n;
    }
    for (i = 0; i < param_2 * 0x38 / 4; i++)
        param_3[i] = 0;
    rec = param_3;
    for (i = 0; i < param_2; i++) {
        rec[0xb] = 100;
        rec += 0xe;
    }
    for (p = (char *)(unsigned)&ai_Chars; p < (char *)(unsigned)((unsigned)ai_Chars + 0x65400); p += 0x21c) {
        if (p == (char *)param_1 || *(short *)p == -1)
            continue;
        if (*(unsigned char *)(p + 2) < 2 || *(unsigned char *)(p + 2) > 7)
            continue;
        if (U_I(p + 0x210) != U_I((char *)param_1 + 4)) {
            f = gm_Liking(*(short *)param_1, *(short *)p, 1);
            if (!(f > *(float *)(unsigned)&full_rdata_5ed0fc))
                continue;
        } else {
            f = *(float *)(unsigned)&ai_Unk5eb788;
        }
        if (n < param_2 && (n == 0 || f >= (float)param_3[param_2 * 0xe - 3])) {
            rec = param_3 + n * 0xe;
            rec[0] = (int)p;
            param_3[param_2 * 0xe - 3] = (int)f;
            n++;
            q = param_3 + n * 0xe - 3;
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
