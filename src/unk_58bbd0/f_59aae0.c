/* 0x59aae0, the wine cellar of the player's house (544 B): "locations\wohnsitz\weinkeller" 0x4741f0, the
   player's own money 0x509e60 (0x770b80 + player * 0x21c) against the cellar's 0x509e30 (0x6f2310 + 0x14);
   the texts 0x1a4c, 0x1a4d (the player) and the counter 0x475b80 / 0x475cb0 (control, 0, the amount, the
   maximum = max (own + cellar, own): 0x5085b0 in the windows.h max macro, which is why the calls repeat); the
   pump 0x5309a0 (0x677c7); the counter 0x475980 / 0x5085f0 moves the difference 0x4c47e0 (the player, the
   cellar record + 2, the player byte, the amount) when 0xf8a668 == 0; the exit 0x4758f0 and 0x52d7a0 */

void ai_WineCellarWindow(void)
{
    int win;
    int own;
    int cellar;
    int ctl;
    int slider;
    int n;
    int a;
    int b;
    char k;

    k = *(char *)(unsigned)&ai_OwnPlayer;
    win = d2_OpenForm(0, 0, "locations\\wohnsitz\\weinkeller");
    own = gm_GetGoodsValue((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c);
    cellar = ((int (*)(int))gm_Unk509e30)(U_I(U_I((unsigned)&sv_PanelShown) + 0x14));
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    io_PrintText(0x1a4c);
    d2_SetForm(win, 2);
    ctl = d2_GetWinObj(win, 2, io_PrintText(0x1a4d, U_I((unsigned)&ai_OwnPlayer)));
    d2_SetObjValue(ctl, 0, ((int (*)(int, int, int))gm_ConvertMoney)(own, k, gm_ConvertMoney(cellar, k)), gm_ConvertMoney(cellar, k));
    d2_SetPointerSprite(0, 0);
    while (sv_RunFrame(ai_WineCellarWindow, 0x677c7) != 0) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0 || U_I((unsigned)&d2_ClickedId) == 1)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) == 0) {
            slider = ((int (*)(int))gm_PlayerFactor)(d2_GetObjValue(ctl, U_I((unsigned)&ai_OwnPlayer)));
            if (cellar < slider) {
                n = slider - cellar;
                a = U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x87);
                b = U_I(U_I((unsigned)&sv_PanelShown) + 2);
            } else {
                n = cellar - slider;
                a = U_I(U_I((unsigned)&sv_PanelShown) + 2);
                b = U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x87);
            }
            cm_SendMoveGoods(a, b, U_I((unsigned)&ai_OwnPlayer), n);
            U_I((unsigned)&sv_PopRequest) = 1;
        }
    }
    if (win != 0)
        d2_CloseView(win);
    sv_CameraFollow();
}
