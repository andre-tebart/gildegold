/* 0x494830, the same with 0.05 */

int ai_Unk494830(AIC4 *a, AIC4 *b)
{
    int x, y;
    double d;

    x = gm_GetWorth(a->id);
    y = gm_GetWorth(b->id);
    d = gm_ConvertMoney(y, ai_OwnPlayer) + gm_ConvertMoney(x, ai_OwnPlayer);
    if (10000.0 > d)
        d = 10000.0;
    return (int)((d / (log10(d) + 1.0) * 0.05f) * 32.0);
}
