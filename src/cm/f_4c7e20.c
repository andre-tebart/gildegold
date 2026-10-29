/* 0x4c7e20, handler of type 0x15: one item data[4] for data[0], with 0x1f bytes of data[6] */

int ai_Unk4c7e20(G17MSG *m, G17STAT *s)
{
    char *o;

    if (net_Conn.s != INVALID_SOCKET)
        ai_NextObjectId = *(int *)&m->data[0x25];
    *(int *)&m->data[0] = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
    o = gm_CreateObject(*(int *)&m->data[0], *(unsigned short *)&m->data[4], 1);
    if (o == 0)
        return 1;
    ai_Unk60bd90 = *(int *)(o + 2);
    memcpy(o + 0x1c, &m->data[6], 0x1f);
    if (s != 0)
        s->state = 1;
    return 0;
}
