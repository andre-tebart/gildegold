/* 0x509400, the working minutes between the clocks a and b of the building b (the day runs from the season's start to its end: 0x617830 / 0x617840 hours); the whole day (0x4fe280) for the night shift (0x6f23f8 < 0) and the kind 0x1d. ROUGH */

#define GM_CLAMPMIN(m, lo, hi) ((float)(m) < (lo) ? (lo) : ((float)(m) > (hi) ? (hi) : (float)(m)))

int gm_Unk509400(GCLOCK *a, GCLOCK *b, unsigned char *bld)
{
    float lo, hi, m, c;
    int s, acc, x, y, d0, d1, n;
    GCLOCK t;

    s = gm_ClockSeason(a);
    lo = ai_Unk617830[s];
    hi = ai_Unk617840[s];
    if ((char)ai_GameFlags < 0)
        return gm_MinutesBetween(a, b);
    if (bld != 0 && ai_BTypes[*bld * 0x24d] == 0x1d)
        return gm_MinutesBetween(a, b);
    if (gm_CompareTime(a, b) > 0)
        return 0;
    t = *a;
    d0 = a->day;
    d1 = b->day;
    if (d0 < d1) {
        m = (float)(t.hour * 60 + t.min);
        c = GM_CLAMPMIN(m, lo * 60.0f, hi * 60.0f);
        x = (int)c;
        acc = (int)(hi * 60.0f - x);
        t.day = d0 + 1;
        gm_SetTime(&t, (int)lo, 0, 0);
        d0 = t.day;
    } else
        acc = 0;
    if (d0 < d1) {
        n = d1 - d0;
        do {
            acc = (int)(acc + (hi - lo) * 60.0f);
            n--;
        } while (n != 0);
    }
    m = (float)(t.hour * 60 + t.min);
    y = (int)GM_CLAMPMIN((float)(b->hour * 60 + b->min), lo * 60.0f, hi * 60.0f);
    x = (int)GM_CLAMPMIN(m, lo * 60.0f, hi * 60.0f);
    return (acc - x) + y;
}
