/* 0x571550, the alloc of the event 0x8d: the character +0x8c or the building 0x502020 (+0x10) missing -> free; the group 0x4ffe90 (building +0x5d, 1, 0, 0xfd), the character registered (0x524260), +0xac = 0, +0xb8 = 0x1fe, the start clock +0xbc and the due clock +0x52 = now + 5 hours, state 0 */

void he_Unk571550(int ev)
{
    char *per = (char *)gm_FindChar(HM_I(ev, 0x8c));
    char *bld;
    int grp;

    if (per == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    bld = (char *)gm_FindBuildingById(HM_I(ev, 0x10));
    if (bld == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    grp = gm_Query(HM_I(bld, 0x5d), 1, 0, 0xfd);
    sv_MoveToGroup(bld, grp, *(unsigned short *)per, ev);
    HM_I(ev, 0xac) = 0;
    HM_I(ev, 0xb8) = 0x1fe;
    *(HETIME *)(ev + 0xbc) = ai_Time;
    *(HETIME *)(ev + 0x52) = ai_Time;
    HM_I(ev, 0x70) = 0;
    gm_AddTime(ev + 0x52, 0, 5, 0);
}
