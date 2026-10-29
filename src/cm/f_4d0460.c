/* 0x4d0460, an order kind 7 to the ground point of the character c (4d2170 checks the way): L[4] = the argument,
   L[5] a float of the lookup; -1 when the point is not on the ground. Rough draft (0x45f170 / 0x4d2170 write through the
   addresses of the parameters in the original). */

int ai_Unk4d0460(char *rec, char *c, int a)
{
    char *sc = *(char **)(ai_Unk5fb5b0 + 0xb0);
    char *q;
    int L[11];
    int x, z;

    if ((char)d3_GetHeightAt(sc, *(char **)(*(char **)(c + 0x184) + 0x34) + 0x4c, &a, &x, &z) == 0)
        return -1;
    if (ai_Unk4d2170(c, z, x, 8, &a, &z) == 0)
        return -1;
    q = (char *)ai_Unk4cdf00(rec, c);
    q[0xc] = 0;
    memcpy(L, q, 0x2c);
    L[0] = *(int *)(c + 4);
    L[4] = a;
    L[1] = 7;
    L[5] = z;
    ai_Unk4c5c40(*(int *)(rec + 0x10), L);
}
