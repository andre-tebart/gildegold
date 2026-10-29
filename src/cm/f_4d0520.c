/* 0x4d0520, an order kind 8 for the character c */

void ai_Unk4d0520(char *rec, char *c)
{
    char *q;
    int L[11];

    q = (char *)ai_Unk4cdf00(rec, c);
    q[0xc] = 0;
    memcpy(L, q, 0x2c);
    L[0] = *(int *)(c + 4);
    L[1] = 8;
    ai_Unk4c5c40(*(int *)(rec + 0x10), L);
}
