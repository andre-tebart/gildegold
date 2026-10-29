/* 0x4fe310, a forward to 0x4fe140 with the first two arguments left out (the second is unused) */

void gm_AddTime2(GCLOCK *c, int unused, int hours, int mins, int secs)
{
    gm_AddTime(c, hours, mins, secs);
}
