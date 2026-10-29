/* 0x4908d0, the decide callback of "Handle Grave" */

int ai_Unk4908d0(int a, AGCH *p, int b, AGARG *o1, AGARG *o2)
{
    AIBLD *bld;
    AGCH *q;
    AIBLD *list[8];
    int *r, *r2;
    int n, i, k, v, m, t2;

    r = 0;
    if (b != 0 || a != 0 || p->unk050 == 0x7fff)
        return 0;
    if (ai_Time.hour % 6 != p->netId % 6)
        return 0;
    if (ai_Time.year % 3 != p->netId % 3)
        return 0;
    if (p->kind != 5) {
        q = AG_CH(p->unk05c);
        if (q != 0) {
            if (q->kind == 5)
                return 0;
            if (q->unk009 == 0 && ai_WorksForMaster((AICHAR *)q) != 0)
                return 0;
        }
    }
    memset(list, 0, sizeof(list));
    bld = ai_Blds;
    n = 0;
    for (i = 0; i < 256; i++, bld = (AIBLD *)((char *)bld + 0xa9)) {
        if (bld->type != 0 && (bld->flags5a & 1) == 0 && ai_BTypes[bld->type].kind == 0x1d) {
            if (n < 8)
                list[n++] = bld;
            r = gv_FindEntry(p, bld, 0, unk_Unk4ef710, 0xff);
            if (r != 0)
                break;
        }
    }
    if (r != 0) {
        if (*r == 3)
            return 0;
        for (k = 3; k > *r; k--) {
            r2 = gv_FindEntry(0, bld, 0, gv_EntryIsFree, k);
            if (r2 != 0) {
                v = unk_Unk4f2440(p, *r, *r2, &i);
                m = gm_Unk509ee0(p, ai_OwnPlayer);
                if ((int)(m * 0.22f) <= v)
                    return 0;
                o1->kind = 5;
                o1->unk0c = bld->id;
                o1->unk04 = *r2;
                o1->unk08 = v;
                o2->unk04 = *r;
                o2->kind = AG_4EF6B0(bld->unk65, r);
                return 0x3e;
            }
        }
        return 0;
    }
    if (n == 0)
        return 0;
    if (util_Random(8) + p->unk00a < 0x23)
        return 0;
    bld = list[util_Random(n)];
    r2 = gv_FindEntry(0, bld, 0, gv_EntryIsFree, 0xff);
    if (r2 == 0)
        return 0;
    {
        int t3;

        v = unk_Unk4f2330(p, 0xff, *r2, 1, &t3);
        m = gm_Unk509ee0(p, ai_OwnPlayer);
    }
    if ((int)(m * 0.22f) < v)
        return 0;
    o1->kind = 4;
    o1->unk0c = bld->id;
    o1->unk04 = *r2;
    o1->unk08 = v;
    return 0x3e;
}
