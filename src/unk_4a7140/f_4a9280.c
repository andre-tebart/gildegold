/* 0x4a9280, decide callback: as 0x4a9080 with a closer acquaintance (at least 5..12) */

int ai_Unk4a9280(int a, AIPER *p, int c, int d, unsigned char *m)
{
    int cnt[32];
    int ids[32];
    unsigned short r;
    int n, i, j;
    AIPER *o;
    unsigned char k;

    r = util_Random(8);
    if (p->unk166 != 13) {
        n = unk_Unk4f3c50(ids, cnt, p->id);
        if (n > 0) {
            i = 0;
            j = n;
            do {
                if (cnt[i] >= r + 5) {
                    o = AI_505DF0(ids[i]);
                    if (o != 0 && o->kind != 15 && ((unsigned char *)o)[8] != 0) {
                        k = o->unk166;
                        if (k != 13 && k != 27 && k != 15 && k != 21) {
                            m[0] = 7;
                            *(int *)(m + 4) = o->netId;
                            return 1;
                        }
                    }
                }
                i = (i + 1) % n;
            } while (--j);
        }
    }
    return 0;
}
