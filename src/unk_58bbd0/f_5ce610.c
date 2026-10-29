/* 0x5ce610, the interrogation window "privillegien_verhoer" 0x62d5c4 (469 B): param_1 is the message
   record (AIMSG: +2 the type, +0x1cc the flags, +4 the handle), param_2 the request (+0x214 the target
   id). Type 6: the config record cfg (20 dwords, +4 = 0xc00, +8 = 0xa7fdfbb6, +0xc = 1, +0x10 = 6) opens
   the request window 0x5c1130 (cfg, U_I (0x14419e0), 0, 0); the window 0x4741f0 (0, 0, 0x62d5c4) with
   the text 0x515960 (0x1e09, the id) and its two buttons 0x475b80; click 0 -> 0x5ce4b0 (the message, the
   request), 0x91; the pump 0x5309a0, the exit 0x4758f0, the flag 0x80000 -> 0x20 (0x81 after 0x521150
   (U_I (0x1441540))). Other types: the target 0x505df0, no target / no kind byte +0x166 -> 0x60, else
   0x5ce4b0 and 0x10. */

int amt_SendOfficeMessage(unsigned char a, char *b, char *c, unsigned char d, unsigned char e); int gm_TitleNumber();

int ai_InterrogationWindow(int param_1, int param_2)  /* @original: the callers push (c, m); the draft body reads its own byte offsets of the parameters */
{
    int cfg[20];
    int win;
    int text;
    int click1;
    int click2;
    int rec;
    int i;
    int ret;

    if (*(char *)(param_1 + 2) != 6) {
        if ((*(unsigned int *)(param_1 + 0x1cc) & 0x80000) != 0)
            return 0x20;
        i = gm_FindChar(*(int *)(param_2 + 0x214));
        if (i == 0)
            return 0x60;
        if (*(char *)(i + 0x166) == 0)
            return 0x60;
        ai_Unk5ce4b0(param_1, i);
        return 0x10;
    }
    ret = 2;
    for (i = 0; i < 20; i++)
        cfg[i] = 0;
    if ((*(unsigned int *)(param_1 + 0x1cc) & 0x80000) != 0) {
        sv_MessageBox(U_I(((unsigned)&ai_MessageFilterTexts + 0x40)));
        return 0x81;
    }
    cfg[1] = 0xc00;
    cfg[2] = 0xa7fdfbb6;
    cfg[3] = 1;
    cfg[4] = 6;
    rec = ai_AmountMenu(cfg, U_I(((unsigned)&ai_MessageFilterTexts + 0x4e0)), 0, 0);
    if (rec == 0)
        return 2;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d5c4);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    text = io_PrintText(0x1e09, *(unsigned short *)rec);
    click1 = d2_GetWinObj(win, 1, text);
    click2 = d2_GetWinObj(win, 1, text + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == click1) {
                ai_Unk5ce4b0(param_1, rec);
                ret = 0x91;
                U_I((unsigned)&sv_PopRequest) = 1;
            } else if (U_I((unsigned)&ai_Unk5fe710) == click2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (sv_RunFrame(ai_InterrogationWindow, 0x657c7) == 0) {
            d2_CloseView(win);
            return ret;
        }
    }
}
