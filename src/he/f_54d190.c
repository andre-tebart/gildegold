/* 0x54d190, a Ghidra miss: the init of the title event: the due clock now, +0x56 = 0x16, +0x58 = 0x1e, +0x5c / +0xac / +0xb0 = 0 */

void he_Unk54d190(int ev)
{
    *(HETIME *)(ev + 0x52) = ai_Time;
    *(short *)(ev + 0x56) = 0x16;
    *(int *)(ev + 0x58) = 0x1e;
    *(int *)(ev + 0x5c) = 0;
    *(int *)(ev + 0xac) = 0;
    *(int *)(ev + 0xb0) = 0;
}
