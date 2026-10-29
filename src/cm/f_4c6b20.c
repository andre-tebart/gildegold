/* 0x4c6b20, handler of type 10: creates a building of kind data[4] for the character data[6] */

int ai_Unk4c6b20(G17MSG *m, G17STAT *s)
{
    char *b;
    unsigned short k;
    int id;

    if (net_Conn.s != INVALID_SOCKET)
        ai_NextObjectId = *(int *)&m->data[0];
    if (*(int *)&m->data[6] == -1) {
        k = 0xffff;
    } else {
        id = ai_Unk4c4450((int)m, *(int *)&m->data[6]);
        *(int *)&m->data[6] = id;
        for (k = 0; k < 0x300; k++)
            if (*(unsigned short *)ai_Chars[k] != 0xffff && *(int *)(ai_Chars[k] + 4) == id)
                break;
        if (k >= 0x300)
            return 1;
    }
    b = gm_CreateBuilding(m->data[4], k);
    if (b == 0)
        return 1;
    ai_Unk60bd8c = *(int *)(b + 1);
    *(short *)(b + 0x5a) = *(short *)&m->data[0xa];
    if (*(int *)&m->data[0xc] != 0)
        memcpy(b + 0x65, &m->data[0x10], 0x30);
    if (s != 0) {
        s->state = 1;
        s->unk01 = 2;
        s->value = (int)b;
    }
    return 0;
}
