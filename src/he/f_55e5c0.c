/* 0x55e5c0, the allocation handler: due clock now + 4 hours (0x4fe140 (clock, 0, 0, 4)) */

void he_Unk55e5c0(int ev)
{
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 4, 0, 0);
}
