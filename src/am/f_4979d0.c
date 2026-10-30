/* 0x4979d0, execute of 0x497660: hands the good over (buys it for action 0x39) */

int ai_Unk4979d0(AIPER *p, AIARG *s1, AIARG *s2)
{
    AIOBJ *o, *it;
    int id, price;

    if (s2->kind == 1) {
        o = (AIOBJ *)gm_FindObjectInst(s2->unk04);
        if (!o)
            return 0;
        id = o->objId;
    } else if (s2->kind == 4) {
        AIBLD *bld = gm_FindBuildingById(s2->unk04);
        if (!bld)
            return 0;
        id = bld->id;
    } else if (s2->kind == 7) {
        AIPER *c = AI_505DF0(s2->unk04);
        if (!c)
            return 0;
        id = c->netId;
    } else
        return 0;
    if (id == -1)
        return 0;
    if (s1->kind != 2)
        return 0;
    if (s1->unk10 == 0x39) {
        price = (int)gm_GoodsPrice(s1->unk04, ai_OwnPlayer);
        gm_FindById(0, (unsigned short **)&it, 0, s1->unk0c);
        if (!it)
            return 0;
        cm_SendMoveGoods(-1, it->objId, ai_OwnPlayer, price);
        cm_SendExSellObjectI(id, -1, s1->unk04, s1->unk08, ai_OwnPlayer, 0);
        return 0x39;
    }
    cm_SendExSellObjectI(id, -1, s1->unk04, s1->unk08, ai_OwnPlayer, (int)gm_GoodsPrice(s1->unk04, ai_OwnPlayer));
    return 11;
}
