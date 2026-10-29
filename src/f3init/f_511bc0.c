/* 0x511bc0, fills the markets of the player p: for every one of the 0x55 goods records of the player's stock
   (0x7d6a70 + p * 0x2a90) that has a market object (gm_FindMarketObject) a random part of the record's demand
   is sent to the market (cm_SendExSellObject): 0.5 .. 1.5 of it for the goods kinds 0x17 / 0x25, 0.2 .. 1.2
   for the others. 0x55 fills all of the player's slots at the start of a game */


void init_FillMarkets(int p)
{
    int i;
    char *pr;
    INIT_STOCK *g;
    INIT_OBJ *m;
    int v;
    float f;

    i = 0x55;
    g = &init_StockBlock[p].rec[0];
    do {
        pr = ai_Goods + g->item * 0x41;
        m = (INIT_OBJ *)gm_FindMarketObject(g->item);
        if (m != 0) {
            if (*pr == 0x17 || *pr == 0x25)
                f = util_RandomF() + 0.5;
            else
                f = util_RandomF() + 0.2;
            v = (int)(f * g->demand);
            cm_SendExSellObject(m->objId, -1, g->item, v, ai_OwnPlayer, 0);
        }
        g++;
        i--;
    } while (i != 0);
}
