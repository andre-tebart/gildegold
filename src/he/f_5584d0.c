/* 0x5584d0, the allocation handler of the group events: the +0xac persons (ids from +0x8c) that exist are sent (0x524260 (0, 0, type, ev)), the due clock is the start clock (+0x44) and the event posted 1 later (state kept) */

void he_Unk5584d0(int ev)
{
    int i, p;

    for (i = 0; i < *(unsigned char *)(ev + 0xac); i++) {
        p = gm_FindChar(*(int *)(ev + 0x8c + i * 4));
        if (p != 0)
            sv_MoveToGroup(0, 0, *(unsigned short *)p, ev);
    }
    *(HETIME *)(ev + 0x52) = *(HETIME *)(ev + 0x44);
    gm_AddTime(ev + 0x52, 0, 1, 0);
}
