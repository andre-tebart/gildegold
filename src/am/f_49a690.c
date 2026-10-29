/* 0x49a690, decide callback 0x1c: one of twelve goods (0x155..0x2b6) the character lacks */

int ai_Unk49a690(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int k, i, r;
    unsigned int flags;
    unsigned short id;
    AIARG s1;
    AIARG s2;
    int st[3];
    G8REC recs[12];
    int info[4];

    k = a;
    if (a == 0)
        k = 0x1c;
    if (f != 0 || a != 0)
        return 0;
    memset(recs, 0, sizeof(recs));
    recs[0].item = 0x155;
    recs[1].item = 0x15d;
    recs[2].item = 0x157;
    recs[3].item = 0x15f;
    recs[4].item = 0x159;
    recs[5].item = 0x165;
    recs[6].item = 0x166;
    recs[7].item = 0x16b;
    recs[8].item = 0x170;
    recs[9].item = 0x167;
    recs[10].item = 0x229;
    recs[11].item = 0x2b6;
    ai_Unk4a5760(p, 12, (int *)recs, st);
    if (st[0] == 6 && st[2] == 0)
        return 0;
    if (st[1] == 12)
        return 0;
    if (st[0] < 6) {
        if (st[2] == 0) {
            if (amt_GetKind(((unsigned char *)p)[0x166], info) == 0)
                i = 7;
            else if ((unsigned char)info[2] < 4)
                i = 0;
            else
                i = (unsigned char)info[2] < 7 ? 2 : 4;
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
    s2.kind = 1;
    s2.unk04 = recs[i].unk08;
    s2.unk08 = 1;
    s1.kind = 0;
    s1.unk04 = -1;
    s1.unk08 = -1;
    s1.unk10 = 0x1c;
    if (i == 9) {
        flags = (((unsigned char *)p)[9] != 0 ? 1 : 2) | ((unsigned)&ai_Unka69140 + 0x4973c0);
        if (G8_4A58D0(p, 1, &flags, 10.0f, (float)util_Random(0x20) + 40.0f, &id) == 0)
            return 0;
        s1.kind = 7;
        s1.unk04 = ai_Chars[id].netId;
        s1.unk08 = -1;
    } else if (i == 10) {
        flags = (((unsigned char *)p)[9] != 1 ? 1 : 2) | ((unsigned)&ai_Unka69140 + 0x4973c0);
        if (G8_4A58D0(p, 1, &flags, 10.0f, (float)util_Random(0x20) + 40.0f, &id) == 0)
            return 0;
        s1.kind = 7;
        s1.unk04 = ai_Chars[id].netId;
        s1.unk08 = -1;
    } else if (i == 8) {
        r = ai_Unk495a50(k, p->id, 3, &s2, &s1);
        if (r == 0)
            return r;
        *o1 = s2;
        *o2 = s1;
        return r;
    }
    *o1 = s2;
    *o2 = s1;
    return 0x1c;
}
