/* 0x4948f0, rolls the three (value, option, option) triples of a character */

void ai_Unk4948f0(AIC4 *ch, int *out)
{
    int t[3][4] = {{4, 0, 3, 1}, {3, 1, 2, 0}, {2, 0, 4, 1}};
    float f[3];
    float m;
    int i, k, v;

    v = gm_GetWorth(ch->id);
    k = (int)(v * 0.025f);
    m = 0.0f;
    for (i = 0; i < 3; i++) {
        f[i] = gm_MapDistance(ai_Unk7e3f80, ai_Unk7e3f84, ai_Unk7e4274[i].a, ai_Unk7e4274[i].b);
        if (f[i] > m)
            m = f[i];
        out[i * 3 + 1] = t[i][0];
        out[i * 3 + 2] = t[i][util_Random(3) + 1];
    }
    for (i = 0; i < 3; i++) {
        f[i] = 1.0f / m * f[i];
        out[i * 3] = (int)(k * f[i]);
    }
}
