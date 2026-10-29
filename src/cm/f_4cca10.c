/* 0x4cca10, handler of type 0x59: the group data[0] gets the 16 members data[4..] */

int ai_Unk4cca10(G17MSG *m, G17STAT *s)
{
    char *q, *c;
    int i;

    if (ai_Unk4c69c0(m) != 0) {
        if (s != 0)
            s->state = 2;
        return 1;
    }
    if (s != 0)
        s->state = 2;
    q = cm_PartyById(G17_I(*m, 0));
    if (q != 0)
        *(int *)(q + 0x28) = 1;
    for (i = 0; i < 16; i++) {
        c = gm_FindChar(*(int *)&m->data[4 + i * 4]);
        if (c != 0)
            *(int *)(c + 0x20c) = G17_I(*m, 0);
    }
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
