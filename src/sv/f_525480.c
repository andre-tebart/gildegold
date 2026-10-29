/* 0x525480, the season of the clock/sky record t, forwarded to gm_ClockSeason (0x4fe350) as a callback */

int sv_ClockSeason(void *t)
{
    return gm_ClockSeason(t);
}
