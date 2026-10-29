/* 0x5a1530, the callback of the guild level 2 request (308 B, Ghidra-missed start: the callback of 0x5a17b0):
   the same window "LOCATIONS\ZUNFTHAUS\LEVEL3", the text 0x166a, the text 0x166d (the office id, 8,
   param_1 + 0x1663), the pump 0x5309a0 (0x5a1530, 0x677c7); a click (0xf8a668 == 0) sends 0x4e9ce0 (the
   player, {4, param_1}) and 0x6f22a0 = 2; 0xf8a668 == 1 raises 0x6f22a0; 0x4758f0 at the end */

void ai_Unk5a1530(int param_1)
{
    int req[2];
    int win;
    int id;

    win = d2_OpenForm(0, 0, "LOCATIONS\\ZUNFTHAUS\\LEVEL3");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    io_PrintText(0x166a);
    d2_SetForm(win, 2);
    if (*(unsigned char *)((unsigned)&ai_Unk770b89 + U_W((unsigned)&ai_PlayerId) * 0x21c) == 0)
        id = *(unsigned char *)(((unsigned)&ai_CharInts + 0x165) + U_W((unsigned)&ai_PlayerId) * 0x21c) + 0x253;
    else
        id = *(unsigned char *)(((unsigned)&ai_CharInts + 0x165) + U_W((unsigned)&ai_PlayerId) * 0x21c) + 0x27b;
    io_PrintText(0x166d, id, 8, param_1 + 0x1663);
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) == 0) {
            req[0] = 4;
            req[1] = param_1;
            amt_ExecuteMayorPowerMessage((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c, req);
            U_I((unsigned)&sv_PopRequest) = 2;
        } else if (U_I((unsigned)&d2_ClickedId) == 1) {
            U_I((unsigned)&sv_PopRequest) = 1;
        }
        if (sv_RunFrame(ai_Unk5a1530, 0x677c7) == 0) {
            d2_CloseView(win);
            return;
        }
    }
}
