/* 0x49ab60, decide callback 0x1d: one of two goods (0x173, 0x15b) */

int ai_Unk49ab60(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int k, i, r;
    int st[3];
    AIARG s1;
    AIARG s2;
    G8REC recs[2];

    k = a;
    if (a == 0)
        k = 0x1d;
    if (f != 0)
        return 0;
    memset(recs, 0, sizeof(recs));
    recs[0].item = 0x173;
    recs[1].item = 0x15b;
    ai_Unk4a5760(p, 2, (int *)recs, st);
    if (st[0] == 6 && st[2] == 0)
        return 0;
    if (st[1] == 2)
        return 0;
    if (st[0] < 6) {
        if (st[2] == 0) {
            i = util_RandomU(2);
            while (recs[i].unk04 != 0)
                i = (i + 1) % 2;
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
    i = util_RandomU(2);
    while (recs[i].unk04 < 1)
        i = (i + 1) % 2;
    s2.kind = 1;
    s2.unk08 = 1;
    s2.unk04 = recs[i].unk08;
    s1.kind = 0;
    s1.unk04 = -1;
    s1.unk08 = -1;
    s1.unk10 = 0x1d;
    if (i == 0) {
        r = ai_Unk495a50(k, p->id, 4, &s2, &s1);
        if (r == 0)
            return r;
        *o1 = s2;
        *o2 = s1;
        return r;
    }
    *o1 = s2;
    *o2 = s1;
    return 0x1d;
}
