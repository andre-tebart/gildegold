/* 0x59d410, the money lender's exchange money messenger window "..geldleihe_WechselGeldbote" (464 B): the
   window, the text 0x1909 (param_3, param_2 * 2 + 0x9a8, the city 0x7e3f40 + param_4 * 0x2f4, param_4); the
   pump 0x5309a0 (0x677c7); on select (0xf8a668 == 0) 0x508620 (param_3, param_2) against 0x508620 (1, param_2),
   the fee 0x4c4880 (-1, param_1 + 1, param_2, the amount, the player byte, 0) and the AIMSG 0x27 (param_1 + 1,
   the character 0x770b84 + slot * 0x21c, the city param_4 at +0x5c, the fee at +0x64); returns 1 after the
   send, else 0 */

int ai_Unk59d410(int param_1, int param_2, int param_3, unsigned char param_4)
{
    char msg[0xf8];
    int win;
    int r;
    int fee;
    int r2;

    win = d2_OpenForm(0, 0, "locations\\geldleihe\\geldleihe_WechselGeldbote");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1909, param_3, param_2 * 2 + 0x9a8, (char *)(unsigned)&ai_PlayerSlots + param_4 * 0x2f4, param_4);
    for (;;) {
        if (sv_RunFrame(ai_Unk59d410, 0x677c7) == 0) {
            d2_CloseView(win);
            return 0;
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0 || U_I((unsigned)&d2_ClickedId) == 1)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) == 0) {
            fee = gm_Unk508620(param_3, param_2);
            r2 = gm_Unk508620(1, param_2);
            if ((float)fee * 0.03f <= (float)r2)
                gm_Unk508620(1, param_2);
            cm_SendExSellObject(-1, U_I(param_1 + 1), param_2, fee, U_I((unsigned)&ai_OwnPlayer), 0);
            memset(msg, 0, 0xf8);
            *(unsigned char *)(msg + 4) = 0x27;
            *(int *)(msg + 0xc) = -1;
            *(unsigned char *)(msg + 0x36) = 1;
            *(int *)(msg + 0x5c) = U_I(param_1 + 1);
            *(int *)(msg + 8) = U_I((unsigned)&ai_CharInts + U_W(param_1 + 0x27) * 0x87);
            *(unsigned char *)(msg + 0x60) = param_4;
            *(int *)(msg + 0x64) = fee;
            cm_SendLongMessage(msg);
            U_I((unsigned)&sv_PopRequest) = 1;
            return 1;
        } else if (U_I((unsigned)&d2_ClickedId) == 1) {
            U_I((unsigned)&sv_PopRequest) = 1;
        }
    }
}
