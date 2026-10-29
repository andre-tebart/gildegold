/* 0x570ef0, the alloc of the event 0x8c: the building 0x501190 (1, 1, ev + 0x10), the group 0x4ffe90 (building +0x5d, 2, 0, 0xfd, 6); each of the four characters +0x8c .. registered (0x524260); none -> free; else +0xb0 = 0, the start clock +0xb8 = now, +0xb4 = 0x168, the due clock now + 5 hours, state 0 */

void he_Unk570ef0(int ev)
{
    int bld = gm_BuildQuery(1, 1, HM_I(ev, 0x10));
    int grp = gm_Query(HM_I(bld, 0x5d), 2, 0, 0xfd, 6);
    int i, n = 0, r;

    for (i = 0; i < 4; i++) {
        r = gm_FindChar(HM_I(ev, 0x8c + i * 4));
        if (r != 0) {
            sv_MoveToGroup(bld, grp, *(unsigned short *)r, ev);
            n++;
        }
    }
    if (n == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    HM_I(ev, 0xb0) = 0;
    *(HETIME *)(ev + 0xb8) = ai_Time;
    HM_I(ev, 0xb4) = 0x168;
    *(HETIME *)(ev + 0x52) = ai_Time;
    HM_I(ev, 0x70) = 0;
    gm_AddTime(ev + 0x52, 0, 5, 0);
}
