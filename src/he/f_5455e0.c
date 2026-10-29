/* 0x5455e0, a Ghidra miss (disassembled): sets the due clock (+0x52) to now with a random hour (+0x56 = 10 + random(10)) and minute (+0x58 = random(0x3b)), +0x5c = 0 */

void he_Unk5455e0(int ev)
{
    *(HETIME *)(ev + 0x52) = ai_Time;
    *(short *)(ev + 0x56) = (short)(util_Random(10) + 10);
    *(int *)(ev + 0x58) = (unsigned short)util_Random(0x3b);
    *(int *)(ev + 0x5c) = 0;
}
