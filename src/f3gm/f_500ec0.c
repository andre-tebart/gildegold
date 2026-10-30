/* 0x500ec0, the market object of the town market building (0x6f1bc5's market 0x6f1bc8) that holds the goods id: looks through its objects 0x92 .. 0x97 / 0x2db for one with the goods id (query (2, 6, 0, id)) and returns its 0x116 object, 0 when none */

int gm_FindMarketObject(int id)
{
    int l[8];
    int o, i;

    if (ai_MarketBuilding != 0) {
        l[0] = (int)gm_Query(*(int *)((char *)ai_MarketBuilding + 0x5d), 1, 0, 0x92);
        l[1] = (int)gm_Query(*(int *)((char *)ai_MarketBuilding + 0x5d), 1, 0, 0x93);
        l[2] = (int)gm_Query(*(int *)((char *)ai_MarketBuilding + 0x5d), 1, 0, 0x94);
        l[3] = (int)gm_Query(*(int *)((char *)ai_MarketBuilding + 0x5d), 1, 0, 0x95);
        l[4] = (int)gm_Query(*(int *)((char *)ai_MarketBuilding + 0x5d), 1, 0, 0x96);
        l[5] = (int)gm_Query(*(int *)((char *)ai_MarketBuilding + 0x5d), 1, 0, 0x97);
        l[6] = (int)gm_Query(*(int *)((char *)ai_MarketBuilding + 0x5d), 1, 0, 0x2db);
        l[7] = 0;
        i = 0;
        for (o = l[0]; o != 0; o = l[i]) {
            if (gm_Query(*(int *)(o + 0x14), 2, 6, 0, id) != 0)
                return (int)gm_Query(*(int *)(l[i] + 0x14), 1, 0, 0x116);
            i++;
        }
    }
    return 0;
}
