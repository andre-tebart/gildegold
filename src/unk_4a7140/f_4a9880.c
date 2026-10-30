/* 0x4a9880, decide callback: p (not flagged 0x100, unk194 >= 4) picks a character of district 5 within 16..31 units,
   not its partner (0x5c), into m (kind 7); 0x20 if p cannot */

int ai_Unk4a9880(int a, AIPER *p, int *ctx, int d, unsigned char *m)
{
    G10DREC rec[6];
    AIPER *who[6];
    AIPER *o;
    int id, n, i;
    unsigned int r;

    if ((p->flags1c8 & 0x100) != 0)
        return 0x20;
    if (p->unk194 < 4)
        return 0x20;
    if (ctx[0] == 1) {
        id = p->netId;
    } else {
        if (ctx[0] != 2)
            return 0;
        if (ctx[0x87] == 0)
            return 0;
        id = ((AIPER *)ctx[0x87])->netId;
    }
    n = amt_GetColumnOffices(5, 6, rec);
    for (i = 0; i < n; i++)
        who[i] = AI_505DF0(rec[i].id);
    if (n < 6)
        memset(&who[n], 0, (6 - n) * sizeof(who[0]));
    r = util_RandomU(6);
    i = 6;
    do {
        if (who[r] != 0 && id != who[r]->netId
            && gm_Liking(who[r]->id, p->id, 1) < (float)util_RandomU(0x10) + 16.0f) {
            o = who[r];
            if (o == 0)
                return 0;
            if (*(int *)((unsigned char *)p + 0x5c) == o->netId)
                return 0;
            m[0] = 7;
            *(int *)(m + 4) = o->netId;
            return 1;
        }
        r = (int)(r + 1) % 6;
    } while (--i);
    return 0;
}
