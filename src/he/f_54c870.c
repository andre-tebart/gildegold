/* 0x54c870, a Ghidra miss: the init of the ranking event: the due clock now, +0x56 = 9, +0x58 = 0, posted 0x18 units ahead (state 0) */

void he_Unk54c870(int ev)
{
    if ((*(unsigned char *)(ev + 0x78) & 4) == 0) {
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0x18, 0, 0);
        *(short *)(ev + 0x56) = 9;
        *(int *)(ev + 0x58) = 0;
        *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
    }
}
