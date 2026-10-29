/* 0x48e120, decide kind 0x40: the master's building needs an upgrade he can nearly pay */

int ai_Unk48e120(int a, AIPX *p, int b, AIARGX *o1, AIARGX *o2)
{
    AIARGX s1, s2;
    AIBLD *bld;
    int cost, lim, m, need, t;

    if (ai_Unk48e070(p, (int *)&bld) && !b && (a == 0x40 || a == 0) && !he_FindEvent(2, 0, 0xd, 2, p->id)) {
        if (bld) {
            t = bld->type;
            if ((unsigned char)ai_BTypes[t].unk247 >= 3)
                return 0;
            if (bld->flags5a & 0x41)
                return 0;
            s2.unk04 = t + 1;
            s2.unk08 = 1;
            s1.kind = 4;
            s1.unk04 = bld->id;
            s1.unk08 = 1;
            cost = (int)(gm_BuildingKindValue(t + 1) * 0.3f);
            if (!ai_Unk4a5140(a, p, 0, &s2, &s1, 0))
                return 0;
            m = gm_Unk509e30(p->inventory, ai_OwnPlayer);
            lim = (int)(m * 0.95f);
            if (lim < cost) {
                need = cost - lim;
                if ((float)need / cost < 0.25f) {
                    o1->unk08 = need;
                    o1->kind = 0x12;
                    return ai_Unk49eec0(0x40, p, 0, o1, o2);
                }
                return 0;
            }
            *o1 = s2;
            *o2 = s1;
            goto ok;
        }
        s1.kind = 5;
        s1.unk04 = 0x4b;
        s1.unk08 = 1;
        if (!ai_Unk4a5140(a, p, 0, &s1, &s2, 1))
            return 0;
        cost = gm_Unk509dc0(p, s1.unk04);
        m = gm_Unk509e30(p->inventory, ai_OwnPlayer);
        lim = (int)(m * 0.95f);
        if (lim < cost) {
            need = cost - lim;
            if ((float)need / cost < 0.25f) {
                o1->unk08 = need;
                o1->kind = 0x12;
                return ai_Unk49eec0(0x40, p, 0, o1, o2);
            }
            return 0;
        }
        *o1 = s1;
        *o2 = s2;
        goto ok;
    }
    return 0;
ok:
    return 0x40;
}
