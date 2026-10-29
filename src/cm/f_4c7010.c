/* 0x4c7010, handler of type 0x10: as 0xf, unchecked (0x500ae0) */

int ai_Unk4c7010(G17MSG *m, G17STAT *s)
{
    if (net_Conn.s != INVALID_SOCKET)
        ai_NextObjectId = *(int *)&m->data[8];
    if (*(int *)&m->data[4] != -1) {
        *(int *)&m->data[4] = ai_Unk4c4450((int)m, *(int *)&m->data[4]);
        gm_AddNegative(*(int *)&m->data[4], *(unsigned short *)(ai_PlayerSlots[m->data[0xc]] + 0x54), *(int *)&m->data[0xd]);
    }
    if (*(int *)&m->data[0] != -1) {
        *(int *)&m->data[0] = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
        gm_AddObjekt(*(int *)&m->data[0], *(unsigned short *)(ai_PlayerSlots[m->data[0xc]] + 0x54), *(int *)&m->data[0xd]);
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
