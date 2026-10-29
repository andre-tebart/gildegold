/* 0x5483a0, the init of a timed event: +0xb0 = -1, the clock +0x52 = the start (+0x44) + random(4) + 2 in the minute word (+0x56), state 0 */

void he_Unk5483a0(int ev)
{
    *(int *)(ev + 0xb0) = -1;
    *(HETIME *)(ev + 0x52) = *(HETIME *)(ev + 0x44);
    *(short *)(ev + 0x56) = *(short *)(ev + 0x56) + (short)util_Random(4) + 2;
    *(int *)(ev + 0x70) = 0;
}
