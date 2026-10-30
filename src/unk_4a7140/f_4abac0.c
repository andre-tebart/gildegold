/* 0x4abac0, decide callback of action 0x46 "Theater": p (unk194 > 1, not of kind 3) picks its theatre (a building
   of kind 0x1f holding object 0x27f, not busy by 0x5741d0; the bigger one, cap 2: 7, else 4) and a play length
   by its rank, into m (kind 7) */

int ai_Unk4abac0(int a, AIPER *p, int c, unsigned char *m)
{
    int v, k, n, i;
    unsigned int r;
    AIBLD *b, *best;
    AIBTYPE *bt;

    v = gm_Unk5044b0(p->id, 0xe);
    k = 0;
    if ((a == 0x46 || a == 0) && c == 0 && v != 0 && p->kind != 3 && p->unk194 > 1) {
        best = 0;
        b = ai_BldsT;
        for (i = 0x100; i != 0; i--) {
            if (b->type != 0 && b->owner == p->id) {
                bt = &ai_BTypesT[b->type];
                if (bt->kind == 0x1f && gm_Query(b->objList, 2, 6, 0, 0x27f) != 0 && ai_FindCurseEvent(b) == 0) {
                    if (best == 0) {
                        n = bt->cap247 != 2 ? 4 : 7;
                    } else {
                        n = bt->cap247 != 2 ? 4 : 7;
                        if (n <= k)
                            goto next;
                    }
                    k = n;
                    best = b;
                }
            }
next:
            b = (AIBLD *)((unsigned char *)b + 0xa9);
        }
        if (best != 0 && k != 0) {
            if (p->unk194 < 4) {
                if (p->unk194 < 3) {
                    if (k > 3)
                        k = 4;
                } else if (k > 6) {
                    k = 7;
                }
            } else if (k > 9) {
                k = 10;
            }
            r = util_RandomU(k);
            if (r < 4)
                k = 2;
            else
                k = (r > 6) + 3;
            *(int *)(m + 4) = best->id;
            *(unsigned int *)(m + 0xc) = r;
            *(int *)(m + 0x10) = k;
            m[0] = 7;
            return 0x46;
        }
    }
    return 0;
}
