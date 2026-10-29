/* 0x49b120, decide callback 0x1f: good 0x17e (or now and then 0x175) */

int ai_Unk49b120(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int k, r;
    int st[3];
    G8REC rec;
    AIARG s1;
    AIARG s2;

    k = a;
    if (a == 0)
        k = 0x1f;
    if (f != 0)
        return 0;
    rec.item = 0;
    rec.unk04 = 0;
    rec.unk08 = 0;
    rec.unk0c = 0;
    if (*(int *)((char *)p + 0x2c) != 0)
        rec.item = 0x17e;
    else if (util_Random(3) != 0)
        return 0;
    else
        rec.item = 0x175;
    ai_Unk4a5760(p, 1, (int *)&rec, st);
    if (st[0] == 6 && st[2] == 0)
        return 0;
    if (st[1] == 1)
        return 0;
    if (st[0] < 6) {
        if (st[2] == 0) {
            s1.unk04 = rec.item;
            s2.unk04 = p->netId;
            s1.kind = 2;
            s1.unk08 = 1;
            s1.unk14 = 0.05f;
            s2.kind = 7;
            s2.unk08 = 1;
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
    s1.unk04 = -1;
    s1.unk08 = -1;
    s2.unk04 = rec.unk08;
    s2.kind = 1;
    s2.unk08 = 1;
    s1.kind = 0;
    s1.unk10 = 0x1f;
    r = ai_Unk495a50(k, p->id, 4, &s2, &s1);
    if (r == 0)
        return r;
    *o1 = s2;
    *o2 = s1;
    return r;
}
