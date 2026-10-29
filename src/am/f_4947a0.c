/* 0x4947a0, a rating from the wealth of two characters: x / (log10 x + 1) * 0.1 * 32, x at least 10000 */

int ai_Unk4947a0(AIC4 *a, AIC4 *b)
{
    int x, y;
    double d;

    x = gm_GetWorth(a->id);
    y = gm_GetWorth(b->id);
    d = gm_ConvertMoney(y, ai_OwnPlayer) + gm_ConvertMoney(x, ai_OwnPlayer);
    if (10000.0 > d)
        d = 10000.0;
    return (int)((d / (log10(d) + 1.0) * 0.1f) * 32.0);
}
