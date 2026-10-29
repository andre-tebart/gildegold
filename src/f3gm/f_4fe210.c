/* 0x4fe210, compares two clocks: -1 when a is earlier, 1 when later, 0 when equal */

int gm_CompareTime(GCLOCK *a, GCLOCK *b)
{
    int x, y;
    int da = a->day, db = b->day;

    if (da < db)
        return -1;
    if (db < da)
        return 1;
    x = a->sec + (a->min + a->hour * 0x3c) * 0x3c;
    y = b->sec + (b->min + b->hour * 0x3c) * 0x3c;
    if (x < y)
        return -1;
    return y < x;
}
