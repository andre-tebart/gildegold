/* 0x4d02d0, an order kind 4 for the character c with the name of the object tgt (strcpy to +0x10) and its ground
   point (x, z); returns 1, 0 when the point is not on the ground. Rough draft. */

int ai_Unk4d02d0(char *rec, char *c, char *tgt)
{
    char *sc = *(char **)(ai_Unk5fb5b0 + 0xb0);
    char *q;
    float v[4];
    int x, z, h;

    ai_Unk4cda50(c);
    d3_PointToWorld(tgt, tgt + 0x4c, v);
    if ((char)d3_GetHeightAt(sc, v, &x, &z, &h) == 0)
        return 0;
    q = (char *)ai_Unk4cdf00(rec, c);
    q[0xc] = 0;
    strcpy(q + 0x10, tgt);
    *(int *)q = *(int *)(c + 4);
    *(int *)(q + 4) = 4;
    *(int *)(q + 0x20) = x;
    *(int *)(q + 0x24) = z;
    return 1;
}
