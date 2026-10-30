/* 0x49b380, decide callback 0x20: gifts for a young child (0x162..0x164), a fiancee (0x167) or a wedding
   ring (0x170) */

int ai_Unk49b380(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int k, i, n, r;
    int *rel;
    AIPER *c;
    AIPER *w;
    AIJOB *j;
    int st[3];
    AIARG s1;
    AIARG s2;
    G8REC recs[3];

    k = a;
    if (a == 0)
        k = 0x20;
    if (f != 0 || a != 0)
        return 0;
    rel = (int *)((char *)p + 0x5c);
    for (i = 3; i < 8; i++) {
        c = AI_505DF0(rel[i]);
        if (c != 0 && c->unk00a < 0x28)
            break;
    }
    for (i = 3; i < 8; i++) {
        c = AI_505DF0(rel[i]);
        if (c != 0 && c->unk00a < 0x28)
            break;
    }
    memset(recs, 0, sizeof(recs));
    if (c != 0) {
        recs[0].item = 0x162;
        recs[1].item = 0x163;
        recs[2].item = 0x164;
        n = 3;
    } else {
        if (rel[0] != -1)
            return 0;
        for (j = he_FindEvent(1, 0, 0x41); j != 0; j = he_NextEvent()) {
            if (((int *)j)[0xac / 4] == (int)p->netId)
                break;
        }
        if (j != 0) {
            w = AI_505DF0(((int *)j)[0xb0 / 4]);
            if (w == 0)
                return 0;
            recs[0].item = 0x167;
        } else {
            recs[0].item = 0x170;
        }
        recs[1].item = 0;
        recs[2].item = 0;
        n = 1;
    }
    ai_Unk4a5760(p, n, (int *)recs, st);
    if (st[0] == 6 && st[2] == 0)
        return 0;
    if (st[1] == n)
        return 0;
    if (st[0] < 6) {
        if (st[2] == 0) {
            i = util_RandomU(n);
            while (recs[i].unk04 != 0)
                i = (i + 1) % n;
            s2.kind = 2;
            s2.unk04 = recs[i].item;
            s1.unk04 = p->netId;
            s2.unk08 = 1;
            s2.unk14 = 0.05f;
            s1.kind = 7;
            s1.unk08 = 1;
            r = ai_Unk495a50(k, p->id, 2, &s2, &s1);
            goto done;
        }
    } else if (st[2] == 0) {
        return 0;
    }
    i = util_RandomU(n);
    while (recs[i].unk04 < 1)
        i = (i + 1) % n;
    s2.kind = 1;
    s2.unk08 = 1;
    s2.unk04 = recs[i].unk08;
    s2.unk14 = 0.05f;
    switch (recs[i].item) {
    case 0x170:
        s1.kind = 0;
        s1.unk08 = -1;
        s1.unk04 = -1;
        break;
    case 0x167:
        s1.kind = 7;
        s1.unk08 = -1;
        s1.unk04 = w->netId;
        break;
    case 0x162:
    case 0x163:
    case 0x164:
        s1.kind = 7;
        s1.unk08 = -1;
        s1.unk04 = c->netId;
        break;
    default:
        return 0;
    }
    s1.unk10 = 0x20;
    if (recs[i].item != 0x170) {
        *o1 = s2;
        *o2 = s1;
        return 0x20;
    }
    r = ai_Unk495a50(k, p->id, 3, &s2, &s1);
done:
    if (r == 0)
        return r;
    *o1 = s2;
    *o2 = s1;
    return r;
}
