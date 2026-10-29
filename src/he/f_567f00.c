/* 0x567f00, a Ghidra miss: the allocation handler of the event 0xa6: the first run is now + 6 units, +0x84 = -1 */

void he_Unk567f00(int ev)
{
    *(HETIME *)(ev + 0x52) = ai_Time;
    *(int *)(ev + 0x84) = -1;
    *(short *)(ev + 0x56) = 6;
    *(int *)(ev + 0x58) = 0;
}
