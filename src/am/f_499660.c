/* 0x499660, decide: sabotage a rival's building (action 0x17) */

int ai_Unk499660(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    int buf[0x88];
    AIDEC d;
    AIARG s1, s2;
    AIBLDX *y;
    AIPER *ch;
    LARGE_INTEGER mask;
    float g;
    int m, x, v, r;

    m = gm_Unk509ee0(p, ai_OwnPlayer);
    if (!b) {
        if (p->bld2 == 0 && p->kind != 3) {
            s1.kind = 0;
            s1.unk04 = 0;
            s1.unk08 = 0;
            s2.kind = 0;
            s2.unk04 = 0;
            s2.unk08 = 0;
            r = ai_Unk495a50(0x17, p->id, 1, &s1, &s2);
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
            r = ai_Unk495a50(0x17, p->id, 1, &s1, &s2);
            if (r) {
                *o1 = s1;
                *o2 = s2;
            }
            return r;
        }
        gv_GetTableRecord(0x18, &d);
        if (d.cur && !util_Random(4) && !ai_Partner)
            return 0;
        if (o1->kind == 0) {
            mask.LowPart = 0x344015;
            mask.HighPart = 1;
            x = -1;
            if ((ai_Partner && (ai_Unk4a63f0((AIPER *)ai_Partner, 4, mask.QuadPart, 0, 100.0f, &x), x != -1))
                || ai_Unk4a63f0(p, 6, mask.QuadPart, 0, util_Random(0x20) + 19.0f, &x)) {
                gm_FindById((int)&y, 0, 0, x);
                if (y && y->owner != 0xffff && he_Unk570ea0((AIPER *)&ai_CharsT[y->owner], p)) {
                    memset(buf, 0, sizeof(buf));
                    buf[0] = 1;
                    v = (int)(gm_EquipmentValue(y, 100) * 0.05f + gm_GetWorth(p->id) * 0.02f);
                    ch = (AIPER *)&ai_CharsT[y->owner];
                    if (ord_DispatchOrder(0x1b, p, buf) & 2)
                        v /= 2;
                    if (!(m * 0.44f < v)) {
                        g = gm_Mix(p, ch, 2);
                        if (!(util_RandomF() + g >= 1.0f)) {
                            s1.kind = 0xe;
                            s1.unk04 = 2;
                            s1.unk08 = 1;
                            s1.unk14 = 0.0f;
                            s2.kind = 0;
                            s2.unk04 = 0;
                            s2.unk08 = 0;
                            r = ai_Unk495a50(0x17, p->id, 0, &s1, &s2);
                            if (r) {
                                *o1 = s1;
                                *o2 = s2;
                            }
                            return r;
                        }
                        if (he_Unk553d60((AIPER *)&ai_CharsT[y->owner], p)
                            && he_Unk553d10((AIPER *)&ai_CharsT[y->owner], p) && he_Unk553cc0((AIBLD *)y)
                            && he_Unk570ea0((AIPER *)&ai_CharsT[y->owner], p)) {
                            o1->kind = 4;
                            o1->unk04 = y->id;
                            o1->unk08 = 1;
                            o1->unk10 = v;
                            o1->unk14 = (float)(d.cur * 2 + 1);
                            return 0x17;
                        }
                    }
                }
            }
        }
    }
    return 0;
}
