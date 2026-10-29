/* 0x54d260, a Ghidra miss (disassembled; 1328 B): the run handler of the inn simulation ("SIM_WIRTSHAUS"): collects the 16 inns (0x501190 kind 5 / 0x16) with their two goods (0x4ffe90 records 0x179 / 0x17a), then in 0x180 steps per run over the figures of the ring (+0xac ..) it lets a figure sitting in an inn (0x54d1e0) buy a good: the 0x5089c0 prices * 1.5 are compared with the 0x509e60 wealth (the roll 0.65), the figure pays the inn (0x4c4830) and the inn's owner gets the amount, with the diary (0x4c3ba0 / 0x4c3d00 +0x65 / +0x6b / +0x69 slots of the good) written for both; at the end of the ring the counters reset (ROUGH) */

void he_Unk54d260(int ev)
{
    int tab[48];
    int i, n, cur, end, idx = 0, fig, p1, p2, wealth, price1, price2, lim, ent, cnt;
    int *e;
    int amt1, amt2;
    int cnt3;

    if (*(int *)(ev + 0x70) == -2) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    memset(tab, 0, sizeof(tab));
    e = tab;
    tab[0] = gm_BuildQuery(1, 5, 0x16);
    n = 0;
    while (tab[n * 3] != 0 && n < 16) {
        int b = tab[n * 3];
        int o = gm_Query(*(int *)(b + 0x5d), 2, 0, 0x2a, 6);

        if (o != 0) {
            tab[n * 3 + 1] = gm_Query(*(int *)(o + 0x14), 1, 0, 0x179);
            tab[n * 3 + 2] = gm_Query(*(int *)(o + 0x14), 1, 0, 0x17a);
        }
        n++;
        tab[n * 3] = gm_BuildNext();
    }
    if (*(int *)(ev + 0x70) != 0)
        return;
    cur = *(int *)(ev + 0xac);
    end = cur + 0x180;
    cnt = 0x180;
    if (cur < end) {
        for (fig = (unsigned)&ai_CharInts + cur * 0x21c; cnt > 0; fig += 0x21c, cnt--) {
            if (he_Unk54d1e0(fig - 4, tab, &idx) == 0)
                continue;
            price1 = (int)((double)gm_GoodsPrice(0x179, *(unsigned char *)(unsigned)&ai_OwnPlayer) * 1.5);
            price2 = (int)((double)gm_GoodsPrice(0x17a, *(unsigned char *)(unsigned)&ai_OwnPlayer) * 1.5);
            cnt3 = (unsigned short)util_Random(4) + 3;
            wealth = gm_GetGoodsValue(fig - 4);
            lim = price1 > price2 ? price1 : price2;
            p1 = tab[idx * 3 + 1];
            p2 = tab[idx * 3 + 2];
            if ((float)wealth < (float)lim * 0.65f || p1 == 0 || p2 == 0) {
                if (wealth > price1 && p1 != 0) {
                    cm_BeginGroup("SIM_WIRTSHAUS");
                    cm_SendExSellObject(-1, *(int *)(p1 + 6), 1, 0x179, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
                    cm_SendMoveGoodsUnchecked(-1, *(int *)fig, *(unsigned char *)(unsigned)&ai_OwnPlayer, (int)((float)price1 * 0.65f));
                    cm_SendMoveGoodsUnchecked(-1, U_I((unsigned)&ai_CharInts + *(unsigned short *)(tab[idx * 3] + 0x27) * 0x21c), *(unsigned char *)(unsigned)&ai_OwnPlayer, price1);
                    cm_BeginFieldBlock(tab[idx * 3], *(int *)(tab[idx * 3] + 1));
                    cm_AddFieldValues(4, 1, *(unsigned short *)tab[idx * 3] - *(unsigned short *)(unsigned)&cm_FieldObject + 0x65, &price1);
                    cm_AddFieldValues(2, 1, *(unsigned short *)tab[idx * 3] - *(unsigned short *)(unsigned)&cm_FieldObject + 0x6b, &cnt3);
                    cm_SendFieldDeltas();
                    cm_EndGroup("SIM_WIRTSHAUS");
                    cm_BeginFieldBlock(tab[idx * 3], *(int *)(tab[idx * 3] + 1));
                    cm_AddFieldValues(2, 1, *(unsigned short *)tab[idx * 3] - *(unsigned short *)(unsigned)&cm_FieldObject + 0x69, &cnt3);
                    cm_SendFieldDeltas();
                } else if (wealth > lim && p2 != 0) {
                    cm_BeginGroup("SIM_WIRTSHAUS");
                    cm_SendExSellObject(-1, *(int *)(p2 + 6), 1, 0x17a, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
                    cm_SendMoveGoodsUnchecked(-1, *(int *)fig, *(unsigned char *)(unsigned)&ai_OwnPlayer, (int)((float)price2 * 0.65f));
                    cm_SendMoveGoodsUnchecked(-1, U_I((unsigned)&ai_CharInts + *(unsigned short *)(tab[idx * 3] + 0x27) * 0x21c), *(unsigned char *)(unsigned)&ai_OwnPlayer, price2);
                    cm_BeginFieldBlock(tab[idx * 3], *(int *)(tab[idx * 3] + 1));
                    cm_AddFieldValues(4, 1, *(unsigned short *)tab[idx * 3] - *(unsigned short *)(unsigned)&cm_FieldObject + 0x65, &price2);
                    cm_AddFieldValues(2, 1, *(unsigned short *)tab[idx * 3] - *(unsigned short *)(unsigned)&cm_FieldObject + 0x6b, &cnt3);
                    cm_SendFieldDeltas();
                    cm_EndGroup("SIM_WIRTSHAUS");
                    cm_BeginFieldBlock(tab[idx * 3], *(int *)(tab[idx * 3] + 1));
                    cm_AddFieldValues(2, 1, *(unsigned short *)tab[idx * 3] - *(unsigned short *)(unsigned)&cm_FieldObject + 0x69, &cnt3);
                    cm_SendFieldDeltas();
                }
            } else if (util_Random(2) != 0) {
                cm_BeginGroup("SIM_WIRTSHAUS");
                cm_SendExSellObject(-1, *(int *)(p1 + 6), 1, 0x179, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
                cm_SendMoveGoodsUnchecked(-1, *(int *)fig, *(unsigned char *)(unsigned)&ai_OwnPlayer, (int)((float)price1 * 0.65f));
                cm_SendMoveGoodsUnchecked(-1, U_I((unsigned)&ai_CharInts + *(unsigned short *)(tab[idx * 3] + 0x27) * 0x21c), *(unsigned char *)(unsigned)&ai_OwnPlayer, price1);
                cm_BeginFieldBlock(tab[idx * 3], *(int *)(tab[idx * 3] + 1));
                cm_AddFieldValues(4, 1, *(unsigned short *)tab[idx * 3] - *(unsigned short *)(unsigned)&cm_FieldObject + 0x65, &price1);
                cm_AddFieldValues(2, 1, *(unsigned short *)tab[idx * 3] - *(unsigned short *)(unsigned)&cm_FieldObject + 0x6b, &cnt3);
                cm_SendFieldDeltas();
                cm_EndGroup("SIM_WIRTSHAUS");
                cm_BeginFieldBlock(tab[idx * 3], *(int *)(tab[idx * 3] + 1));
                cm_AddFieldValues(2, 1, *(unsigned short *)tab[idx * 3] - *(unsigned short *)(unsigned)&cm_FieldObject + 0x69, &cnt3);
                cm_SendFieldDeltas();
            }
        }
    }
    if (end >= 0x300) {
        *(int *)(ev + 0xac) = 0;
        *(int *)(ev + 0xb0) = 0;
        *(int *)(ev + 0x52) = *(int *)(ev + 0x52) + 1;
        *(short *)(ev + 0x56) = 0x16;
        *(int *)(ev + 0x58) = 0x1e;
        return;
    }
    gm_AddTime(ev + 0x52, 0, 0xf, 0);
}
