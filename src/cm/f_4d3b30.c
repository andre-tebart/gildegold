/* 0x4d3b30, the winning side of a fight record: the character record of the side (ai_Unk6f0cec attacker, ai_Unk6f0ce8
   defender) that has nobody left standing opposite or owns every "sp_CONQUER" object of the scene; the winner gets
   message 0x50 with its fight record (0x4cdf00) marked 5. 0 / the pending result 0x6f0cf4 if no winner yet */

char *ai_Unk4d3b30(char *rec)
{
    char *w = 0;
    char *ret;
    int total = 0;
    int mine = 0;
    int na = 0, nb = 0;
    int out;
    int i, id, n;
    char *c, *o, *rr;
    int sl[11];

    if (ai_Unk6f0cf4 != 0)
        return (char *)ai_Unk6f0cf4;
    for (i = 0; i < 16; i++) {
        id = *(int *)(rec + 0x80 + i * 4);
        if (id != -1) {
            c = ai_Unk4ce130(id);
            if (c[8] != 0 && c[8] != 4 && *(int *)(*(char **)(*(char **)(c + 0x184) + 0x34) + 0x218) != 1)
                na++;
        }
    }
    for (i = 0; i < 16; i++) {
        id = *(int *)(rec + 0xc0 + i * 4);
        if (id != -1) {
            c = ai_Unk4ce130(id);
            if (c[8] != 0 && c[8] != 4 && *(int *)(*(char **)(*(char **)(c + 0x184) + 0x34) + 0x218) != 1)
                nb++;
        }
    }
    if (nb > 0) {
        if (na <= 0)
            w = (char *)ai_Unk6f0ce8;
        ret = w;
    } else
        ret = (char *)ai_Unk6f0cec;
    for (i = 0; i < ai_Unk6f0d58; i++) {
        if (strcmp(ai_Unk935980[i], "sp_CONQUER") != 0) {
            o = ai_Unk935980[i];
            d3_PointToWorld(o, o + 0x4c, &out);
            total = out + 1;
            if (*(char **)(o + 0x200) != 0 && *(int *)(*(char **)(o + 0x200) + 0x16c) == ai_Unk6f0cec)
                mine++;
        }
    }
    if (total != 0 && mine == total)
        w = ret = (char *)ai_Unk6f0ce8;
    if (ret != 0) {
        n = (unsigned char)ai_Unk6f0d04[0x30];
        for (i = 0; i < n && *(int *)(ai_Unk6f0d04 + 0x34 + i * 4) == -1; i++)
            ;
        rr = (char *)ai_Unk4cdf00(ai_Unk92cae0[i], ret);
        memcpy(sl, rr, 0x2c);
        sl[0] = *(int *)(ret + 4);
        sl[1] = 5;
        ai_Unk4c5c40(*(int *)(ai_Unk92cae0[i] + 0x10), sl);
    }
    return ret;
}
