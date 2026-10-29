/* 0x4c6f60, handler of type 0xf: moves n of the player good k from a to b (checked) */

int ai_Unk4c6f60(G17MSG *m, G17STAT *s)
{
    if (net_Conn.s != INVALID_SOCKET)
        ai_NextObjectId = *(int *)&m->data[8];
    if (*(int *)&m->data[4] != -1) {
        *(int *)&m->data[4] = ai_Unk4c4450((int)m, *(int *)&m->data[4]);
        if (gm_TakeObjekt(*(int *)&m->data[4], *(unsigned short *)(ai_PlayerSlots[m->data[0xc]] + 0x54), *(int *)&m->data[0xd]) == 0)
            return 1;
    }
    if (*(int *)&m->data[0] != -1) {
        *(int *)&m->data[0] = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
        if (gm_AddObjekt(*(int *)&m->data[0], *(unsigned short *)(ai_PlayerSlots[m->data[0xc]] + 0x54), *(int *)&m->data[0xd]) == 0)
            return 1;
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
