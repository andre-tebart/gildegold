/* 0x4960b0, decide: one of five needs (action 4) */

int ai_Unk4960b0(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    int q[0x88];
    float w[5];
    unsigned int cnt[5], n[5];
    int ids[5], valid[5];
    float f, fb;
    unsigned int i;
    int count, k, v, id, r;
    unsigned char c;

    count = 5;
    ids[0] = 0x3a;
    ids[1] = 0x1a;
    ids[2] = 0x17;
    ids[3] = 0x18;
    ids[4] = 0x19;
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
        r = ai_Unk495a50(4, p->id, 1, &s1, &s2);
        if (r) {
            *o1 = s1;
            *o2 = s2;
        }
        return r;
    }
    if (he_FindEvent(2, 2, p->id, 0, 0x60))
        return 0;
    if (o1->kind == 0xe) {
        k = o1->unk04;
        memset(q, 0, sizeof(q));
        q[0] = 1;
        f = (ord_DispatchOrder(ids[k], p, q) & 2) ? -1.0f : 0.0f;
        c = p->unk080[k];
        if (c < 42)
            v = 1;
        else if (c < 84)
            v = 2;
        else if (c < 126)
            v = 4;
        else if (c < 168)
            v = 6;
        else if (c < 210)
            v = 9;
        else
            v = 12;
        v = (int)(v + f);
        if (v < p->unk194)
            return 0;
        if (c == 252.0f)
            return 0;
        o1->unk10 = v;
        return 4;
    }
    for (i = 0; i < 5; i++) {
        memset(q, 0, sizeof(q));
        q[0] = 1;
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
        if (n[i] > p->unk194) {
            valid[i] = 0;
            count--;
        } else {
            fb = c;
            if (fb == 252.0f) {
                valid[i] = 0;
                count--;
            } else
                w[i] = fb / cnt[i];
        }
    }
    if (!count)
        return 0;
    k = util_RandomU(5);
    while (!valid[k])
        k = (k + 1) % 5;
    if (p->kind == 3) {
        if (k == 4) {
            if (!(p->flags1e8 & 4))
                return 0;
        } else if (k == 3) {
            if (!(p->flags1e8 & 8))
                return 0;
        } else if (k == 2 && !(p->flags1e8 & 2))
            return 0;
    } else if (k > 1) {
        if (k == 4)
            id = 0x15;
        else
            id = k == 3 ? 0x17 : 0x14;
        if (!gm_Query(p->bld2->objList, 1, 0, id)) {
            AIARG s1, s2;

            s1.kind = 2;
            s1.unk04 = id;
            s1.unk08 = 1;
            s1.unk14 = 0.44f;
            s2.kind = 4;
            s2.unk04 = p->bld2->id;
            s2.unk08 = 1;
            r = ai_Unk495a50(4, p->id, 1, &s1, &s2);
            if (r) {
                *o1 = s1;
                *o2 = s2;
            }
            return r;
        }
    }
    o1->kind = 0xe;
    o1->unk04 = k;
    o1->unk10 = n[k];
    return 4;
}
