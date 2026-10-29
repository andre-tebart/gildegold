/* 0x4fc7a0, the health float (+0x1c) of the character c iterated down to 1.0 (v = (v - k) + k / v, k = +0x14), truncated */

int unk_Unk4fc7a0(char *c)
{
    float v;

    for (v = *(float *)(c + 0x1c); 1.0 < v; v = (v - *(float *)(c + 0x14)) + *(float *)(c + 0x14) / v)
        ;
    return (int)v;
}
