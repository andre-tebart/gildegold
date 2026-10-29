/* 0x5482c0, a Ghidra miss: the start handler: the due clock +0x52 = now, posted 0x18 units ahead */

void he_Unk5482c0(int ev)
{
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0x18, 0, 0);
}
