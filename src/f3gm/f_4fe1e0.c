/* 0x4fe1e0, sets the time of day of the clock c (hour, minute, second) */

void gm_SetTime(GCLOCK *c, unsigned char hour, unsigned min, unsigned sec)
{
    c->hour = hour;
    c->sec = sec & 0xff;
    c->min = min & 0xff;
}
