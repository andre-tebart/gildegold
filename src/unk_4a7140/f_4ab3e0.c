/* 0x4ab3e0, decide callback of action 0x44 "Blutrache": p (unk194 >= 5, not flagged 8, not of kind 3) with its
   best building of kind 0x1f holding object 0x27c picks a victim within 25..57 units and up to four helpers
   (0x4a61d0) it can afford by their weight (0x4e7c40 / 3 + 2), into m / m2 (kind 7) */

int ai_Unk4ab3e0(int a, AIPER *p, int c, unsigned char *m, unsigned char *m2)
{
    unsigned short ids[4];
    AIPER *who[4];
    int v, i, n, cnt, s, t;
    int bestv, k;
    unsigned int flags;
    unsigned short id;
    AIBLD *b, *best;
    AIPER *o;

    v = gm_Unk5044b0(p->id, 0xe);
    ids[0] = 0xffff;
    ids[1] = 0xffff;
    ids[2] = 0xffff;
    ids[3] = 0xffff;
    best = 0;
    bestv = 0;
    who[0] = 0;
    who[1] = 0;
    who[2] = 0;
    who[3] = 0;
    if ((p->flags1cc & 8) != 0 || p->unk194 < 5)
        return 0;
    if ((a != 0x44 && a != 0) || c != 0 || v == 0 || p->kind == 3)
        return 0;
    b = ai_BldsT;
    for (i = 0x100; i != 0; i--) {
        if (b->type != 0 && b->owner == p->id && ai_BTypesT[b->type].kind == 0x1f
            && gm_Query(b->objList, 2, 6, 0, 0x27c) != 0) {
            if (best == 0) {
                best = b;
                bestv = gm_BathObjectCount(b);
            } else {
                k = gm_BathObjectCount(b);
                if (k > bestv) {
                    best = b;
                    bestv = k;
                }
            }
        }
        b = (AIBLD *)((unsigned char *)b + 0xa9);
    }
    if (best == 0 || bestv == 0)
        return 0;
    flags = ((unsigned)&ai_Unka69140 + 0x4973c0);
    if (G10_4A58D0(p, 1, (int *)&flags, 0.0f, (float)util_Random(0x20) + 25.0f, &id) == 0)
        return 0;
    o = &((AIPER *)ai_CharsT)[id];
    s = amt_GetKindLevel(o->unk166) / 3 + 2;
    if (s + 2 > p->unk194)
        return 0;
    n = ai_Unk4a61d0(p, 4, flags, 25.0f, 75.0f, bestv, ids);
    if (n == 0)
        return 0;
    cnt = 0;
    for (i = 0; i < n; i++) {
        t = amt_GetKindLevel(((AIPER *)ai_CharsT)[ids[i]].unk166) / 3 + 2 + s;
        if (t >= p->unk194) {
            s = t;
            who[cnt++] = &((AIPER *)ai_CharsT)[ids[i]];
        }
    }
    if (cnt == 0)
        return 0;
    *(int *)(m + 4) = o->netId;
    *(int *)(m + 0xc) = who[0]->netId;
    if (cnt > 1)
        *(int *)(m + 8) = who[1]->netId;
    if (cnt > 2)
        *(int *)(m2 + 0xc) = who[2]->netId;
    if (cnt > 3)
        *(int *)(m2 + 8) = who[3]->netId;
    *(int *)(m + 0x10) = s;
    m[0] = 7;
    return 0x44;
}
