/* 0x4cc920, handler of type 0x57: frees the record +0x184 of character data[0] */

int ai_Unk4cc920(G17MSG *m, G17STAT *s)
{
    char *c;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    c = gm_FindChar(G17_I(*m, 0));
    if (c != 0 && *(int *)(c + 0x184) != 0) {
        ch_Delete(*(int *)(c + 0x184));
        *(int *)(c + 0x184) = 0;
        if (s != 0)
            s->state = 1;
        return 0;
    }
    return 1;
}
