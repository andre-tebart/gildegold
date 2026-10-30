/* 0x497af0, decide: sell a good (o1, kind 2) to a building (o2, kind 4) with room for it (action 14) */

int ai_Unk497af0(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    AIBLD *bld;
    AIBTX *t;
    AIOBJ *o;
    AIJOB *j;
    AIPER *c;
    int v, n;

    if (p->kind == 3)
        return 0;
    if (b != 1)
        return 0;
    if (o1->kind != 2)
        return 0;
    if (AI_GOODX[o1->unk04].type != 2)
        return 0;
    if (o2->kind != 4)
        return 0;
    bld = gm_FindBuildingById(o2->unk04);
    if (!bld)
        return 0;
    if (bld->owner == 0xffff)
        return 0;
    c = (AIPER *)&ai_CharsT[bld->owner];
    v = gm_Unk509ee0(c, ai_OwnPlayer);
    if (o1->unk14 > 0.0f && o1->unk14 < 1.0f)
        v = (int)(v * o1->unk14);
    if (v < (int)gm_GoodsPrice(o1->unk04, ai_OwnPlayer))
        return 0;
    t = &AI_BTX[bld->type];
    n = 0;
    for (o = gm_Query(bld->objList, 1, 4, 2); o; o = gm_QueryNext())
        if (o->id != 0xfd)
            n++;
    if (n >= t->unk21)
        return 0;
    for (j = he_FindEvent(2, 0, 0x1f, 3, bld->id); j; j = he_NextEvent())
        n++;
    return n >= t->unk21 ? 0 : 14;
}
