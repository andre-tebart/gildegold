/* 0x552c30, the allocation handler of an event: state 0, due clock (+0x52) and the clock +0xac set to now */

void he_Unk552c30(int ev)
{
    *(HETIME *)(ev + 0x52) = ai_Time;
    *(HETIME *)(ev + 0xac) = ai_Time;
    *(int *)(ev + 0x70) = 0;
}
