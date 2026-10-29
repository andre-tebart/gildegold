/* 0x5096a0, the same with the fixed day 360 .. 1380 minutes (6:00 to 23:00, 1020 a day) */

int gm_Unk5096a0(GCLOCK *a, GCLOCK *b)
{
    float m;
    int acc, x, y, d0, d1, n;
    GCLOCK t;

    if (gm_CompareTime(a, b) > 0)
        return 0;
    t = *a;
    d0 = a->day;
    d1 = b->day;
    if (d0 < d1) {
        m = (float)(t.hour * 60 + t.min);
        x = (int)GM_CLAMPMIN(m, 360.0f, 1380.0f);
        acc = (int)(1380.0f - x);
        t.day = d0 + 1;
        gm_SetTime(&t, 6, 0, 0);
        d0 = t.day;
    } else
        acc = 0;
    if (d0 < d1) {
        n = d1 - d0;
        do {
            acc = (int)(acc + 1020.0f);
            n--;
        } while (n != 0);
    }
    y = (int)GM_CLAMPMIN((float)(b->hour * 60 + b->min), 360.0f, 1380.0f);
    m = (float)(t.hour * 60 + t.min);
    x = (int)GM_CLAMPMIN(m, 360.0f, 1380.0f);
    return (acc - x) + y;
}
