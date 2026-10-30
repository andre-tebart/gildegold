/* 0x49ae50, decide callback 0x1e: one of four goods (0x168, 0x16c, 0x297, 0x2b7) */

int ai_Unk49ae50(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int k, i, r;
    int st[3];
    AIARG s1;
    AIARG s2;
    G8REC recs[4];

    k = a;
    if (a == 0)
        k = 0x1e;
    if (f != 0)
        return 0;
    memset(recs, 0, sizeof(recs));
    recs[0].item = 0x168;
    recs[1].item = 0x16c;
    recs[2].item = 0x297;
    recs[3].item = 0x2b7;
    ai_Unk4a5760(p, 4, (int *)recs, st);
    if (st[0] == 6 && st[2] == 0)
        return 0;
    if (st[1] == 4)
        return 0;
    if (st[0] < 6) {
        if (st[2] == 0) {
            i = util_RandomU(4);
            while (recs[i].unk04 != 0)
                i = (i + 1) % 4;
            s1.kind = 2;
            s1.unk04 = recs[i].item;
            s1.unk08 = 1;
            s2.unk08 = 1;
            s2.unk04 = p->netId;
            s1.unk14 = 0.05f;
            s2.kind = 7;
            r = ai_Unk495a50(k, p->id, 2, &s1, &s2);
            if (r == 0)
                return r;
            *o1 = s1;
            *o2 = s2;
            return r;
        }
    } else if (st[2] == 0) {
        return 0;
    }
    i = util_RandomU(4);
    while (recs[i].unk04 < 1)
        i = (i + 1) % 4;
    s1.kind = 1;
    s1.unk08 = 1;
    s1.unk04 = recs[i].unk08;
    *o1 = s1;
    s2.kind = 0;
    s2.unk04 = -1;
    s2.unk08 = -1;
    s2.unk10 = 0x1e;
    *o2 = s2;
    return 0x1e;
}
