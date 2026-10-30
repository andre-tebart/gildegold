/* 0x49bd90, decide callback 0x22: one of ten goods (0x17c..0x2b3) */

int ai_Unk49bd90(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int k, i, r;
    AIARG s1;
    AIARG s2;
    int st[3];
    G8REC recs[10];

    k = a;
    if (a == 0)
        k = 0x22;
    if (f != 0 || a != 0)
        return 0;
    memset(recs, 0, sizeof(recs));
    recs[0].item = 0x17c;
    recs[1].item = 0x171;
    recs[2].item = 0x16f;
    recs[3].item = 0x15c;
    recs[4].item = 0x15a;
    recs[5].item = 0x22e;
    recs[6].item = 0x22a;
    recs[7].item = 0x2b4;
    recs[8].item = 0x2b5;
    recs[9].item = 0x2b3;
    ai_Unk4a5760(p, 10, (int *)recs, st);
    if (st[0] == 6 && st[2] == 0)
        return 0;
    if (st[1] == 10)
        return 0;
    if (st[0] < 6) {
        if (st[2] == 0) {
            i = util_RandomU(10);
            while (recs[i].unk04 != 0)
                i = (i + 1) % 10;
            s1.unk04 = recs[i].item;
            s1.unk08 = 1;
            s2.unk08 = 1;
            s1.kind = 2;
            s1.unk14 = 0.05f;
            s2.kind = 7;
            s2.unk04 = p->netId;
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
    i = util_RandomU(10);
    while (recs[i].unk04 < 1)
        i = (i + 1) % 10;
    s1.kind = 1;
    s1.unk08 = 1;
    s2.kind = 0;
    s1.unk04 = recs[i].unk08;
    s2.unk04 = -1;
    s2.unk08 = -1;
    s2.unk10 = 0x22;
    if (i == 0) {
        r = ai_Unk495a50(k, p->id, 4, &s1, &s2);
    } else if (i < 4) {
        r = ai_Unk495a50(k, p->id, 3, &s1, &s2);
    } else {
        *o1 = s1;
        *o2 = s2;
        return 0x22;
    }
    if (r == 0)
        return r;
    *o1 = s1;
    *o2 = s2;
    return r;
}
