/* 0x4cc270, handler of type 0x51 */

int ai_Unk4cc270(G17MSG *m, G17STAT *s)
{
    char *r;
    int i;

    if (m->data[0x30] != ai_CharInts[ai_PlayerId][0x82])
        return 1;
    for (i = 0; i < 16; i++) {
        if (*(int *)(ai_Unk92cae0[i] + 0x10) == G17_I(*m, 0))
            break;
    }
    if (i < 16) {
        r = ai_Unk4cdf00(ai_Unk92cae0[i], ai_Unk4ce130(G17_I(*m, 4)));
        if (*(int *)(r + 4) == G17_I(*m, 8)) {
            if (*(int *)(r + 4) != 2 || *(int *)(r + 0x10) == G17_I(*m, 0x14))
                memcpy(r, &m->data[4], 0x2c);
        }
    }
    if (s != 0) {
        s->state = 1;
        s->unk01 = 8;
        s->value = 0;
    }
    return 0;
}
