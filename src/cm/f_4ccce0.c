/* 0x4ccce0, handler of type 0x5e: the job record 0xca of building data[4] (for character data[0x16]) is set */

int ai_Unk4ccce0(G17MSG *m, G17STAT *s)
{
    char *a, *b, *q;
    int v;

    if (ai_Unk1460ae0 != -1)
        ai_NextObjectId = G17_I(*m, 0);
    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    a = gm_FindBuildingById(G17_I(*m, 0x16));
    if (a == 0)
        return 1;
    b = gm_FindBuildingById(G17_I(*m, 4));
    if (b == 0)
        return 1;
    q = gm_Query(*(int *)(b + 0x5d), 1, 0, 0xca);
    while (q != 0) {
        if (*(int *)(q + 0x2a) == *(int *)(a + 1))
            goto found;
        q = gm_QueryNext();
    }
    q = gm_CreateObject(*(int *)(b + 1), 0xca, 1);
    if (q == 0)
        return 1;
    ai_Unk60bd90 = *(int *)(q + 2);
found:
    *(G17T14 *)(q + 0x1c) = *(G17T14 *)&m->data[8];
    *(int *)(q + 0x2a) = G17_I(*m, 0x16);
    *(int *)(q + 0x32) = G17_I(*m, 0x1e);
    q[0x36] = m->data[0x22];
    q[0x37] = q[0x37] + m->data[0x23];
    if ((unsigned char)q[0x37] > 0x64)
        v = 0x64;
    else
        v = (unsigned char)q[0x37];
    q[0x37] = v;
    if (s != 0)
        s->state = 1;
    return 0;
}
