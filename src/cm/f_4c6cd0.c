/* 0x4c6cd0, handler of type 0xc: creates a named character (0x507690), names it (data[0x15], data[0x25]) */

int ai_Unk4c6cd0(G17MSG *m, G17STAT *s)
{
    char *c, *t, *r;
    unsigned int k;

    if (net_Conn.s != INVALID_SOCKET)
        ai_NextObjectId = *(int *)&m->data[0];
    else
        *(int *)&m->data[0x35] = rand();
    util_Seed(*(int *)&m->data[0x12]);
    k = gm_Unk507690(s != 0 ? 6 : 7, *(int *)&m->data[4], *(int *)&m->data[8], *(unsigned short *)&m->data[0xc], 0, m->data[0xe], 0, m->data[0x13]);
    if (k == 0xffff)
        return 1;
    c = ai_Chars[k];
    ai_Unk60bd88 = *(int *)(c + 4);
    strncpy(c + 0x30, (char *)&m->data[0x15], 0x10);
    t = gm_PlayerRecord(c);
    if (t != 0) {
        *(short *)t = *(short *)(c + 0x50);
        strncpy(t + 2, (char *)&m->data[0x25], 0x10);
        strncpy(c + 0x40, (char *)&m->data[0x25], 0x10);
        r = gm_FindChar(*(int *)(c + 0x60));
        if (r != 0)
            strncpy(r + 0x40, (char *)&m->data[0x25], 0x10);
        *(float *)(t + 0x88) = -1.0f;
    }
    *(int *)(c + 0x54) = *(int *)&m->data[0xf];
    c[0xc] = m->data[0x14];
    if (s != 0) {
        s->state = 1;
        s->unk01 = 1;
        s->value = (int)c;
    }
    return 0;
}
