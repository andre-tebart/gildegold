/* 0x4fe940, 1 when the object prototype n is a building part of the kinds 1, 3, 4, 0xb, 0x1a, 0x1b, 0x1c */

int gm_IsBuildingKind(int n)
{
    char k;

    k = *(char *)(ai_Goods + n * 0x40 + n);
    if (k != 1 && k != 0x1a && k != 0x0b && k != 0x1c && k != 0x1b && k != 3 && k != 4)
        return 0;
    return 1;
}
