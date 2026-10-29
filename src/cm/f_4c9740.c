/* 0x4c9740, handler of type 0x2d: creates an object of kind 0x1b5 (ids -2 / -3 / -4 resolved) */

int ai_Unk4c9740(G17MSG *m, G17STAT *s)
{
    char *o;
    int k;

    if (ai_Unk1460ae0 != -1)
        ai_NextObjectId = G17_I(*m, 0);
    k = ai_Unk4c4450((int)m, G17_I(*m, 4));
    G17_I(*m, 4) = k;
    o = gm_CreateObject(k, 0x1b5, 1);
    if (o == 0)
        return 1;
    ai_Unk60bd90 = *(int *)(o + 2);
    o[0x12] = 0x40;
    *(G17B7 *)(o + 0x1c) = *(G17B7 *)&m->data[8];
    o[0x22] = 0;
    if (s != 0) {
        s->state = 1;
        s->unk01 = 3;
        s->value = (int)o;
    }
    return 0;
}
