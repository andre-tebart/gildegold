/* 0x5ce7f0, the dismissal window "privillegien_arbeiter_ausweisen" 0x62d5dc (352 B): the types 6 / 7
   only; the kind byte +0x166 through 0x4e6ec0 (the list out[0]); the window 0x4741f0 with the text
   0x515960 (0x1e24, the kind 0x253 + the own byte +9 ? 0x28 : 0 and the kind byte) and the button
   0x515960 (0x1e25) / 0x475b80; click 0 -> 0x4e6680 (out[0], 0, 0, the hour 0x770ae4 > 0xd ? -1 : 3,
   0xff) and 0x90; click 1 closes; the pump 0x5309a0 (0x657c7) and the exit 0x4758f0. The return is
   0x22 when nothing was clicked, 0x20 for another type, 0x40 when the kind has no list. */

int ai_WorkerDismissalWindow(int param_1)  /* @original: the callers push (c, m); the draft body reads its own byte offsets of the parameters */
{
    int out[6];
    int win;
    int text;
    int click1;
    int click2;
    int i;
    int ret;

    ret = 0x22;
    if (*(char *)(param_1 + 2) != 6 && *(char *)(param_1 + 2) != 7)
        return 0x20;
    i = amt_GetFirstOffice(*(unsigned char *)(param_1 + 0x166), out);
    if (i == 0)
        return 0x40;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d5dc);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1e24, (*(char *)(param_1 + 9) != 0 ? 0x28 : 0) + 0x253 +
                          *(unsigned char *)(param_1 + 0x166));
    text = io_PrintText(0x1e25);
    click1 = d2_GetWinObj(win, 1, text);
    click2 = d2_GetWinObj(win, 1, text + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == click1) {
                amt_SendOfficeMessage(out[0], 0, 0, (U_W((unsigned)&ai_TimeHour) > 0xd ? -1 : 3), 0xff);
                ret = 0x90;
                U_I((unsigned)&sv_PopRequest) = 1;
            } else if (U_I((unsigned)&ai_Unk5fe710) == click2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_WorkerDismissalWindow, 0x657c7) == 0) {
            d2_CloseView(win);
            return ret;
        }
    }
}
