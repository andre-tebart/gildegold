/* 0x4a9030, eval callback: p (unk194 >= 2) with n > 0 of 0x4f39b0: the count, at most unk194 / 2, into m->0x10 */

int ai_Unk4a9030(int a, AIPER *p, int c, unsigned char *m)
{
    int n, k;

    if (p->unk194 < 2)
        return 0;
    n = unk_Unk4f39b0(p->netId);
    if (n < 1)
        return 0;
    k = p->unk194 / 2;
    if (n <= k)
        k = n;
    *(int *)(m + 0x10) = k;
    return 1;
}
