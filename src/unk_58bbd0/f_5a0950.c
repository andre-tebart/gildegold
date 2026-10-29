/* 0x5a0950, the guild cash payment window "locations\zunfthaus\gildenkasse" (1472 B, ROUGH): the room
   0x4ffe90 (param_1 + 0x5d, 2, 6, 0, 0x142) and its money 0x509e30 (room + 0x14); the player's money
   0x509e60; for the kinds 0x17 / 0x18 / 0x19 / 0x1a with the player's kind byte 0x770ce9 == 0x1e / 0x1f /
   0x20 / 0x21 the guild box opens: the text 0x1a1a (the money), 0x1a1b (the city), the amount 0x5bd060
   (0x640, the player, 0x20, out) and the slider 0x475b80 / 0x475cb0 (0x5085b0 (the amount)); a click on the
   slider pays 0x4c47e0 (room + 2, the character, the player byte, the amount) when 0x521040 (the player, the
   amount, the player byte) confirms; the second button only sets 0x6f22a0; the pump 0x5309a0 (0x5a0950,
   0x677c7), 0x52d7a0 / 0x4758f0 at the end */

void ai_Unk5a0950(unsigned char *param_1)
{
    int win;
    int room;
    int money;
    int own;
    int amount;
    int ctl1 = -1;
    int ctl2 = -1;
    int text;
    int id;
    int i;
    int k;
    int r;
    int v;
    char kind;

    kind = *(char *)(U_I((unsigned)&ai_BTypes) + *param_1 * 0x24d);
    room = gm_Query(U_I(param_1 + 0x5d), 2, 6, 0, 0x142);
    money = gm_Unk509e30(U_I(room + 0x14), *(unsigned char *)(unsigned)&ai_OwnPlayer);
    own = gm_GetGoodsValue((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c);
    id = 0;
    if (kind == 0x17 && *(unsigned char *)(((unsigned)&ai_CharInts + 0x165) + U_W((unsigned)&ai_PlayerId) * 0x21c) == 0x1e)
        k = 1;
    else if (kind == 0x18 && *(unsigned char *)(((unsigned)&ai_CharInts + 0x165) + U_W((unsigned)&ai_PlayerId) * 0x21c) == 0x1f)
        k = 1;
    else if (kind == 0x19 && *(unsigned char *)(((unsigned)&ai_CharInts + 0x165) + U_W((unsigned)&ai_PlayerId) * 0x21c) == 0x20)
        k = 1;
    else if (kind == 0x1a && *(unsigned char *)(((unsigned)&ai_CharInts + 0x165) + U_W((unsigned)&ai_PlayerId) * 0x21c) == 0x21)
        k = 1;
    else
        k = 0;
    win = d2_OpenForm(0, 0, "locations\\zunfthaus\\gildenkasse");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    io_PrintText(0x1a1a, money);
    if (k) {
        text = io_PrintText(0x1a1b, (unsigned int)U_W((unsigned)&ai_Unk7e3f94 + U_I((unsigned)&ai_OwnPlayer) * 0x2f4) * 2 + 0x9a8);
        ai_Unk5bd060(0x640, own, 0x20, &amount);
        v = gm_ConvertMoney(0x640, U_I((unsigned)&ai_OwnPlayer));
        v = ((int (*)(int, int, int))gm_ConvertMoney)(amount, U_I((unsigned)&ai_OwnPlayer), v);
        v = ((int (*)(int, int, int))gm_ConvertMoney)(0x640, U_I((unsigned)&ai_OwnPlayer), v);
        ctl2 = d2_GetWinObj(win, 1, text, v);
        d2_SetObjValue(ctl2);
        ctl1 = d2_GetWinObj(win, 1, text + 1);
        i = d2_GetWinObj(win, 1, text + 2);
    }
    U_I((unsigned)&d2_ClickedId) = -1;
    while (sv_RunFrame(ai_Unk5a0950, 0x677c7) != 0) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (k && U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == ctl1) {
                r = gm_PlayerFactor(d2_GetObjValue(d2_GetWinObj(win, 1, text)), U_I((unsigned)&ai_OwnPlayer));
                if (sv_CheckGoodAmount((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c, r, U_I((unsigned)&ai_OwnPlayer)) != 0) {
                    cm_SendMoveGoods(U_I(room + 2), U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x21c),
                                 U_I((unsigned)&ai_OwnPlayer), r);
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            } else if (U_I((unsigned)&ai_Unk5fe710) == ctl2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
    }
    sv_CameraFollow();
    d2_CloseView(win);
}
