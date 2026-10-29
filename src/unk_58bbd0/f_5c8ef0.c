/* 0x5c8ef0, the divorce privilege handler (927 B, ROUGH): a message record (param_1, the AIMSG by word
   offsets: +0 the id, +2 the type, +4 the handle, +9 a flag, +0xd a byte, +0x24 the amount, +0x5c the
   partner's id record, +0x194 the choice count). Types 6 / 7 open the window "privillegien_scheidung"
   0x62d4ac with the two buttons 0x515960 (0x1d8f, 0x114 + byte +0xd + (flag ? 10 : 0), 8, main - partner
   price) / 0x475b80; the click 0 (0x520f90 (8) and 0x521040) closes 0x4c5f60 (-8) and runs the two carriers
   0x4c3ba0 / 0x4c3d90 (4, 1, 0x5c / 2, 1, 0x50) with 0x4c49e0, pays 0x4c47e0 (partner handle, handle,
   0x6f1bc5, main) and shows the texts 0x602568 / 0x1d90; the click 1 and the abort set 0x6f22a0. Other
   types: the count < 8 returns 0x22, the ratio 0x509e60 / price < 0.12 returns 0x22, else the same carrier
   chain with 0x4c49a0 and 0x4c47e0, return 0x11. The pump 0x5309a0 and the end 0x4758f0. ROUGH: the float
   arguments of 0x5de518 are lost; the price conversions are written as float calls. */

int ai_Unk5c8ef0(unsigned short *param_1)
{
    int ret;
    int done;
    int win;
    int t;
    int mainv;
    int other;
    int h;
    int v;
    unsigned short *partner;

    ret = -1;
    done = 0;
    if (*(char *)((char *)param_1 + 2) == 6 || *(char *)((char *)param_1 + 2) == 7) {
        mainv = (int)((float (*)(unsigned short))gm_GetWorth)(*param_1);
        partner = (unsigned short *)(int)gm_FindChar(*(int *)((char *)param_1 + 0x5c));
        other = (int)((float (*)(unsigned short))gm_GetWorth)(*partner);
        win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d4ac);
        d2_SetPointerSprite(0, 0);
        d2_CentreViewWindows(win);
        d2_SetForm(win, 1);
        t = io_PrintText(0x1d8f,
                         *(unsigned char *)((char *)param_1 + 0xd) +
                             (*(char *)((char *)param_1 + 9) != 0 ? 10 : 0) + 0x114,
                         8, mainv - other);
        d2_GetWinObj(win, 1, t);
        d2_GetWinObj(win, 1, t + 1);
        *(int *)(unsigned)&d2_ClickedId = -1;
        do {
            if (*(int *)(unsigned)&d2_ClickedId != -1) {
                if (*(int *)(unsigned)&d2_ClickedId == 0) {
                    if (sv_CheckMoney(param_1, 8) != 0 &&
                        sv_CheckGoodAmount(param_1, mainv, *(int *)(unsigned)&ai_OwnPlayer) != 0) {
                        cm_SendMoneyDelta(*(int *)((char *)param_1 + 4), 0xfffffff8);
                        cm_BeginFieldBlock(param_1, *(int *)((char *)param_1 + 4));
                        cm_AddFieldValue(4, 1, 0x5c, &ret);
                        h = ((int (*)())cm_SendFieldValues)();
                        while (cm_GetSlotState(h) == 0)
                            sv_PumpNet();
                        cm_BeginFieldBlock(partner, *(int *)((char *)partner + 4));
                        cm_AddFieldValue(4, 1, 0x5c, &ret);
                        cm_AddFieldValue(2, 1, 0x50, &ret);
                        h = ((int (*)())cm_SendFieldValues)();
                        while (cm_GetSlotState(h) == 0)
                            sv_PumpNet();
                        h = ((int (*)(int, int, int, int))cm_SendMoveGoods)(*(int *)((char *)partner + 4),
                                         *(int *)((char *)param_1 + 4), *(int *)(unsigned)&ai_OwnPlayer, mainv);
                        while (cm_GetSlotState(h) == 0)
                            sv_PumpNet();
                        io_PrintText((char *)(unsigned)"$C");
                        io_PrintText(0x1d90);
                        *(int *)(unsigned)&sv_PopRequest = 1;
                        done = 0x81;
                    }
                } else if (*(int *)(unsigned)&d2_ClickedId == 1) {
                    *(int *)(unsigned)&sv_PopRequest = 1;
                    done = 2;
                }
            }
            if (*(int *)((unsigned)&dm_L + 0x20) != 0) {
                *(int *)(unsigned)&sv_PopRequest = 1;
                done = 2;
            }
        } while (sv_RunFrame(ai_Unk5c8ef0, 0x657c7) != 0);
        d2_CloseView(win);
        return done;
    }
    if (*(int *)((char *)param_1 + 0x194) < 8)
        return 0x22;
    mainv = (int)((float (*)(unsigned short))gm_GetWorth)(*param_1);
    v = gm_GetGoodsValue(param_1);
    if ((float)v / (float)mainv < 0.12f)
        return 0x22;
    partner = (unsigned short *)(int)gm_FindChar(*(int *)((char *)param_1 + 0x5c));
    cm_SendMoneyDelta(*(int *)((char *)param_1 + 4), 0xfffffff8);
    cm_BeginFieldBlock(param_1, *(int *)((char *)param_1 + 4));
    cm_AddFieldValue(4, 1, 0x5c, &ret);
    cm_SendFieldDeltas();
    cm_BeginFieldBlock(partner, *(int *)((char *)partner + 4));
    cm_AddFieldValue(4, 1, 0x5c, &ret);
    cm_AddFieldValue(2, 1, 0x50, &ret);
    cm_SendFieldDeltas();
    h = ((int (*)(int, int, int, int))cm_SendMoveGoods)(*(int *)((char *)partner + 4), *(int *)((char *)param_1 + 4),
                     *(int *)(unsigned)&ai_OwnPlayer, mainv);
    while (cm_GetSlotState(h) == 0)
        sv_PumpNet();
    return 0x11;
}
