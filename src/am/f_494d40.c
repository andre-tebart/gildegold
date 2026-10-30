/* 0x494d40, a rating from the rank difference of two characters and a count */

int ai_Unk494d40(AIC4 *a, AIC4 *b, int n)
{
    int x, y;

    x = gm_Unk505e60S(a->id, 0);
    y = gm_Unk505e60S(b->id, 0);
    return (int)((float)((x - y + 10) * n * 3) * 0.33333334f);
}
