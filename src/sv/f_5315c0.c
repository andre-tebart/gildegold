/* 0x5315c0, a callback of 0x42ea10 (the screens' wait loops): while the clock is shown by a game speed (0x6f3608: seconds = (speed * 0.00625 + 0.5) * 0x6248b4) it stores that time of day (hours / minutes /
   seconds) in the clock record 0x75e130 (0x4fe140); skipped in the day phase 0x17 and later unless the flag bit 7 of 0x6f23f8 is set; a start Ghidra merged into 0x5309a0 */

void sv_UpdateClock(void)
{
    int t, h, m;

    if ((SV_US((unsigned)&ai_Unk75e134) >= 0x17 || SV_US((unsigned)&ai_TimeHour) >= 0x17) && (SV_B((unsigned)&ai_GameFlags) & 0x80) == 0)
        return;
    t = (int)(((float)SV_I((unsigned)&ai_OptionSpeed) * 0.00625f + 0.5) * SV_I((unsigned)&full_data_6248b4));
    h = t / 3600;
    m = (t - h * 3600) / 60;
    gm_AddTime((unsigned)&ai_Unk75e130, h, m, t - (m + h * 60) * 60);
}
