/* 0x59b880, the money lender's credit release window "..geldleihe_KreditFreigabe" (592 B): the window, the text
   0x18e5, the limit 0x508660 (param_1 + 0x5d, the player byte), the texts 0x18e6 and the two controls 0x475b80
   (the same text and the next text id), the range control 0x475cb0 (5, 0x28, the character +0x65), the money
   0x509e60 (the character 0x770b80 + slot * 0x21c) and the counter 0x475cb0 (0, max (0x5085b0 (the money) +
   0x5085b0 (the limit), 0x5085b0 (the money)), 0x5085b0 (the money)); the pump 0x5309a0 (0x677c7); 0x6f22dc =
   the limit minus the selected amount; on select (0xf8a668 == 0) the difference 0x4c47e0 (param_1 + 1, the
   character, the player byte, the amount), 0x4c3ba0 (param_1, param_1 + 1), 0x4c3c00 (4, 1, 0x65, &value),
   0x4c49a0; 0x4758f0 at the end, 0x6f22dc = 0 */

void ai_Unk59b880(int param_1)
{
    int win;
    int limit;
    int ctl1;
    int ctl2;
    int v;
    int r;
    int n;
    int a;
    int b;
    char k;

    k = *(char *)(unsigned)&ai_OwnPlayer;
    win = d2_OpenForm(0, 0, "locations\\geldleihe\\geldleihe_KreditFreigabe");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x18e5);
    limit = gm_SumSlotAmounts(U_I(param_1 + 0x5d), U_I((unsigned)&ai_OwnPlayer));
    d2_SetForm(win, 2);
    r = io_PrintText(0x18e6, (unsigned int)U_W((unsigned)&ai_Unk7e3f94 + U_I((unsigned)&ai_OwnPlayer) * 0x2f4) * 2 + 0x9a8);
    ctl1 = d2_GetWinObj(win, 2, r);
    ctl2 = d2_GetWinObj(win, 2, r + 1);
    d2_SetObjValue(ctl1, 5, 0x28, U_I(param_1 + 0x65));
    v = gm_GetGoodsValue((char *)(unsigned)&ai_Chars + U_W(param_1 + 0x27) * 0x21c);
    a = gm_ConvertMoney(v, k);
    b = gm_ConvertMoney(limit, k);
    d2_SetObjValue(ctl2, 0, max(a + b, a), a);
    d2_SetPointerSprite(0, 0);
    while (sv_RunFrame(ai_Unk59b880, 0x677c7) != 0) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0 || U_I((unsigned)&d2_ClickedId) == 1)
            U_I((unsigned)&sv_PopRequest) = 1;
        v = d2_GetObjValue(ctl2, U_I((unsigned)&ai_OwnPlayer));
        n = ((int (*)(int))gm_PlayerFactor)(v);
        U_I((unsigned)&full_data_6f22dc) = limit - n;
        if (U_I((unsigned)&d2_ClickedId) == 0) {
            v = d2_GetObjValue(ctl2, U_I((unsigned)&ai_OwnPlayer));
            n = ((int (*)(int))gm_PlayerFactor)(v);
            if (limit < n) {
                n = n - limit;
                a = U_I(param_1 + 1);
                b = U_I((unsigned)&ai_CharInts + U_W(param_1 + 0x27) * 0x87);
            } else {
                n = limit - n;
                b = U_I(param_1 + 1);
                a = U_I((unsigned)&ai_CharInts + U_W(param_1 + 0x27) * 0x87);
            }
            cm_SendMoveGoods(a, b, *(unsigned char *)(unsigned)&ai_OwnPlayer, n);
            v = d2_GetObjValue(ctl1);
            cm_BeginFieldBlock(param_1, U_I(param_1 + 1));
            cm_AddFieldDelta(4, 1, 0x65, &v);
            cm_SendFieldDeltas();
            U_I((unsigned)&sv_PopRequest) = 1;
        }
    }
    d2_CloseView(win);
    U_I((unsigned)&full_data_6f22dc) = 0;
}
