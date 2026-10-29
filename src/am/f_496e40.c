/* 0x496e40, decide: talk a companion into money (action 9) */

int ai_Unk496e40(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    AIPER *list[6];
    AIPER *c;
    float g, best, hi;
    int m, lim, n, j, bi, lo, hi2, r;

    if (b)
        return 0;
    if (p->bld2 == 0 && p->kind != 3) {
        AIARG s1, s2;

        s1.kind = 0;
        s1.unk04 = 0;
        s1.unk08 = 0;
        s1.unk14 = 0.66f;
        s2.kind = 0;
        s2.unk04 = 0;
        s2.unk08 = 0;
        r = ai_Unk495a50(9, p->id, 1, &s1, &s2);
        if (r) {
            *o1 = s1;
            *o2 = s2;
        }
        return r;
    }
    if ((p->flags1e8 & 0x400) && util_Random(8))
        return 0;
    if (p->kind == 3) {
        if (!(p->flags1e8 & 2))
            return 0;
    } else if (!gm_Query(p->bld2->objList, 2, 6, 0, 0x14)) {
        AIARG s1, s2;

        s1.kind = 2;
        s1.unk04 = 0x14;
        s1.unk08 = 1;
        s1.unk14 = 0.22f;
        s2.kind = 4;
        s2.unk04 = p->bld2->id;
        s2.unk08 = 1;
        r = ai_Unk495a50(9, p->id, 1, &s1, &s2);
        if (r) {
            *o1 = s1;
            *o2 = s2;
        }
        return r;
    }
    m = gm_Unk509ee0(p, ai_OwnPlayer);
    lim = (int)(m * 0.33f);
    if (o1->kind == 0) {
        if (p->unk168) {
            bi = -1;
            best = 100.0f;
            n = amt_GetOfficeApplicants(p->unk168, 6, list);
            if (n <= 0)
                return 0;
            for (j = 0; j < n; j++) {
                if (list[j] != AI_6F0468) {
                    if ((g = gm_Liking(p->id, list[j]->id, 1)) < best) {
                        best = g;
                        bi = j;
                    }
                }
            }
            if (bi < 0)
                return 0;
            c = list[bi];
        } else {
            unsigned short cid;
            int flags;

            if (p->flags1cc & 0x1c000)
                return 0;
            flags = 0x10000500;
            if (!ai_Unk4a58d0(p, 1, &flags, 0, util_Random(25) + 10.0, &cid))
                return 0;
            c = (AIPER *)&ai_Chars[cid];
        }
        if (!c)
            return 0;
        o1->kind = 7;
        o1->unk04 = c->netId;
        o1->unk08 = 1;
        lo = (int)max(abs(gm_GetWorth(p->id) + gm_GetWorth(c->id)) / 1275.0f, 1600.0f);
        if (lo > lim)
            return 0;
        hi2 = min(lim, lo + 320000);
        o2->kind = 0x12;
        o2->unk04 = 0;
        o2->unk08 = (int)(util_RandomF() * (hi2 - lo) + lo);
        o2->unk10 = (int)(o2->unk08 * 500.0f / (gm_GetWorth(p->id) + gm_GetWorth(c->id)));
        return 9;
    }
    if (o1->kind == 7) {
        c = AI_505DF0(o1->unk04);
        if (!c || c == AI_6F0468)
            return 0;
        hi2 = (int)max(lim * 0.01f, 3200.0f);
        hi = min(lim * 0.15f, 160000.0f);
        o2->kind = 0x12;
        o2->unk04 = 0;
        o2->unk08 = (int)(util_RandomF() * ((int)hi - hi2) + hi2);
        o2->unk10 = (int)(o2->unk08 * 500.0f / (gm_GetWorth(p->id) + gm_GetWorth(c->id)));
        return 9;
    }
    return 0;
}
