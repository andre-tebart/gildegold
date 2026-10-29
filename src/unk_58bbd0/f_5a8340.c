/* 0x5a8340, the record's random (68 B): the LCG at +4 (0x41c64e6d, 0x3039) and the high word / 0x7fff */

unsigned int ai_Unk5a8340(int param_1)
{
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) * 0x41c64e6d + 0x3039;
    return *(unsigned short *)(param_1 + 6) / 0x7fff;
}
