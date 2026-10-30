/* 0x48f2b0, decide of action 0x43 */

int ai_Unk48f2b0(int a, AIPX *p, int b, AIARG *o1, AIARG *o2)
{
    struct { AIARG s1, s2; } L;
    AIBLD *bl;
    int v, lim, d, t;

    if (!ai_Unk48f250(p, &bl) || b)
        return 0;
    if (a != 0x43 && a != 0)
        return 0;
    if (he_FindEvent(2, 0, 0xd, 2, p->id))
        return 0;
    if (bl) {
        if ((unsigned char)ai_BTypesT[bl->type].unk247 >= 3 || (bl->flags5a & 0x41))
            return 0;
        t = bl->type + 1;
        L.s2.unk04 = t;
        L.s2.unk08 = 1;
        L.s1.kind = 4;
        L.s1.unk04 = bl->id;
        L.s1.unk08 = 1;
        v = (int)(gm_BuildingKindValue(t) * 0.3f);
        if (!ai_Unk4a5140(a, p, 0, &L.s2, &L.s1, 0))
            return 0;
        lim = (int)(gm_Unk509e30(p->inventory, ai_OwnPlayer) * 0.5f);
        if (lim < v) {
            d = v - lim;
            if (!((float)d / v < 0.25f))
                return 0;
            o1->unk08 = d;
            o1->kind = 0x12;
            return ai_Unk49eec0(0x43, p, 0, o1, o2);
        }
        *o1 = L.s2;
        *o2 = L.s1;
    } else {
        L.s2.kind = 5;
        L.s2.unk04 = 0x54;
        L.s2.unk08 = 1;
        if (!ai_Unk4a5140(a, p, 0, &L.s1, &L.s2, 1))
            return 0;
        v = gm_Unk509dc0(p, L.s1.unk04);
        lim = (int)(gm_Unk509e30(p->inventory, ai_OwnPlayer) * 0.5f);
        if (lim < v) {
            d = v - lim;
            if (!((float)d / v < 0.25f))
                return 0;
            o1->unk08 = d;
            o1->kind = 0x12;
            return ai_Unk49eec0(0x43, p, 0, o1, o2);
        }
        *o1 = L.s1;
        *o2 = L.s2;
    }
    return 0x43;
}
