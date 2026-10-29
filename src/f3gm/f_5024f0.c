/* 0x5024f0, 1 when the building b is of the type 10 (the town market) */

int gm_IsType10(unsigned char *b)
{
    return ai_BTypes[*b * 0x24d] == 10;
}
