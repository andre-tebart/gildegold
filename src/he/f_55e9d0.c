/* 0x55e9d0, a Ghidra miss: the allocation handler of the daily routine event 0x3a: +0xb0 = 3, +0xac = 0 (the routine step), the clocks +0x52 / +0x44 are now, the minute word is set to 6:15 (0x4fe1e0 (+0x52, 6, 0xf, 0)) */

int init_ObjectSceneUniverse();

void he_Unk55e9d0(int ev)
{
    gm_ClockSeason((unsigned)&ai_Time);
    *(int *)(ev + 0xb0) = 3;
    *(int *)(ev + 0xac) = 0;
    *(HETIME *)(ev + 0x52) = ai_Time;
    *(HETIME *)(ev + 0x44) = ai_Time;
    gm_SetTime(ev + 0x52, 6, 0xf, 0);
}
