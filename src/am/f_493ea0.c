/* 0x493ea0, picks a character for p (its watchers, listed at 0x68) */

AGCH *ai_Unk493ea0(AGCH *p)
{
    AGCH *list[5];
    AGCH *q, *s, *found;
    int n, m, i, k;
    int *lp;

    found = 0;
    m = 0;
    n = 0;
    lp = p->list;
    for (i = 0; i < 5; i++, lp++) {
        q = AG_CH(*lp);
        if (q != 0) {
            n++;
            if (q->unk008 > 0 && q->unk00a > 11)
                list[m++] = q;
        }
    }
    if (m != 0)
        found = list[util_RandomU(m)];
    if (p->kind == 5) {
        if (found != 0)
            return found;
        if (n != 0) {
            int neg;

            q = AG_CH(p->unk05c);
            neg = -1;
            if (q != 0) {
                cm_BeginFieldBlock(q, q->netId);
                cm_AddFieldDelta(4, 1, 0x5c, &neg);
                cm_SendFieldDeltas();
            }
            for (i = 0; i < 5; i++) {
                s = AG_CH(p->list[i]);
                if (s != 0) {
                    if (s->unk060 == p->netId)
                        k = 1;
                    else if (s->unk064 != p->netId)
                        continue;
                    else
                        k = 2;
                    cm_BeginFieldBlock(s, s->netId);
                    cm_AddFieldDelta(4, 1, k * 4 + 0x5c, &neg);
                    cm_SendFieldDeltas();
                }
            }
        }
    } else {
        q = AG_CH(p->unk05c);
        if (q != 0 && q->unk008 > 0 && q->kind > 1 && q->kind < 10 && (q->flags1c8 & 0x40000) == 0)
            return q;
        if (found != 0 && found->kind > 1 && found->kind < 10)
            return found;
    }
    return 0;
}
