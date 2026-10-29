/* 0x4cc980, handler of type 0x58: ends the group data[0] (its members' link +0x20c is cleared) */

int ai_Unk4cc980(G17MSG *m, G17STAT *s)
{
    char *q, *c;
    int i, id;

    q = cm_PartyById(G17_I(*m, 0));
    if (q == 0)
        return 1;
    for (i = 0; i < (unsigned char)q[0x30]; i++) {
        id = *(int *)(q + 0x34 + i * 4);
        if (id != -1) {
            c = gm_FindChar(id);
            if (c != 0 && *(int *)(c + 0x20c) == *(int *)q)
                *(int *)(c + 0x20c) = -1;
        }
    }
    ai_Unk4e5180(G17_I(*m, 0));
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
