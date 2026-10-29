/* 0x567620, a Ghidra miss: the allocation handler of a periodic check, the next run is 20 units ahead */

void he_Unk567620(int ev)
{
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 0x14, 0);
}
