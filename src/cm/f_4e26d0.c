/* 0x4e26d0, the answer of a computer character to the offer m of 0x4e24c0: the price (+0x20) is compared with a value the character's
   resources (0x509e60) and a random factor (0x4e5360, 0x5eba9c / 0x5ebb30) give; r + 0x98 = 1 and r + 0x94 = the counter offer rounded to
   a multiple of 32 when it is acceptable. ROUGH: the float expressions are only roughly in the order of the code */

void ai_Unk4e26d0(char *r, char *m)
{
    int i, n, id, pay, v, x;
    float lim, k;
    char *c;

    *(int *)(r + 0x98) = 0;
    n = *(int *)(m + 0x48);
    id = *(int *)(r + 0x10);
    for (i = 0; i < n && *(int *)(m + 0x24 + i * 4) != id; i++)
        ;
    if (i >= n || *(int *)(m + i * 4) == -1)
        return;
    *(int *)(r + 0x94) = *(int *)(m + 0x20);
    gm_BuildQuery(1, 1, *(int *)(m + 0x44));
    pay = *(int *)(m + 0x20);
    c = (char *)gm_FindChar(*(int *)(r + 0x10));
    if (c == 0)
        return;
    lim = (float)(*(int *)(m + 0x4c) * 32000 + pay);
    k = ai_Unk4e5360() * 0.16666667f + 0.3f;
    x = gm_GetGoodsValue(c);
    if ((float)x * k > lim)
        v = (int)lim;
    else {
        k = ai_Unk4e5360() * 0.16666667f + 0.3f;
        x = gm_GetGoodsValue(c);
        v = (int)(lim * x);
    }
    if (v < pay)
        return;
    if (k < ai_Unk4e5360() / v)
        return;
    if (v > pay * 2)
        v = pay * 2;
    v -= pay;
    *(int *)(r + 0x98) = 1;
    x = (int)(ai_Unk4e52f0(v) * *(int *)(m + 0x4c) * 0.1f + pay);
    *(int *)(r + 0x94) = x - x % 32;
}
