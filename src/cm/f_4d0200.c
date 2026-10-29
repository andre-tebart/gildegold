/* 0x4d0200, like 0x4cffd0, the order kind 3 with the ground point of tgt (record 4cdf00 copied; L[0] the character id,
   L[4] / L[5] the arguments c / tgt as they are in the original). Rough draft. */

int ai_Unk4d0200(char *rec, char *c, char *tgt)
{
    char *sc = *(char **)(ai_Unk5fb5b0 + 0xb0);
    char *q;
    float v[3];
    int L[11];
    int a, b, h;

    if (tgt == 0 || c == 0 || rec == 0)
        return -1;
    d3_PointToWorld(tgt, tgt + 0x4c, v);
    if ((char)d3_GetHeightAt(sc, v, &a, &b, &h) == 0)
        return -1;
    q = (char *)ai_Unk4cdf00(rec, c);
    if (q == 0)
        return -1;
    q[0xc] = 0;
    memcpy(L, q, 0x2c);
    L[4] = (int)tgt;
    L[0] = *(int *)(c + 4);
    L[1] = 3;
    L[5] = (int)c;
    ai_Unk4c5c40(*(int *)(rec + 0x10), L);
}
