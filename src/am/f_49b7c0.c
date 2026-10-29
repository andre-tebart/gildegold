/* 0x49b7c0, decide callback 0x21: one of twelve goods (0x169..0x29c), some given to one or two characters */

int ai_Unk49b7c0(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int k, i, n, r;
    unsigned int flags[2];
    unsigned short id[2];
    int v;
    LARGE_INTEGER mask;
    G8REC *rp;
    AIARG s1;
    AIARG s2;
    int st[3];
    G8REC recs[12];

    k = a;
    if (a == 0)
        k = 0x1c;
    if (f != 0 || a != 0)
        return 0;
    memset(recs, 0, sizeof(recs));
    recs[0].item = 0x169;
    recs[1].item = 0x16a;
    recs[2].item = 0x177;
    recs[3].item = 0x178;
    recs[4].item = 0x17d;
    recs[5].item = 0x16d;
    recs[6].item = 0x17b;
    recs[7].item = 0x22c;
    recs[8].item = 0x22b;
    recs[9].item = 0x22d;
    recs[10].item = 0x29a;
    recs[11].item = 0x29c;
    ai_Unk4a5760(p, 12, (int *)recs, st);
    if (st[0] == 6 && st[2] == 0)
        return 0;
    if (st[1] == 12)
        return 0;
    if (st[0] < 6) {
        if (st[2] == 0) {
            i = util_Random(12);
            while (recs[i].unk04 != 0)
                i = (i + 1) % 12;
            s1.unk04 = recs[i].item;
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
    i = util_Random(12);
    while (recs[i].unk04 < 1)
        i = (i + 1) % 12;
    s1.kind = 1;
    s1.unk08 = 1;
    s1.unk14 = 0.05f;
    rp = &recs[i];
    s1.unk04 = recs[i].unk08;
    if (rp->item == 0x169 || rp->item == 0x16d) {
        flags[0] = ((unsigned)&ai_Unka69140 + 0x4973c0);
        flags[1] = ((unsigned)&ai_Unka69140 + 0x4973c0);
        n = 2;
    } else {
        n = 1;
        if (rp->item == 0x29a)
            flags[0] = ((unsigned)&ai_Unka69140 + 0x497bc0);
        else
            flags[0] = ((unsigned)&ai_Unka69140 + 0x4973c0);
    }
    if (rp->item == 0x177)
        flags[0] |= 0x10000;
    if (rp->item == 0x178) {
        mask.LowPart = 0xa07c4390;
        mask.HighPart = 1;
        v = -1;
        if (!(ai_Partner && (ai_Unk4a63f0((AIPER *)ai_Partner, 4, mask.QuadPart, 0, 100.0f, &v), v != -1))
            && !ai_Unk4a63f0((AIPER *)p, 6, mask.QuadPart, 0, (float)util_Random(0x40), &v))
            return 0;
    } else if (G8_4A58D0(p, n, flags, 0.0f, (float)util_Random(0x40), id) == 0) {
        return 0;
    }
    switch (rp->item) {
    case 0x169:
    case 0x16d:
        s2.unk04 = ai_Chars[id[0]].netId;
        s2.unk08 = ai_Chars[id[1]].netId;
        s2.kind = 7;
        break;
    case 0x177:
        s2.kind = 4;
        s2.unk04 = *(int *)((char *)ai_Chars[id[0]].bld2 + 1);
        s2.unk08 = -1;
        break;
    case 0x178:
        s2.kind = 4;
        s2.unk04 = v;
        s2.unk08 = -1;
        break;
    case 0x16a:
    case 0x17b:
    case 0x17d:
    case 0x22b:
    case 0x22c:
    case 0x22d:
    case 0x29a:
    case 0x29c:
        s2.unk08 = -1;
        s2.unk04 = ai_Chars[id[0]].netId;
        s2.kind = 7;
        break;
    default:
        return 0;
    }
    s2.unk10 = 0x21;
    r = 0x21;
    if (i > 1) {
        r = ai_Unk495a50(k, p->id, 4, &s1, &s2);
        if (r == 0)
            return r;
    }
    *o1 = s1;
    *o2 = s2;
    return r;
}
