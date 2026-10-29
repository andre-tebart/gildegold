/* 0x48f010, decide kind 0x42: a master without a second level object wants one, or a building to pair
   with his own */

int ai_Unk48f010(int a, AIPX *p, int b, AIARGX *o1, AIARGX *o2)
{
    AIARGX s1, s2;
    AIBLD *bld, *b2;
    int r, v;
    union { struct { unsigned int lo, hi; } s; __int64 q; } m;

    if (p->kind == 3)
        return 0;
    if (a != 0x42 && a != 0)
        return 0;
    bld = gm_BuildQuery(2, 5, 0x1e, 4, p->id);
    if (!bld)
        return 0;
    if (!gm_Query(bld->objList, 2, 6, 0, 0x256)) {
        s1.kind = 2;
        s1.unk04 = 0x256;
        s1.unk08 = 1;
        s1.unk14 = 0.22f;
        s2.kind = 4;
        s2.unk04 = bld->id;
        s2.unk08 = 1;
        r = ai_Unk495a50(0x42, p->id, 1, &s1, &s2);
        if (r) {
            *o1 = s1;
            *o2 = s2;
        }
        return r;
    }
    if (!gv_FindEffect(bld->id) && !(p->flags1cc & 0x80000000)) {
        cm_SendSetField(p->netId, 0x1e8, 4, 0x80000000, 0);
        unk_Unk4eea70(bld);
        return 0;
    }
    m.s.lo = 0xe07c43b4;
    m.s.hi = 1;
    if (!((int (*)(AIPX *, int, __int64, int, float, int *))ai_Unk4a63f0)(p, 6, m.q, 0, (float)(util_Random(0x20) + 0x20), &v))
        return 0;
    gm_FindById((int)&b2, 0, 0, v);
    if (!b2)
        return 0;
    if (!ai_Unk4a67a0(p, bld, b2))
        return 0;
    o1->kind = 4;
    o1->unk04 = bld->id;
    o2->kind = 4;
    o2->unk04 = b2->id;
    return 0x42;
}
