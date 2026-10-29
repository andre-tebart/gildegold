/* 0x4fe350, the season of the clock c (day % 4) */

int gm_ClockSeason(GCLOCK *c)
{
    return c->day % 4;
}
