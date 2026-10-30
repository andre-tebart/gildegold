/* 0x4fec10, the market sync of the player p: for the own player (0x6f1bc5) the goods lists are rebuilt as in 0x4ff190 (the flag +4 marks goods in stock or needed), then every goods record is offered to / requested from the market building (0x4c4880 messages: a marked good that is not in stock is requested (1), an unmarked one in stock is sold with its stock (+0xe)) and sent to the player (0x4c56d0); the market object counts (0x92 .. 0x97, 0x2db) are written back to the byte +0x1c of the object of kind 0x11. For another player: the goods of its two market objects (0x1db / 0x1dc) are randomly marked (a byte at +0x28 of the record grows by random 0x18 + 0 .. 7 per call; below 0x81 means offered) and sent (0x4c3b30 .. 0x4c3b50). ROUGH */

#define GM_WAIT(h) while (cm_GetSlotState(h) == 0) sv_PumpNet()

void gm_SyncMarket(unsigned p)
{
    int list[8];
    unsigned rec[0x20];
    char *g, *g2;
    unsigned char *b;
    int o, o2, i, j, k, cnt, m, h, a1;
    int *lp;
    unsigned short id;
    unsigned char c28;

    list[0] = 0x92;
    list[1] = 0x93;
    list[2] = 0x94;
    list[3] = 0x95;
    list[4] = 0x96;
    list[5] = 0x97;
    list[6] = 0x2db;
    list[7] = 0;
    ai_MarketBuildingI = gm_BuildQuery(1, 5, 10);
    if (ai_MarketBuildingI == 0)
        return;
    if (p == ai_OwnPlayer) {
        for (i = 0; i < 0x55; i++)
            *(int *)(GM_GOODS(p, i) + 4) = 0;
        b = (unsigned char *)gm_BuildQuery(1, 6);
        while (b != 0) {
            if (ai_BTypes[*b * 0x24d] != 10) {
                o = (int)gm_Query(*(int *)(b + 0x5d), 1, 0, 0xff);
                if (o != 0) {
                    for (i = 0; i < 0x55; i++) {
                        g = GM_GOODS(p, i);
                        id = *(unsigned short *)g;
                        if (id != 0 && gm_Query(*(int *)(o + 0x14), 1, 0, id) != 0) {
                            *(int *)(g + 4) = 1;
                            for (j = 0; j < 0x55; j++) {
                                g2 = GM_GOODS(p, j);
                                for (k = 0; k < 4; k++)
                                    if (*(unsigned short *)g2 == *(unsigned short *)(ai_Goods + id * 0x41 + 0x2e + k * 2))
                                        *(int *)(g2 + 4) = 1;
                            }
                        }
                    }
                }
            }
            b = (unsigned char *)gm_BuildNext();
        }
        for (i = 0; i < 0x55; i++) {
            g = GM_GOODS(p, i);
            memcpy(rec, GM_GOODS(p, i), 0x80);
            o = (int)gm_Query(*(int *)(ai_MarketBuildingI + 0x5d), 1, 0, *(unsigned short *)(g + 0x3c));
            if (o != 0) {
                o2 = (int)gm_Query(*(int *)(o + 0x14), 1, 0, 0xff);
                if (o2 == 0) {
                    h = ((int (*)(void *, int, int, int, int, int))cm_SendExSellObject)(*(int *)(o + 2), -1, 0xff, 1, ai_OwnPlayer, 0);
                    GM_WAIT(h);
                    o2 = cm_GetSlotValue(h);
                }
                if (*(int *)(g + 4) == 0) {
                    m = (int)gm_Query(*(int *)(o2 + 0x14), 1, 0, *(unsigned short *)g);
                    if (m != 0) {
                        h = ((int (*)(int, void *, void *, void *, int, int))cm_SendExSellObject)(-1, *(int *)(o2 + 2), *(unsigned short *)g, *(int *)(m + 0xe), ai_OwnPlayer, 0);
                        GM_WAIT(h);
                    }
                } else {
                    m = (int)gm_Query(*(int *)(o2 + 0x14), 1, 0, *(unsigned short *)g);
                    if (m == 0) {
                        h = ((int (*)(void *, int, void *, int, int, int))cm_SendExSellObject)(*(int *)(o2 + 2), -1, *(unsigned short *)g, 1, ai_OwnPlayer, 0);
                        GM_WAIT(h);
                    }
                }
                ai_Unk4c56d0(rec, p);
            }
        }
        lp = list;
        k = 0x92;
        do {
            cnt = 0;
            o = (int)gm_Query(*(int *)(ai_MarketBuildingI + 0x5d), 1, 0, k);
            if (o != 0 && (o2 = (int)gm_Query(*(int *)(o + 0x14), 1, 0, 0xff)) != 0) {
                o2 = (int)gm_Query(*(int *)(o2 + 0x14), 1, 5);
                while (o2 != 0) {
                    cnt++;
                    o2 = (int)gm_QueryNext();
                }
                o = (int)gm_Query(*(int *)(o + 0x14), 1, 4, 0x11);
                if (o != 0 && (int)(unsigned char)*(char *)(o + 0x1c) < cnt) {
                    cnt -= (unsigned char)*(char *)(o + 0x1c);
                    cm_BeginFieldBlock(o, *(int *)(o + 2));
                    cm_AddFieldValues(1, 1, ((char *)o - cm_FieldObject) + 0x1c, &cnt);
                    cm_SendFieldDeltas();
                }
            }
            k = lp[1];
            lp++;
        } while (k != 0);
        return;
    }
    o = gm_FindPlayerBuilding(p);
    if (o != 0 && (a1 = (int)gm_Query(*(int *)(o + 0x5d), 2, 0, 0x1db, 6)) != 0 && (o2 = (int)gm_Query(*(int *)(o + 0x5d), 2, 0, 0x1dc, 6)) != 0) {
        cm_BeginSendGroup();
        for (i = 0; i < 0x55; i++) {
            memcpy(rec, GM_GOODS(p, i), 0x80);
            if ((short)rec[0] != 0) {
                c28 = *((unsigned char *)rec + 0x28);
                if (gm_Query(*(int *)(a1 + 0x14), 1, 0, rec[0] & 0xffff) == 0 && gm_Query(*(int *)(o2 + 0x14), 1, 0, rec[0] & 0xffff) == 0) {
                    rec[1] = 0;
                } else if (c28 == 0) {
                    c28 = (unsigned char)util_RandomU(0xff);
                    rec[1] = c28 < 0x81;
                } else {
                    c28 = c28 + (char)util_RandomU(8) + 0x18;
                    rec[1] = c28 < 0x81;
                }
                *((unsigned char *)rec + 0x28) = c28;
                ai_Unk4c56d0(rec, p);
            }
        }
        cm_EndSendGroup();
        while (cm_SendGroupState() == 0)
            sv_PumpNet();
    }
}
