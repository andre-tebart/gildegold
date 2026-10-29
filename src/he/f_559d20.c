/* 0x559d20, the allocation handler of the espionage event "Spionage" (ROUGH): the spy +0xac / +0xb0 is checked (0x505df0, flag 4 of +0x78), the target building is rolled (random 0x100, 0x42ec00 twice), the records of the type 0x35-like list (0x53e400 / 0x53e300) are searched for another spy on the same target (+0xd8, +8), the target person is looked up (0x4fea50, 0x4a63f0 (.., 100.0)) and the event posted (0x4c4c60); a spy of kind 6 / 7 gets the message record (0x4c4540 "Spionage", the message 0x4c59f0 "SPIONAGE" (0x62751c) and 0x4c4580), the handle goes to +0x84 */

void he_Unk559d20(int ev)
{
    int p, o = 0, e, n;

    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    p = gm_FindChar(*(int *)(ev + 0xb0));
    if (p == 0) {
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    }
    n = (unsigned short)util_Random(0x100);
    (void)n;
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 0, 1);
    for (e = (int)he_FindEvent(1, 0, 0x4e); e != 0; e = (int)he_NextEvent()) {
        if (*(unsigned short *)(e + 8) == *(unsigned short *)(ev + 8) && *(int *)(e + 0xd8) == *(int *)(ev + 0xd8))
            break;
    }
    gm_FindById(&o, 0, 0, *(int *)(ev + 0xac));
    if (o == 0) {
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    }
    if (ord_DispatchOrder(0x26, (char *)ev, &o) == 2 || ((int (*)(void *))cm_BeginGroup)("Spionage") == 0) {
        ai_Unk4c59f0(0, *(int *)(ev + 0x10), -1, 0, 1, 0x174, "SPIONAGE");
        *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
    } else {
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
    }
    cm_EndGroup("Spionage");
}
