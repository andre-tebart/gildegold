/* 0x4e5360, a random float in [0, 1) (logged "cut_rndfloat: %f") */

double ai_Unk4e5360(void)
{
    char buf[0x40];

    ai_Unk92c4e0 = ai_Unk92c4e0 * 0x41c64e6d + 0x3039;
    sprintf(buf, "cut_rndfloat: %f", (float)(G28_HI % 0x7fffu) * 3.0518509e-5f);
    return (float)(G28_HI % 0x7fffu) * 3.0518509e-5f;
}
