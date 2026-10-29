/* 0x548ef0, a Ghidra miss: the start handler: the due clock +0x52 = now, posted 0x60 units ahead */

void he_Unk548ef0(int ev)
{
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0x60, 0, 0);
}
