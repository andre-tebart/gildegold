/* 0x496610, decide: the strongest of five needs (action 5) */

int ai_Unk496610(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    AIX6 r6;
    int q[0x88];
    float w[5];
    unsigned int cnt[5], n[5];
    int ids[5], valid[5];
    float f, fb;
    unsigned int i;
    int best, second, ka, kb, v, id, r, sum;
    unsigned char c;

    best = 0;
    second = 0;
    ids[0] = 0x3a;
    ids[1] = 0x1a;
    ids[2] = 0x17;
    ids[3] = 0x18;
    ids[4] = 0x19;
    if (b)
        return 0;
    if (p->jobId == 0) {
        AIARG s1, s2;

        s2.kind = 0;
        s1.kind = 0xe;
        s1.unk04 = 0;
        s1.unk08 = 0;
        s2.unk04 = 0;
        s2.unk08 = 0;
        r = ai_Unk495a50(5, p->id, 0, &s1, &s2);
        if (r) {
            *o1 = s1;
            *o2 = s2;
        }
        return r;
    }
    gm_Unk503ee0(&r6, p->jobId);
    if (p->bld2 == 0 && p->kind != 3) {
        AIARG s1, s2;

        s1.kind = 0;
        s1.unk04 = 0;
        s1.unk08 = 0;
        s1.unk14 = 0.66f;
        s2.kind = 0;
        s2.unk04 = 0;
        s2.unk08 = 0;
        r = ai_Unk495a50(4, p->id, 1, &s1, &s2);
        if (r) {
            *o1 = s1;
            *o2 = s2;
        }
        return r;
    }
    if (he_FindEvent(2, 2, p->id, 0, 0x60))
        return 0;
    ka = util_Random(5);
    kb = util_Random(5);
    for (i = 0; i < 5; i++) {
        memset(q, 0, sizeof(q));
        v = r6.v[i];
        q[0] = 1;
        if (v > best) {
            if (best > 0) {
                kb = ka;
                second = best;
            }
            ka = i;
            best = v;
        } else if (v > second) {
            kb = i;
            second = v;
        }
        valid[i] = 1;
        cnt[i] = ord_GetRatingByte(i);
        if (!cnt[i])
            cnt[i] = 252;
        f = (ord_DispatchOrder(ids[i], p, q) & 2) ? -1.0f : 0.0f;
        c = p->unk080[i];
        if (c < 42)
            n[i] = 1;
        else if (c < 84)
            n[i] = 2;
        else if (c < 126)
            n[i] = 4;
        else if (c < 168)
            n[i] = 6;
        else if (c < 210)
            n[i] = 9;
        else
            n[i] = 12;
        n[i] = (int)(n[i] + f);
        if (n[i] > p->unk194)
            valid[i] = 0;
        else {
            fb = c;
            if (fb == 252.0f)
                valid[i] = 0;
            else
                w[i] = fb / cnt[i];
        }
    }
    sum = valid[kb] + valid[ka];
    if (sum == 0) {
        AIARG s1, s2;

        s1.kind = 0xf;
        s1.unk04 = 0;
        s1.unk08 = 0;
        s2.kind = 0;
        s2.unk04 = 0;
        s2.unk08 = 0;
        r = ai_Unk495a50(5, p->id, 0, &s1, &s2);
        if (r) {
            *o1 = s1;
            *o2 = s2;
        }
        return r;
    }
    if (sum == 2) {
        if (!util_Random(2))
            ka = kb;
    } else if (!valid[ka])
        ka = kb;
    if (p->kind == 3) {
        if (ka == 4) {
            if (!(p->flags1e8 & 4))
                return 0;
        } else if (ka == 3) {
            if (!(p->flags1e8 & 8))
                return 0;
        } else if (ka == 2 && !(p->flags1e8 & 2))
            return 0;
    } else if (ka > 1) {
        if (ka == 4)
            id = 0x15;
        else
            id = ka == 3 ? 0x17 : 0x14;
        if (!gm_Query(p->bld2->objList, 1, 0, id)) {
            AIARG s1, s2;

            s1.kind = 2;
            s1.unk04 = id;
            s1.unk08 = 1;
            s1.unk14 = 0.44f;
            s2.kind = 4;
            s2.unk04 = p->bld2->id;
            s2.unk08 = 1;
            r = ai_Unk495a50(5, p->id, 1, &s1, &s2);
            if (r) {
                *o1 = s1;
                *o2 = s2;
            }
            return r;
        }
    }
    o1->kind = 0xe;
    o1->unk04 = ka;
    o1->unk10 = n[ka];
    return 5;
}
