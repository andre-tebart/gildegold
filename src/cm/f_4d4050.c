/* 0x4d4050, the fight role of a building kind (first byte of its 0x24d-byte record): 0x13 -> 0, 0x10 -> 1, 4 -> 2, else 0 */

int ai_Unk4d4050(unsigned char *b)
{
    char *r = ai_BTypes + *b * 0x24d;
    char k = *r;

    if (k == 0x13)
        return 0;
    if (k == 0x10)
        return 1;
    return k == 4 ? 2 : 0;
}
