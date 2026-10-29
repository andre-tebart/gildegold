/* 0x499c10, decide: beat up a character (action 0x18) */

int ai_Unk499c10(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    int buf[0x88];
    AIDEC d;
    AIARG s1, s2;
    AIPER *c;
    unsigned short cid;
    float g, t;
    int m, flags, v, r;

    m = gm_Unk509ee0(p, ai_OwnPlayer);
    if (!b) {
        memset(buf, 0, sizeof(buf));
        buf[0] = 1;
        v = (int)(gm_GetWorth(p->id) * 0.0085f);
        if (ord_DispatchOrder(0x1b, p, buf) & 2)
            v /= 2;
        if (!(m * 0.22f < v)) {
            if (p->bld2 == 0 && p->kind != 3) {
                s1.kind = 5;
                s1.unk04 = 4;
                s1.unk08 = 1;
                s2.kind = 0;
                s2.unk04 = 0;
                s2.unk08 = 0;
                r = ai_Unk495a50(0x18, p->id, 1, &s1, &s2);
                if (r) {
                    *o1 = s1;
                    *o2 = s2;
                }
                return r;
            }
            if (p->kind == 3) {
                if (!(p->flags1e8 & 2))
                    return 0;
            } else if (!gm_Query(p->bld2->objList, 2, 6, 0, 0x14)) {
                s1.kind = 2;
                s1.unk04 = 0x14;
                s1.unk08 = 1;
                s1.unk14 = 0.22f;
                s2.kind = 4;
                s2.unk04 = p->bld2->id;
                s2.unk08 = 1;
                r = ai_Unk495a50(0x18, p->id, 1, &s1, &s2);
                if (r) {
                    *o1 = s1;
                    *o2 = s2;
                }
                return r;
            }
            gv_GetTableRecord(0x15, &d);
            if (!d.cur || util_Random(3) || ai_Partner) {
                if (o1->kind == 0) {
                    flags = ((unsigned)&ai_Unka69140 + 0x4973c0);
                    t = util_Random(0x20) + 32.0f;
                    if (ai_Unk4a58d0(p, 1, &flags, 0, t, &cid)) {
                        c = (AIPER *)&ai_Chars[cid];
                        if (c->unk194 >= 2) {
                            g = gm_Mix(p, c, 3);
                            if (!(util_RandomF() + g >= 1.0f)) {
                                s1.kind = 0xe;
                                s1.unk04 = 3;
                                s1.unk08 = 1;
                                s1.unk14 = 0.0f;
                                s2.kind = 0;
                                s2.unk04 = 0;
                                s2.unk08 = 0;
                                r = ai_Unk495a50(0x18, p->id, 0, &s1, &s2);
                                if (r) {
                                    *o1 = s1;
                                    *o2 = s2;
                                }
                                return r;
                            }
                            if (he_Unk553d60(c, p) && he_Unk553d10(c, p)) {
                                o1->kind = 7;
                                o1->unk04 = c->netId;
                                o1->unk08 = 1;
                                o1->unk10 = v;
                                o1->unk14 = (float)(d.cur * 2 + 1);
                                return 0x18;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}
