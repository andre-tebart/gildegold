/* 0x5c6050, the gift window "misc\geschenck" 0x62d3fc (1136 B): param_1 = the own character record,
   param_2 = the caller's record; the amount chain 0x50a640 / 0x509e60 clamped to 0x640 / 0xc80 (the
   __ftol); the gift flag bit 0x20 of the record +0x1c8: set -> the text 0x1f4f only, clear -> the
   caption 0x1f4c, the slider 0x515960 (0x1f4d, param_2[0], 0x6f1bc5) through 0x475b80 (win, 1), the two
   buttons 0x1f4e and its +1 (0x475b80 (win, 1)) with 0x475cb0 (the slider, 0x5085b0 (the amount)); the
   frame loop 0x5309a0 (this, 0x677c7): the confirm 0x475980 (the slider) / 0x5085f0 / 0x521040 (the own
   record) -> 0x4c4540 / 0x4c47e0 (param_2 + 2, the character + 4, 0x6f1bc5, the amount) / 0x4c3ba0 /
   0x4c3c00 (4, 1, 0x1c8, the +0x1c8 word with 0x20) / 0x4c49a0 / 0x4c4580 (0x62d3f4) and 0x6f22a0 = 1;
   the second button sets 0x6f22a0; returns 1 after a confirm, 0 when the amount is below 1;
   0x4758f0 (win). ROUGH: the float chain of the amount is 0x50a640 / 0x509e60 with the 0.05 factor */

int ai_Unk5c6050(char *param_1, short *param_2)
{
    int win;
    int slider;
    int b1;
    int b2;
    int amount;
    int flag;
    int v;
    float f;

    b1 = -1;
    b2 = -1;
    v = 0;
    amount = (int)gm_GetWorth(U_W(param_1));
    if (amount < 0x640) {
        amount = 0x640;
        f = (float)amount;
    } else {
        f = gm_GetWorth(U_I(param_1));
    }
    if ((float)gm_GetGoodsValue(param_1) > f * 0.05f)
        f = gm_GetWorth(U_I(param_1));
    else
        f = (float)gm_GetGoodsValue(param_1);
    amount = (int)f;
    if (amount < 0xc80) {
        amount = 0xc80;
    } else {
        if ((float)gm_GetGoodsValue(param_1) > (float)amount * 0.05f)
            f = gm_GetWorth(U_I(param_1));
        else
            f = (float)gm_GetGoodsValue(param_1);
        amount = (int)f;
        if (amount < 1)
            return 0;
    }
    flag = (U_I(param_1 + 0x1c8) >> 5) & 1;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d3fc);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1f4c);
    if (flag == 0) {
        slider = d2_GetWinObj(win, 1, io_PrintText(0x1f4d, *param_2, U_I((unsigned)&ai_OwnPlayer)));
        v = io_PrintText(0x1f4e);
        b1 = d2_GetWinObj(win, 1, v);
        b2 = d2_GetWinObj(win, 1, v + 1);
        d2_SetObjValue(slider, gm_ConvertMoney(gm_ConvertMoney(amount, U_I((unsigned)&ai_OwnPlayer)), 0));
    } else {
        io_PrintText(0x1f4f);
    }
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) != -1 && flag == 0) {
            if (U_I((unsigned)&ai_Unk5fe710) == b1) {
                v = ((int (*)(int))gm_PlayerFactor)(d2_GetObjValue(slider, U_I((unsigned)&ai_OwnPlayer)));
                if (sv_CheckGoodAmount(param_1, v, U_I((unsigned)&ai_OwnPlayer)) != 0) {
                    f = (float)(U_I(param_1 + 0x1c8) | 0x20);
                    cm_BeginGroup((char *)(unsigned)&full_data_62d3f4);
                    cm_SendMoveGoods(U_I((char *)param_2 + 2), U_I(param_1 + 4), U_I((unsigned)&ai_OwnPlayer), v);
                    cm_BeginFieldBlock(param_1, U_I(param_1 + 4));
                    cm_AddFieldDelta(4, 1, 0x1c8, &f);
                    cm_SendFieldDeltas();
                    cm_EndGroup((char *)(unsigned)&full_data_62d3f4);
                }
                v = 1;
                U_I((unsigned)&sv_PopRequest) = 1;
            } else if (U_I((unsigned)&ai_Unk5fe710) == b2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk5c6050, 0x677c7) == 0)
            break;
    }
    d2_CloseView(win);
    return v;
}
