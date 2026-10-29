/* 0x54ef70, a Ghidra miss (disassembled): the init of the "Dunkle Ecke" (dark corner, the robbery) event: +0xac = -1, +0xc4 = a random building index, +0xc8 = a random step of the table 0x5ed140, +0xc0 = 0, +0xb8 = -1, +0xbc = 5 + random(4); the clocks (+0x52 / +0x44) are now and posted 1 unit ahead; when a running event of the type 0x74 (0x53e400) of another player has the same victim +0xb0 the event ends; else the victim (0x4fea50) must have a record of the building (0x4ffe90 2 / 6 / 0x12c): its job table gets the time (+0x28 + 0x4c3c00) and a carrier (0x4c59f0 (0xe, .., 0x12, .. "DunkleEcke")) is created (+0xac) (ROUGH) */

void he_Unk54ef70(int ev)
{
    int e, o = 0, b, h;
    HETIME tm;

    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    *(int *)(ev + 0xac) = -1;
    *(int *)(ev + 0xc4) = (unsigned short)util_Random(0x100);
    *(int *)(ev + 0xc8) = U_I((unsigned)&ai_Unk5ed140 + (unsigned short)util_Random(0x10) * 4);
    *(int *)(ev + 0xc0) = 0;
    *(int *)(ev + 0xb8) = -1;
    *(int *)(ev + 0xbc) = (unsigned short)util_Random(4) + 5;
    *(HETIME *)(ev + 0x52) = ai_Time;
    *(HETIME *)(ev + 0x44) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 1, 0);
    e = (int)he_FindEvent(1, 0, 0x74);
    while (e != 0) {
        if (*(int *)(e + 4) == *(int *)(ev + 4) || *(int *)(e + 0xb0) != *(int *)(ev + 0xb0))
            e = (int)he_NextEvent();
        else {
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
    }
    gm_FindById(&o, 0, 0, *(int *)(ev + 0xb0));
    if (o == 0) {
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    }
    b = gm_Query(*(int *)(o + 0x5d), 2, 6, 0x12c);
    if (b == 0) {
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    }
    tm = ai_Time;
    cm_BeginFieldBlock(b, *(int *)(b + 2));
    cm_AddFieldDelta(4, 1, (b - U_I((unsigned)&cm_FieldObject)) + 0x28, &tm);
    cm_SendFieldDeltas();
    cm_BeginGroup("Dunkle Ecke");
    h = ((int (*)(int, int, int, int, int, int, const char *))ai_Unk4c59f0)(0xe, *(int *)(o + 1), -1, 0, 0, 0, "DunkleEcke");
    *(int *)(ev + 0xac) = h;
    *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
    cm_EndGroup("Dunkle Ecke");
}
