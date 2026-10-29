/* 0x4cc390, handler of type 0x53: the sub-commands 'recl' (reset the slots of the job 0x12c of building data[8]) and
   'buy ' (character data[4] takes one of its three slots) */

int ai_Unk4cc390(G17MSG *m, G17STAT *s)
{
    char *b, *c, *q;
    int i, k;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    if (G17_I(*m, 0) == 0x636c6572) {
        b = gm_BuildQuery(1, 1, G17_I(*m, 8));
        if (b == 0)
            return 1;
        q = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x12c);
        if (q == 0)
            return 1;
        if (*(int *)ai_Time - *(int *)(q + 0x28) <= 1)
            return 1;
        *(int *)(q + 0x1c) = -1;
        *(int *)(q + 0x20) = -1;
        *(int *)(q + 0x24) = -1;
        *(int *)(q + 0x28) = *(int *)ai_Time - 1;
    } else if (G17_I(*m, 0) == 0x62757920) {
        c = gm_FindChar(G17_I(*m, 4));
        if (c == 0)
            return 1;
        b = gm_BuildQuery(1, 1, G17_I(*m, 8));
        if (b == 0)
            return 1;
        q = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x12c);
        if (q == 0)
            return 1;
        k = G17_I(*m, 0xc);
        for (i = 0; i < 3; i++) {
            if (*(int *)(q + 0x1c + i * 4) == k)
                break;
        }
        if (i >= 3)
            return 1;
        *(int *)(q + 0x1c + i * 4) = -1;
        if (ai_PlayerId == *(unsigned short *)(b + 0x27))
            ai_Unk5a8c50(c, ai_Chars[ai_PlayerId], b, G17_I(*m, 0x10));
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
