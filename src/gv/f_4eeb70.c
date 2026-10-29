/* 0x4eeb70, sends the explosion parts (a, b, d: 0x68-byte records, any may be 0) of the object o to the server: the message 0x4c6260 (id +1 of o, v, mode 1 / 2, the record); before the first one
   the charge is taken (0x4f2e30 (8, 1, c[1], -1, -1), 0x4c5f60 (c[1], -n), 0x4c4830 (-1, c[1], player, x)); returns the answer or -1. ROUGH */

int unk_Unk4eeb70(char *o, int *c, int *a, int *b, int *d, float v, int n, int x)
{
    int first;
    char *s;
    int m[32];
    int mode, r, v2, i;

    v2 = v;
    first = 1;
    r = -1;
    mode = 0;
    if (o != 0) {
        s = unk_Unk4eedd0(*(int *)(o + 1));
        if (s != 0 && (((unsigned char *)s)[4] & 2) == 0) {
            if (d == 0) {
                if (b != 0)
                    mode = 1;
            } else
                mode = 2;
            memset(m, 0, 0x80);
            if (a != 0) {
                unk_Unk4f2e30(8, 1, c[1], -1, -1);
                cm_SendMoneyDelta(c[1], -n);
                cm_SendMoveGoodsUnchecked(-1, c[1], ai_OwnPlayer, x);
                m[0] = *(int *)(o + 1);
                m[2] = mode;
                m[1] = v;
                for (i = 0; i < 0x1a; i++)
                    m[3 + i] = a[i];
                first = 0;
                v2 = 0;
                r = ((int (*)(int))ai_Unk4c6260)(m);
            }
            if (b != 0) {
                if (first) {
                    cm_SendMoneyDelta(c[1], -n);
                    cm_SendMoveGoodsUnchecked(-1, c[1], ai_OwnPlayer, x);
                    first = 0;
                }
                m[0] = *(int *)(o + 1);
                m[2] = mode;
                m[1] = v2;
                for (i = 0; i < 0x1a; i++)
                    m[3 + i] = b[i];
                v2 = 0;
                r = ((int (*)(int))ai_Unk4c6260)(m);
            }
            if (d != 0) {
                if (first) {
                    cm_SendMoneyDelta(c[1], -n);
                    cm_SendMoveGoodsUnchecked(-1, c[1], ai_OwnPlayer, x);
                }
                m[0] = *(int *)(o + 1);
                m[1] = v2;
                m[2] = mode;
                for (i = 0; i < 0x1a; i++)
                    m[3 + i] = d[i];
                r = ((int (*)(int))ai_Unk4c6260)(m);
            }
            return r;
        }
    }
    return -1;
}
