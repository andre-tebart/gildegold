/* 0x549040, a Ghidra miss: the due clock +0x52 = now, posted 5 units ahead */

void he_Unk549040(int ev)
{
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 5, 0);
}
