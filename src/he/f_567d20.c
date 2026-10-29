/* 0x567d20, a Ghidra miss: the allocation handler of the tutorial event 0x85: the first run is now + 7 units (+0x56 = 7, +0x58 = 0) */

void he_Unk567d20(int ev)
{
    *(HETIME *)(ev + 0x52) = ai_Time;
    *(short *)(ev + 0x56) = 7;
    *(int *)(ev + 0x58) = 0;
}
