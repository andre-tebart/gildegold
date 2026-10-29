/* 0x563070, a Ghidra miss: the allocation handler of the event 0x48: the clocks +0x52 and +0xcc are now, the run at +0xcc 0x30 units ahead and at +0x52 one unit ahead */

void he_Unk563070(int ev)
{
    *(HETIME *)(ev + 0x52) = ai_Time;
    *(HETIME *)(ev + 0xcc) = ai_Time;
    gm_AddTime(ev + 0xcc, 0, 0x30, 0);
    gm_AddTime(ev + 0x52, 0, 0, 1);
}
