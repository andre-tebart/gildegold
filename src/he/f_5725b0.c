/* 0x5725b0, the run of the event 0x90 (the meeting): the states -2 / -1 free (and continue, as in the original); paused -> nothing; the due clock +0x52 = now; the building 0x501190 (1, 1, +0xac) missing -> 0x4c4c60 (-1); state 0 with +0xb8 < 4: 0x572310 (building, +0xb0, +0xb4, +0xb8), due now + 0x1e hours, +0xb8 + 1, 0x4c4c60 (0); else state -1, 0x4fe140 (+0x52, 0x16, 0, 0) and 0x4c4c60 (-1) */

void he_Unk5725b0(int ev)
{
    char *bld;

    if (HM_I(ev, 0x70) == -2 || HM_I(ev, 0x70) == -1)
        he_Unk53e1f0((int *)ev);
    if (*(unsigned char *)(ev + 0x78) & 4)
        return;
    *(HETIME *)(ev + 0x52) = ai_Time;
    bld = (char *)gm_BuildQuery(1, 1, HM_I(ev, 0xac));
    if (bld == 0) {
        cm_SendExSetHE(-1, ev);
        return;
    }
    if (HM_I(ev, 0x70) != 0)
        return;
    if (HM_I(ev, 0xb8) < 4) {
        he_Unk572310((int)bld, HM_I(ev, 0xb0), HM_I(ev, 0xb4), HM_I(ev, 0xb8));
        gm_AddTime(ev + 0x52, 0, 0x1e, 0);
        HM_I(ev, 0xb8) = HM_I(ev, 0xb8) + 1;
        cm_SendExSetHE(0, ev);
        return;
    }
    HM_I(ev, 0x70) = -1;
    gm_AddTime(ev + 0x52, 0x16, 0, 0);
    cm_SendExSetHE(-1, ev);
}
