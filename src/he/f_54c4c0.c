/* 0x54c4c0, a Ghidra miss: the init of the event: the due clock now with +0x56 = 0xa, +0x58 / +0x5c / +0xac / +0xb0 / +0xb4 = 0 */

void he_Unk54c4c0(int ev)
{
    *(HETIME *)(ev + 0x52) = ai_Time;
    *(short *)(ev + 0x56) = 0xa;
    *(int *)(ev + 0x58) = 0;
    *(int *)(ev + 0x5c) = 0;
    *(int *)(ev + 0xac) = 0;
    *(int *)(ev + 0xb0) = 0;
    *(int *)(ev + 0xb4) = 0;
}
