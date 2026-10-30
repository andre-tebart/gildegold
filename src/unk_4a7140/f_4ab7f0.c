/* 0x4ab7f0, decide callback of action 0x45 "Verfluchen": p (not flagged 0x10, not of kind 3) with its best
   building of kind 0x1f holding object 0x28a curses a character within 25..57 units with a strength it can
   afford (32000 per level from a quarter of its money), into m (kind 7) */

int ai_Unk4ab7f0(int a, AIPER *p, int c, unsigned char *m)
{
    int v, i, k, r, money;
    int bestv, n;
    unsigned int flags;
    unsigned short id;
    AIBLD *b, *best;
    AIPER *o;

    v = gm_Unk5044b0(p->id, 0xe);
    bestv = 0;
    best = 0;
    flags = (unsigned int)best;
    if ((p->flags1cc & 0x10) != 0)
        return 0;
    if ((a != 0x45 && a != 0) || c != 0 || v == 0 || p->kind == 3)
        return 0;
    b = ai_BldsT;
    for (i = 0x100; i != 0; i--) {
        if (b->type != 0 && b->owner == p->id && ai_BTypesT[b->type].kind == 0x1f
            && gm_Query(b->objList, 2, 6, 0, 0x28a) != 0) {
            if (best == 0) {
                best = b;
                bestv = gm_CurseObjectCount(b);
            } else {
                n = gm_CurseObjectCount(b);
                if (n > bestv) {
                    best = b;
                    bestv = n;
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
    money = (int)(gm_Unk509e30(*(int *)((unsigned char *)p + 0x178), ai_OwnPlayer) * 0.25f);
    gm_GetWorth(p->id);
    for (k = bestv - 1; k > 0; k--) {
        if (money > (k > 1 ? k : 1) * 32000)
            break;
    }
    if (k == 0)
        return 0;
    r = util_Random(k + 1);
    *(int *)(m + 4) = o->netId;
    k = r > 1 ? r : 1;
    *(int *)(m + 0xc) = r;
    *(int *)(m + 0x10) = k * 32;
    m[0] = 7;
    return 0x45;
}
