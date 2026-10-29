/* 0x5a13f0, the callback of the guild level 3 request (313 B, Ghidra-missed start: the callback of 0x5a17b0):
   the window "LOCATIONS\ZUNFTHAUS\LEVEL3" 0x4741f0, the text 0x167c, the text 0x167f (the office id, 8,
   the word of param_1) and the pump 0x5309a0 (0x5a13f0, 0x677c7); a click (0xf8a668 == 0) sends the request
   0x4e9ce0 (the player, {1, param_1 + 4}) and 0x6f22a0 = 3 (returns 1); 0xf8a668 == 1 only raises 0x6f22a0;
   0x4758f0 at the end, returns whether the request went through */

int amt_ExecuteMayorPowerMessage(char *c, int *m);

int ai_Unk5a13f0(int param_1)
{
    int req[2];
    int win;
    int id;
    int ok = 0;
    int r;

    win = d2_OpenForm(0, 0, "LOCATIONS\\ZUNFTHAUS\\LEVEL3");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    io_PrintText(0x167c);
    d2_SetForm(win, 2);
    if (*(unsigned char *)((unsigned)&ai_Unk770b89 + U_W((unsigned)&ai_PlayerId) * 0x21c) == 0)
        id = *(unsigned char *)(((unsigned)&ai_CharInts + 0x165) + U_W((unsigned)&ai_PlayerId) * 0x21c) + 0x253;
    else
        id = *(unsigned char *)(((unsigned)&ai_CharInts + 0x165) + U_W((unsigned)&ai_PlayerId) * 0x21c) + 0x27b;
    io_PrintText(0x167f, id, 8, *(unsigned short *)param_1);
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) == 0) {
            req[0] = 1;
            req[1] = U_I(param_1 + 4);
            amt_ExecuteMayorPowerMessage((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c, req);
            U_I((unsigned)&sv_PopRequest) = 3;
            ok = 1;
        } else if (U_I((unsigned)&d2_ClickedId) == 1) {
            U_I((unsigned)&sv_PopRequest) = 1;
        }
        if (sv_RunFrame(ai_Unk5a13f0, 0x677c7) == 0) {
            d2_CloseView(win);
            return ok;
        }
    }
}
