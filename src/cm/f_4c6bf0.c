/* 0x4c6bf0, handler of type 0xb: creates a character (0x507690), seeded with data[0x16] */

int ai_Unk4c6bf0(G17MSG *m, G17STAT *s)
{
    char *b;
    unsigned int k;

    if (net_Conn.s != INVALID_SOCKET)
        ai_NextObjectId = *(int *)&m->data[0];
    else
        *(int *)&m->data[0x16] = rand();
    util_Seed(*(int *)&m->data[0x16]);
    if (*(int *)&m->data[0xf] == -1) {
        b = 0;
    } else {
        *(int *)&m->data[0xf] = ai_Unk4c4450((int)m, *(int *)&m->data[0xf]);
        b = gm_BuildQuery(1, 1, *(int *)&m->data[0xf]);
        if (b == 0)
            return 1;
    }
    k = gm_Unk507690(m->data[4], *(int *)&m->data[5], *(int *)&m->data[9], *(unsigned short *)&m->data[0xd], b, m->data[0x13], m->data[0x14], m->data[0x15]);
    if (k == 0xffff)
        return 1;
    ai_Unk60bd88 = *(int *)(ai_Chars[k] + 4);
    if (s != 0) {
        s->state = 1;
        s->unk01 = 1;
        s->value = (int)ai_Chars[k];
    }
    return 0;
}
