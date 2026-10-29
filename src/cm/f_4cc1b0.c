/* 0x4cc1b0, handler of type 0x50: the cutscene record data[0] (when it is our player's) gets the entry data[4] */

int ai_Unk4cc1b0(G17MSG *m, G17STAT *s)
{
    char *rec, *c, *r;
    int i;

    if (m->data[0x30] != ai_CharInts[ai_PlayerId][0x82])
        return 1;
    for (i = 0; i < 16; i++) {
        if (*(int *)(ai_Unk92cae0[i] + 0x10) == G17_I(*m, 0))
            break;
    }
    if (i < 16) {
        rec = ai_Unk92cae0[i];
        c = ai_Unk4ce130(G17_I(*m, 4));
        if (c == 0)
            c = gm_FindChar(G17_I(*m, 4));
        if (c != 0) {
            r = ai_Unk4cdf00(rec, c);
            memcpy(r, &m->data[4], 0x2c);
            if (*(int *)(r + 4) == 0)
                *(int *)r = -1;
        }
    }
    if (s != 0) {
        s->state = 1;
        s->unk01 = 8;
        s->value = 0;
    }
    return 0;
}
