/* 0x49c880, execute callback 0x0c: the character buys the good s1->unk04 from s2->unk04 at its price */

int ai_Unk49c880(G5CH *p, AIARG *s1, AIARG *s2)
{
    if (s1->kind != 2)
        return 0;
    cm_SendExSellObjectI(p->netId, s2->unk04, s1->unk04, s1->unk08, ai_OwnPlayer,
                 (int)gm_GoodsPrice(s1->unk04, ai_OwnPlayer));
    return 0xc;
}
