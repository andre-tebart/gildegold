/* 0x4f2330, the cost of a character's action: x = 0x5085b0 (0x50a640 of the character id) clamped to 10000 .. 1000000, v = log10(x)^2 * 100 * (0.25, 0.5, 0.75 for kind 0, 1, 2); with flag 0 also times
   (0.3 + 0.7 * (a & 0xff) / 255) * 0.75; returns r = 0x5085f0 (v) and stores r * (0.8, 0.7, 0.6)[kind] in *out. ROUGH */

int unk_Unk4f2330(unsigned short *c, int a, int kind, int flag, int *out)
{
    double x, v;
    int r;

    x = (double)gm_ConvertMoney(gm_GetWorth(*c), ai_OwnPlayer);
    if (x <= 1000000.0) {
        if (x <= 10000.0)
            x = 10000.0;
    } else
        x = 1000000.0;
    v = log10(x);
    v = v * v * 100.0;
    if (kind == 0)
        v *= 0.25;
    else if (kind == 1)
        v *= 0.5;
    else if (kind == 2)
        v *= 0.75;
    if (flag == 0)
        v = ((a & 0xff) * 0.00392156862745098 * 0.7f + 0.3f) * (v * 0.75);
    r = gm_PlayerFactor((int)v, ai_OwnPlayer);
    *out = (int)(r * unk_Tab5ee034[kind]);
    return r;
}
