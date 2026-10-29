/* 0x504350, the group of the title number n (1 .. 6 and 0x28 .. 0x2d: 7; 0x23 .. 0x27: 2; 0x2e .. 0x33: 3; 0x3a .. 0x3f: 4; 0x40 .. 0x45: 5), 0 else */


int gm_Unk504350(int n)
{
    if (0 < n && n < 7)
        return 7;
    if (0x27 < n && n < 0x2e)
        return 7;
    if (0x22 < n && n < 0x28)
        return 2;
    if (0x2d < n && n < 0x34)
        return 3;
    if (0x39 < n && n < 0x40)
        return 4;
    if (0x3f < n && n < 0x46)
        return 5;
    if (0xc < n && n < 7)
        return 6;
    return 0;
}
