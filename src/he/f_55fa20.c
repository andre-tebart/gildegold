/* 0x55fa20, the allocation handler of the market event: the due clock is now (+0x52, 0x4fe1e0 sets the minute word 5) and the event posted (0x4fe140 (.., 0, 0, 1)) */

void he_Unk55fa20(int ev)
{
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_SetTime(ev + 0x52, 5, 0, 0);
    gm_AddTime(ev + 0x52, 1, 0, 0);
}
