/* 0x4cc510, handler of type 0x54: the sub-commands 'join' / 'vael' (leave) / 'recl' of the four slots of the job
   0x12d of building data[8] (character data[4]) */

int ai_Unk4cc510(G17MSG *m, G17STAT *s)
{
    char *b, *q, *ch;
    int i, j, id;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    if (G17_I(*m, 0) == 0x6a6f696e) {
        b = gm_BuildQuery(1, 1, G17_I(*m, 8));
        if (b == 0)
            return 1;
        q = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x12d);
        if (q == 0)
            return 1;
        for (i = 0; i < 4; i++) {
            if (*(int *)(q + 0x1c + i * 4) == G17_I(*m, 4))
                return 1;
        }
        for (j = 0; j < 4; j++) {
            if (*(int *)(q + 0x1c + j * 4) == -1)
                break;
        }
        if (j >= 4)
            return 1;
        if (s != 0) {
            s->state = 1;
            he_Unk541b40(G17_I(*m, 0), b, q, G17_I(*m, 4));
        }
        *(int *)(q + 0x1c + j * 4) = G17_I(*m, 4);
        return 0;
    }
    if (G17_I(*m, 0) == 0x6c656176) {
        b = gm_BuildQuery(1, 1, G17_I(*m, 8));
        if (b == 0)
            return 1;
        q = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x12d);
        if (q == 0)
            return 1;
        for (j = 0; j < 4; j++) {
            if (*(int *)(q + 0x1c + j * 4) == G17_I(*m, 4))
                break;
        }
        if (j >= 4)
            return 1;
        if (s != 0) {
            s->state = 1;
            he_Unk541b40(G17_I(*m, 0), b, q, G17_I(*m, 4));
        }
        *(int *)(q + 0x1c + j * 4) = -1;
        return 0;
    }
    if (G17_I(*m, 0) == 0x636c6572) {
        b = gm_BuildQuery(1, 1, G17_I(*m, 8));
        if (b == 0)
            return 1;
        id = *(unsigned short *)(b + 0x27);
        if (id != 0xffff) {
            ch = ai_Chars[id];
            if (ch != 0 && ch != ai_PlayerCharC && ch[8] != 0 && ch[2] != 0xf)
                return 1;
        }
        q = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x12d);
        if (q == 0)
            return 1;
        if (s != 0) {
            s->state = 1;
            he_Unk541b40(G17_I(*m, 0), b, q, G17_I(*m, 4));
        }
        for (j = 0; j < 4; j++) {
            if (*(int *)(q + 0x1c + j * 4) == G17_I(*m, 4))
                *(int *)(q + 0x1c + j * 4) = -1;
        }
        return 0;
    }
    return 1;
}
