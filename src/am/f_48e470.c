/* 0x48e470, decide kind 0x41: a master without a workshop level object wants one built, otherwise one of the
   four ways to pay (0x48e620 .. 0x48e9f0) */

int ai_Unk48e470(int a, AIPX *p, int b, AIARGX *o1, AIARGX *o2)
{
    AIARGX s1, s2;
    AIBLD *bld;
    int r;

    if (p->kind == 3)
        return 0;
    if (b)
        return 0;
    if (a != 0x41 && a != 0)
        return 0;
    bld = gm_BuildQuery(2, 5, 0x1e, 4, p->id);
    if (!bld)
        return 0;
    if (!gm_Query(bld->objList, 2, 6, 0, 0x266)) {
        s1.kind = 2;
        s1.unk04 = 0x266;
        s1.unk08 = 1;
        s1.unk14 = 0.22f;
        s2.kind = 4;
        s2.unk04 = bld->id;
        s2.unk08 = 1;
        r = ai_Unk495a50(0x41, p->id, 1, &s1, &s2);
        if (r) {
            *o1 = s1;
            *o2 = s2;
        }
        return r;
    }
    if (he_Unk56f170(bld->id))
        return 0;
    switch (util_Random(4)) {
    case 0:
        r = ai_Unk48e620(p, &s1, &s2);
        break;
    case 1:
        r = ai_Unk48e730(p, &s1, &s2);
        break;
    case 2:
        r = ai_Unk48e830(p, &s1, &s2);
        break;
    case 3:
        r = ai_Unk48e9f0(p, &s1, &s2);
        break;
    default:
        return 0;
    }
    if (!r)
        return 0;
    *o1 = s1;
    *o2 = s2;
    o1->unk0c = bld->id;
    return 0x41;
}
