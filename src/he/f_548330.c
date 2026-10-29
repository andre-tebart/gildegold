/* 0x548330, a Ghidra miss: +0xb0 = +0xac + 1, the due clock now, posted 0x18 units ahead */

void he_Unk548330(int ev)
{
    *(HETIME *)(ev + 0x52) = ai_Time;
    *(int *)(ev + 0xb0) = *(int *)(ev + 0xac) + 1;
    gm_AddTime(ev + 0x52, 0x18, 0, 0);
}
