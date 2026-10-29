/* 0x4fe2c0, converts the clock c into the calendar record out: byte 0 = 1, byte 1 = season (day % 4) * 3 + 1, word at +2 = year (day + 0x578), byte 4 = hour, byte 5 = minute, int at +6 = the seconds field */

void gm_ClockToDate(GCLOCK *c, unsigned char *out)
{
    int r;

    *(short *)(out + 2) = (short)c->day + 0x578;
    r = c->day % 4;
    out[0] = 1;
    out[1] = (char)r * 3 + 1;
    out[4] = (char)c->hour;
    out[5] = (char)c->min;
    *(int *)(out + 6) = c->sec;
}
