/* 0x494a30, whether a character is not worth a second look at another (rank and distance against a roll) */

int ai_Unk494a30(AIC4 *a, AIC4 *b)
{
    float t, s;
    int x, y;

    if (a->netId == (unsigned int)b->partner)
        return 0;
    x = gm_Unk505e60S(a->id, 0);
    y = gm_Unk505e60S(b->id, 0);
    t = (float)(y - x) * 2.5f;
    s = gm_Liking(a->id, b->id, 1) + t;
    if (s < util_RandomF() * 75.0f)
        return 0;
    return 1;
}
