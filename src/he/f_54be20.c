/* 0x54be20, a Ghidra miss: the init of the patrol-meeting event: +0xd4 = -1, +0xdc = 0, the start (+0x44) and the due clock (+0x52) = now, posted at once (0x4fe140 (.., 0, 1, 0), state 0) */


void he_Unk54be20(int ev)
{
    if ((*(unsigned char *)(ev + 0x78) & 4) == 0) {
        *(int *)(ev + 0xd4) = -1;
        *(unsigned char *)(ev + 0xdc) = 0;
        *(HETIME *)(ev + 0x44) = ai_Time;
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 1, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
    }
}
