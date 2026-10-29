/* 0x542490, a Ghidra miss (disassembled): the event with the building (+0x10): sets the clock (+0x52 + 0x18, state word 7 at +0x56), posts it again when the building is gone; else with no record 0x1b5 in the building sends 0x4c50b0 (a 7 byte value of -1 passed by value, here only approximated) and posts the event (ROUGH) */

void ai_Unk4c50b0(int a, int b, short c);

void he_Unk542490(int ev)
{
    int bld = 0;
    int r;

    if ((*(unsigned char *)(ev + 0x78) & 4) == 0) {
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0x18, 0, 0);
        *(short *)(ev + 0x56) = 7;
        *(int *)(ev + 0x58) = 0;
        gm_FindById(&bld, 0, 0, *(int *)(ev + 0x10));
        if (bld == 0) {
            cm_SendExSetHE(-1, ev);
            return;
        }
        r = gm_Query(*(int *)(bld + 0x5d), 1, 0, 0x1b5);
        if (r == 0)
            ((void (*)(int, int))ai_Unk4c50b0)(*(int *)(bld + 1), -1);
        *(int *)(ev + 0x84) = -1;
        cm_SendExSetHE(0, ev);
    }
}
