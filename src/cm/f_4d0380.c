/* 0x4d0380, an order kind 6 with the name of the object tgt, copied to +0x10 of the order record that goes to 0x4c5c40;
   -1 when the point is not on the ground or the character c has a job (+0x1ac). Rough draft. */

int ai_Unk4d0380(char *rec, char *c, char *tgt, unsigned char a)
{
    char *sc = *(char **)(ai_Unk5fb5b0 + 0xb0);
    char *q;
    float v[4];
    int L[11];
    int x, z, h;

    ai_Unk4cda50(c);
    d3_PointToWorld(tgt, tgt + 0x4c, v);
    if ((char)d3_GetHeightAt(sc, v, &x, &z, &h) == 0 || *(int *)(c + 0x1ac) != 0)
        return -1;
    q = (char *)ai_Unk4cdf00(rec, c);
    q[0xc] = 0;
    memcpy(L, q, 0x2c);
    L[0] = *(int *)(c + 4);
    L[1] = 6;
    strcpy((char *)&L[4], tgt);
    ((char *)L)[0x1c] = a;
    ((char *)L)[0x1d] = ((char *)v)[0];
    ai_Unk4c5c40(*(int *)(rec + 0x10), L);
}
