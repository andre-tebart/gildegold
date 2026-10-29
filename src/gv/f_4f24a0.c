/* 0x4f24a0, calls 0x5085f0 with 0.1 * x / (log10(x) + 1) where x = 0x5085b0 (0x50a640 of the character id) clamped to 10000 .. 100000. ROUGH */

int unk_Unk4f24a0(unsigned short *c)
{
    double x;

    x = (double)gm_ConvertMoney(gm_GetWorth(*c), ai_OwnPlayer);
    if (x <= 100000.0) {
        if (x <= 10000.0)
            x = 10000.0;
    } else
        x = 100000.0;
    return gm_PlayerFactor((int)(x / (log10(x) + 1.0) * 0.1f), ai_OwnPlayer);
}
