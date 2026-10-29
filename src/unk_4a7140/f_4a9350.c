/* 0x4a9350, decide callback: p (unk194 > 1, not flagged 0x100) picks an acquaintance (more than 3) not flagged
   0x200 whose district level is above 3, into m (kind 7) */

int ai_Unk4a9350(int a, AIPER *p, int c, int d, unsigned char *m)
{
    G10DIST info;
    int cnt[32];
    int ids[32];
    int n, i, j;
    AIPER *o;

    if (p->unk194 > 1 && (p->flags1c8 & 0x100) == 0) {
        n = unk_Unk4f3c50(ids, cnt, p->id);
        if (n > 0) {
            i = 0;
            j = n;
            do {
                if (cnt[i] > 3) {
                    o = AI_505DF0(ids[i]);
                    if (o != 0 && (o->flags1c8 & 0x200) == 0) {
                        if (amt_GetKind(o->unk166, &info) != 0 && info.lvl > 3) {
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
