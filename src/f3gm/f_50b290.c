/* 0x50b290, whether a random number plus 0.15 is below c * (the weight of 0x50c550) * 0.01 / d */

int gm_Unk50b290(int a, int b, int c, int d)
{
    float f, r;

    f = gm_Liking(a, b, 1);
    r = util_RandomF();
    if (r + 0.15 < (double)(c * f * 0.01 / d))
        return 1;
    return 0;
}
