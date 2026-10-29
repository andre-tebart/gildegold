/* 0x4fe280, the minutes from clock a to clock b (days, hours and minutes; the seconds are not used) */

int gm_MinutesBetween(GCLOCK *a, GCLOCK *b)
{
    return ((((b->day - a->day) * 0x18 - a->hour) + b->hour) * 0x3c - a->min) + b->min;
}
