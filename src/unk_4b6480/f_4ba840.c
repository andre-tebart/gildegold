/* 0x4ba840, takes a share f of the goods (0x4ffe90 (1, 5) list, more than 1 each) of the object (2, 6, 0, 0x2a) of
   one of c's buildings of kind 1 / 2 (0x4c4880 to -1); *id = the building; 0: none */

int ai_Unk4ba840(AICHAR *c, int *id, float f)
{
    AIBLD *b;
    AIOBJ *o, *e;
    int n, k, i, v;
    void *a[12];

    n = 0;
    for (b = gm_BuildQuery(1, 3, c->id); b != 0 && n < 8; b = gm_BuildNext()) {
        i = gm_BuildingClass(*(unsigned char *)b);
        if (i == 1 || i == 2)
            a[n++] = b;
    }
    if (n != 0) {
        k = 0;
        b = a[util_RandomU(n)];
        *id = *(int *)((char *)b + 1);
        o = gm_Query(*(int *)((char *)b + 0x5d), 2, 6, k, 0x2a);
        if (o != 0) {
            for (e = gm_Query(*(int *)((char *)o + 0x14), 1, 5); e != 0 && k < 12; e = gm_QueryNext())
                a[k++] = e;
            if (k != 0) {
                for (i = 0; i < k; i++) {
                    e = a[i];
                    v = *(int *)((char *)e + 0xe);
                    if (v > 1) {
                        v = (int)(v * f);
                        if (v < 1)
                            v = 1;
                        cm_SendExSellObjectI(-1, *(int *)((char *)o + 2), *(unsigned short *)e, v, ai_OwnPlayer, 0);
                    }
                }
                return 1;
            }
        }
    }
    return 0;
}
