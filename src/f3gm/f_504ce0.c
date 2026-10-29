/* 0x504ce0, the number of titles left in the group of the title number n: the end of its group of six minus n (n in 1 .. 6: 7 - n, 7 .. 12: 13 - n, ..), 0 for others */


int gm_TitlesLeft(int n)
{
    int r;

    r = 0;
    if (0 < n && n < 7)
        return 7 - n;
    if (6 < n && n < 0xd)
        return 0xd - n;
    if (0x21 < n && n < 0x28)
        return 0x28 - n;
    if (0x27 < n && n < 0x2e)
        return 0x2e - n;
    if (0x2d < n && n < 0x34)
        return 0x34 - n;
    if (0x39 < n && n < 0x40)
        return 0x40 - n;
    if (0x3f < n && n < 0x46)
        return 0x46 - n;
    if (0x45 < n && n < 0x4c)
        return 0x4c - n;
    if (0xc < n && n < 0x13)
        return 0x13 - n;
    if (0x33 < n && n < 0x3a)
        return 0x3a - n;
    if (0x18 < n && n < 0x1f)
        return 0x1f - n;
    if (0x12 < n && n < 0x19)
        return 0x19 - n;
    if (0x4b < n && n < 0x52)
        return 0x52 - n;
    if (0x58 < n && n < 0x5f)
        return 0x5f - n;
    if (0x52 < n && n < 0x59)
        r = 0x59 - n;
    return r;
}
