/* 0x525a00, fills the day phase table 0x623ba9 (6 rows of 10 bytes: +0 hours, +1 minutes, +2 the seconds of day) from the minutes of the phase table 0x5eeab8 of the season k (0x4fe350) */

void sv_Unk525a00(int t)
{
    int k;
    char *p;
    int *q;
    char h, m;

    k = gm_ClockSeason(t);
    p = ai_Unk623ba9;
    q = ai_Unk5eeab8 + k * 6;
    do {
        h = (char)(*q / 0x3c);
        p[-1] = h;
        m = (char)*q + h * -0x3c;
        *p = m;
        *(int *)(p + 1) = (m + h * 0x3c) * 0x3c;
        p += 10;
        q++;
    } while ((int)p < ((unsigned)((unsigned)ai_Unk623ba9 + 0x1) + 0x3b));
}
