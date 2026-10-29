/* 0x502520, 1 when the building b is a home (the types 0xb, 0xc, 0xd, 0x10, 0x1c, 0x1d, 0x1f) */

int gm_IsHomeType(unsigned char *b)
{
    char k;

    if (b == 0)
        return 0;
    k = *(char *)(ai_BTypes + (unsigned)*b * 0x24d);
    if (k != 0x0b && k != 0x0d && k != 0x0c && k != 0x10 && k != 0x1c && k != 0x1d && k != 0x1f)
        return 0;
    return 1;
}
