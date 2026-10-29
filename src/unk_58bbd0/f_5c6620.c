/* 0x5c6620, the marriage rating of two characters (752 B, ROUGH): param_1 / param_2 the ids; the 0x220
   local score record (0x88 dwords, [0] = 1); 0x505df0 resolves both; the partner records +0x30 / +0x32
   (0x505df0, 0x50a640 / 2 the two halves into the heights 0x224 / 0x22c); the own amount 0x50a640 (the
   larger of the two + the own, the ratio * 5 + 1, else 1.0); the relation 0x50c550 and the age 0x505e30
   (clamped to 15 / 30) give (1 - age * 1/15) * 4 + 1 + (100 - relation) * 0.05 + 1 + the score + the
   five bytes param_2 + 0x80 .. (each / 252); the 0x4f81c0 (0x10, the record, the score) bit 2 halves
   it; the result is the __ftol of the score (the <= 2 / < 15 ladder). ROUGH: the float order of the sum
   and the two __ftol exits from Ghidra */

int ai_Unk5c6620(int param_1, int param_2)
{
    char *a;
    char *b;
    char *p;
    int score[0x88];
    float hi1;
    float hi2;
    float lim;
    float rate;
    float age;
    float rel;
    int i;
    int v;
    int n;

    for (i = 0; i < 0x88; i++)
        score[i] = 0;
    hi1 = 0.0f;
    hi2 = 0.0f;
    score[0] = 1;
    a = (char *)gm_FindChar(param_1);
    b = (char *)gm_FindChar(param_2);
    if (a == 0 || b == 0)
        return 0;
    p = (char *)gm_FindChar(U_I(b + 0x30));
    if (p != 0) {
        v = gm_GetWorth(*(unsigned short *)p);
        hi2 = (float)(v / 2);
    }
    p = (char *)gm_FindChar(U_I(b + 0x32));
    if (p != 0) {
        v = gm_GetWorth(*(unsigned short *)p);
        hi1 = (float)(v / 2);
    }
    v = gm_GetWorth(*(unsigned short *)b);
    if ((float)v <= 0.0f)
        lim = 0.0f;
    else
        lim = (float)gm_GetWorth(*(unsigned short *)b);
    if (lim <= (hi1 < hi2 ? hi2 : hi1))
        lim = hi1 <= hi2 ? hi2 : hi1;
    v = gm_GetWorth(*(unsigned short *)a);
    if ((float)v <= lim)
        rate = ((lim - (float)v) / lim) * 5.0f + 1.0f;
    else
        rate = 1.0f;
    rel = gm_Liking(*(unsigned short *)a, *(unsigned short *)b, 1);
    age = (float)gm_CharAge(*(unsigned short *)b);
    if (age > 15.0f)
        age = age > 30.0f ? 30.0f : age;
    else
        age = 15.0f;
    n = (int)((1.0f - age * 0.06666667f) * 4.0f + 1.0f + (float)((100.0f - rel) * 0.05f + 1.0f) + rate);
    /* ROUGH: the score sum is a float; the five bytes param_2 + 0x80 .. + 0x84 enter as
       *(unsigned char *)(b + 0x80 + i) * 0.003968254 */
    for (i = 0; i < 5; i++)
        n = n + (int)(*(unsigned char *)(b + 0x80 + i) * 0.003968254f);
    if (ord_DispatchOrder(0x10, a, score) & 2)
        n = n / 2;
    return n;
}
