/* 0x5bf0d0, the candidate count / list of a special-amount page (560 B): with param_2 == 0 it counts the
   characters 0x770b80 + i * 0x21c that are not param_1, alive (word 0 != -1), of the kind byte +2 < 10 and
   either in the 8 ids at param_1 + 0x5c (the id +4) or of the same world (word +0x50) and land byte +0x58,
   plus the carrier 0x53e400 (1, 0, 0x41) of the +0xac owner; with a count (param_2) it fills the records
   param_3 (0xe dwords: +0 the character, +0x2c the class, +0x30 the 0x5c33c0 result) */

unsigned int ai_Unk5bf0d0(short *param_1, unsigned int param_2, int *param_3, int param_4)
{
    unsigned int n;
    int i;
    char *p;
    char *car;
    int *rec;
    int *ids;
    int j;

    n = 0;
    ids = (int *)((char *)param_1 + 0x5c);
    if (param_2 == 0) {
        for (p = (char *)(unsigned)&ai_Chars; p < (char *)(unsigned)((unsigned)ai_Chars + 0x65400); p += 0x21c) {
            if (p == (char *)param_1 || *(short *)p == -1 || *(unsigned char *)(p + 2) >= 10)
                continue;
            for (j = 0; j < 8; j++)
                if (ids[j] == U_I(p + 4))
                    break;
            if (j < 8 || (*(unsigned short *)(p + 0x50) == *(unsigned short *)((char *)param_1 + 0x50) &&
                          *(char *)(p + 0x58) == *((char *)param_1 + 0x58)))
                n++;
        }
        car = (char *)he_FindEvent(1, 0, 0x41);
        while (car != 0) {
            if (U_I(car + 0xac) == U_I((char *)param_1 + 4))
                return n + 1;
            car = (char *)he_NextEvent();
        }
        return n;
    }
    for (i = 0; i < (int)(param_2 * 0x38 / 4); i++)
        param_3[i] = 0;
    rec = param_3;
    for (p = (char *)(unsigned)&ai_Chars; p < (char *)(unsigned)((unsigned)ai_Chars + 0x65400); p += 0x21c) {
        if (n >= param_2)
            break;
        if (p == (char *)param_1 || *(short *)p == -1 || *(unsigned char *)(p + 2) >= 10)
            continue;
        for (j = 0; j < 8; j++)
            if (ids[j] == U_I(p + 4))
                break;
        if (j < 8) {
            rec[0] = (int)p;
            if (j == 0)
                rec[0xb] = (*(char *)(p + 9) != 0) + 4;
            else if (j == 1)
                rec[0xb] = 0;
            else if (j == 2)
                rec[0xb] = 1;
            else
                rec[0xb] = (*(char *)(p + 9) != 0) + 6;
            n++;
            rec += 0xe;
        } else if (*(unsigned short *)(p + 0x50) == *(unsigned short *)((char *)param_1 + 0x50) &&
                   *(char *)(p + 0x58) == *((char *)param_1 + 0x58)) {
            rec[0] = (int)p;
            rec[0xb] = (*(char *)(p + 9) != 0) + 2;
            n++;
            rec += 0xe;
        }
    }
    if (n < param_2) {
        car = (char *)he_FindEvent(1, 0, 0x41);
        while (car != 0) {
            if (U_I(car + 0xac) == U_I((char *)param_1 + 4)) {
                i = gm_FindChar(U_I(car + 0xb0));
                if (i != 0 && U_I(i + 4) != ids[0]) {
                    rec[0] = i;
                    rec[0xb] = (*(char *)(i + 9) != 0) + 8;
                    n++;
                }
                break;
            }
            car = (char *)he_NextEvent();
        }
    }
    rec = param_3;
    for (i = 0; i < (int)n; i++) {
        ai_AmountRowStatus(rec);
        rec[0xc] = ai_Unk5c33c0(*(unsigned short *)param_1, *(unsigned short *)rec[0],
                                (unsigned int *)param_4, 0);
        rec += 0xe;
    }
    return n;
}
