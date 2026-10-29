/* 0x561130, the allocation handler: the due clock and the clock +0xb4 are now (copy of the game clock twice) */

void he_Unk561130(int ev)
{
    *(HETIME *)(ev + 0x52) = ai_Time;
    *(HETIME *)(ev + 0xb4) = ai_Time;
}
