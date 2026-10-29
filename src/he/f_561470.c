/* 0x561470, the allocation handler: the due clock is now + a random 0x42ec00(0x1e) units */

void he_Unk561470(int ev)
{
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 1, 0, (unsigned short)util_Random(0x1e) - 0x14);
}
