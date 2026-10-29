/* 0x54da20, a Ghidra miss: the init of the talent event: +0xac = the number of the talent (0x4ac140), +0xb4 = -1, the due clock now, posted (state 0) */

void he_Unk54da20(int ev)
{
    if ((*(unsigned char *)(ev + 0x78) & 4) == 0) {
        *(unsigned short *)(ev + 0xac) = (unsigned short)hmt_RecordCount();
        *(int *)(ev + 0xb4) = -1;
        *(HETIME *)(ev + 0x52) = ai_Time;
        *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
    }
}
