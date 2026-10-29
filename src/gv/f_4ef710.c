/* 0x4ef710, 1 when the +8 word of b is the +0x50 word of a and not 0x7fff */

int unk_Unk4ef710(char *a, char *b)
{
    short v;

    v = *(short *)(a + 0x50);
    if (*(short *)(b + 8) == v && v != 0x7fff)
        return 1;
    return 0;
}
