/* 0x5a0ca0, the callback of the office application request "LOCATIONS\ZUNFTHAUS\LEVEL2" (619 B, Ghidra
   missed this start: it is only reached as the address-callback of 0x5a0f10): the amount 0x50a640 (the
   player's record 0x770b80 + slot * 0x21c) times 0x5eb7f0 capped at 0x5ed0f0 (min, the FPU compare) is the
   fee; 0x521040 (the player, the fee, the player byte) confirms; the window 0x4741f0, the texts 0x165f and
   0x1660 (the office id: 0x253 + the byte 0x770ce9 + slot * 0x21c, or 0x27b + it when the byte 0x770b89 is
   set; the town param_1 + 0x27, the fee, param_1), the two buttons 0x475b80; the first button with 0x521040
   sends the request 0x4e9a00 (the player, {2, param_1 + 1, the fee}) and 0x6f22a0 = 2; the second only
   0x6f22a0; the pump 0x5309a0 (0x5a0ca0, 0x677c7); 0x4758f0 and 1 at the end */

int amt_ExecuteShareMessage(short *c, int *m);

int ai_Unk5a0ca0(int param_1)
{
    int req[8];
    int win;
    int ctlA;
    int ctlB;
    int fee;
    int text;
    int id;
    int slot;
    int r;

    slot = U_W((unsigned)&ai_PlayerId);
    fee = gm_GetWorth(*(unsigned short *)((unsigned)&ai_Chars + slot * 0x21c));
    if ((float)fee * (0.00999999978f) > (160.0f)) {
        fee = (int)(160.0f);
    } else {
        fee = (int)((float)fee * (0.00999999978f));
    }
    if (sv_CheckGoodAmount((char *)(unsigned)&ai_Chars + slot * 0x21c, fee, *(unsigned char *)(unsigned)&ai_OwnPlayer) == 0)
        return 0;
    win = d2_OpenForm(0, 0, "LOCATIONS\\ZUNFTHAUS\\LEVEL2");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    io_PrintText(0x165f);
    d2_SetForm(win, 2);
    if (*(unsigned char *)((unsigned)&ai_Unk770b89 + slot * 0x21c) == 0)
        id = *(unsigned char *)(((unsigned)&ai_CharInts + 0x165) + slot * 0x21c) + 0x253;
    else
        id = *(unsigned char *)(((unsigned)&ai_CharInts + 0x165) + slot * 0x21c) + 0x27b;
    text = io_PrintText(0x1660, id, *(unsigned short *)(param_1 + 0x27), fee, 5, param_1);
    ctlA = d2_GetWinObj(win, 2, text);
    ctlB = d2_GetWinObj(win, 2, text + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == ctlA) {
                if (sv_CheckGoodAmount((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c, fee,
                                 *(unsigned char *)(unsigned)&ai_OwnPlayer) != 0) {
                    req[0] = 2;
                    req[1] = U_I(param_1 + 1);
                    req[2] = fee;
                    amt_ExecuteShareMessage((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c, req);
                    U_I((unsigned)&sv_PopRequest) = 2;
                }
            } else if (U_I((unsigned)&ai_Unk5fe710) == ctlB) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk5a0ca0, 0x677c7) == 0) {
            d2_CloseView(win);
            return 1;
        }
    }
}
