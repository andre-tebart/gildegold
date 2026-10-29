/* 0x571ac0, the alloc of the event 0x8f: the character +0xac missing, already with an event 0x8f (+0x17c type 0x8f) or the building 0x501190 (1, 1, +0x10) missing -> free; +0xb4 != -1 -> the group 0x4ffe90 (building +0x5d, 2, 6, 1, +0xb4); the due clock +0x52 and the start clock +0x44 = now; the character registered (0x524260) in the group (or alone without one); the second clock +0x60 = now; due now + 0x1e hours (0x4fe140); +0xb8 = the due clock and 0x4fe140 (+0xb8, 3, 0, 0) */

void he_Unk571ac0(int ev)
{
    char *per = (char *)gm_FindChar(HM_I(ev, 0xac));
    char *bld;
    int grp = 0;

    if (per == 0)
        goto free_ev;
    if (HM_I(per, 0x17c) != 0 && HM_B(HM_I(per, 0x17c), 0) == 0x8f)
        goto free_ev;
    bld = (char *)gm_BuildQuery(1, 1, HM_I(ev, 0x10));
    if (bld == 0)
        goto free_ev;
    if (HM_I(ev, 0xb4) != -1)
        grp = gm_Query(HM_I(bld, 0x5d), 2, 6, 1, HM_I(ev, 0xb4));
    *(HETIME *)(ev + 0x52) = ai_Time;
    *(HETIME *)(ev + 0x44) = ai_Time;
    if (grp != 0)
        sv_MoveToGroup(bld, grp, *(unsigned short *)per, ev);
    else
        sv_MoveToGroup(bld, 0, *(unsigned short *)per, ev);
    *(HETIME *)(ev + 0x60) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 0x1e, 0);
    *(HETIME *)(ev + 0xb8) = *(HETIME *)(ev + 0x52);
    gm_AddTime(ev + 0xb8, 3, 0, 0);
    return;
free_ev:
    he_Unk53e1f0((int *)ev);
}
