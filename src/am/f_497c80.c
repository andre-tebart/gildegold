/* 0x497c80, execute of 0x497af0 */

int ai_Unk497c80(AIPER *p, AIARG *s1, AIARG *s2)
{
    AIBLD *bld;

    bld = gm_FindBuildingById(s2->unk04);
    if (!bld)
        return 0;
    if (s1->kind != 2)
        return 0;
    cm_SendExSellObject(bld->id, -1, s1->unk04, s1->unk08, ai_OwnPlayer, (int)gm_GoodsPrice(s1->unk04, ai_OwnPlayer));
    return 14;
}
