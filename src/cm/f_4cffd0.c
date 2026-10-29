/* 0x4cffd0, an order for the character c of the player record rec to the object / ground point (tgt, x, z): the order
   record (0x2c bytes, the one of 4cdf00 copied; L[1] = 2 the order kind, L[4..6] target id / x / z, byte +0x1c a flag)
   is passed to 0x4c5c40; characters of the kinds 0x15e .. 0x176 need a worker count in 4cdac0 (else the message
   ai_Unk143e8a4 .. a8 / ac). Rough draft: 0x45f170 writes through the addresses of the parameters in the original. */

int ai_Unk4cffd0(char *rec, char *c, char *tgt, int x, int z)
{
    char *sc = *(char **)(ai_Unk5fb5b0 + 0xb0);
    char *k, *q, *r;
    int L[11];
    int h;

    if (c == 0)
        return -1;
    k = (char *)ai_Unk4cda50(c);
    if (tgt == 0 || *(char **)(*(char **)(tgt + 0x184) + 0x34) == 0)
        return -1;
    if ((char)d3_GetHeightAt(sc, *(char **)(*(char **)(tgt + 0x184) + 0x34) + 0x4c, &x, &z, &h) == 0)
        return -1;
    q = (char *)ai_Unk4cdf00(rec, c);
    if (q == 0)
        return -1;
    q[0x1c] = 0;
    q[0xc] = 0;
    memcpy(L, q, 0x2c);
    if (k == 0 || *(int *)k == 0x154 || *(int *)k == 0x156 || *(int *)k == 0x158 || *(int *)k == 0x16e ||
        *(int *)k == 0x172 || *(int *)k == 0) {
        L[4] = *(int *)(tgt + 4);
        L[0] = *(int *)(c + 4);
        L[1] = 2;
    } else if (*(int *)k == 0x15e || *(int *)k == 0x160) {
        r = ai_Unk4cdac0(c);
        if (r == 0 || *(int *)(r + 0x20) <= 0) {
            sv_SetStatusText((char *)ai_Unk143e8a4);
            return -1;
        }
        L[0] = *(int *)(c + 4);
        L[4] = *(int *)(tgt + 4);
        L[5] = x;
        L[6] = z;
        L[1] = 2;
    } else if (*(int *)k == 0x174) {
        r = ai_Unk4cdac0(c);
        if (r == 0 || *(int *)(r + 0x20) <= 0) {
            sv_SetStatusText((char *)ai_Unk143e8a8);
            return -1;
        }
        L[0] = *(int *)(c + 4);
        L[4] = 0;
        L[5] = x;
        L[6] = z;
        L[1] = 2;
    } else if (*(int *)k == 0x176) {
        r = ai_Unk4cdac0(c);
        if (r == 0 || *(int *)(r + 0x20) <= 0) {
            sv_SetStatusText((char *)ai_Unk143e8ac);
            return -1;
        }
        L[4] = 0;
        L[5] = x;
        L[6] = z;
        L[0] = *(int *)(c + 4);
        L[1] = 2;
    }
    if (c[0x1c4] == 1 || c[0x1c4] == 2 || c[0x1c4] == 3)
        ((char *)L)[0x1c] = 1;
    ai_Unk4c5c40(*(int *)(rec + 0x10), L);
}
