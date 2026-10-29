/* 0x5cede0, the interrogation window variant 1 (320 B): param_1 the message record (+2 the type, +0x1c8
   the flags 0x100, +0x194 the count, +4 the handle), param_2 the request (+0x214 the target id). Other
   types: the target 0x505df0, no target / no kind -> 0x60, else 0x5ced50 (the message, the target, 1)
   and 0x10. Type 6: the menu 0x5bea50 (4, cfg, U_I (0x1441a18), 0x631090, 0) with cfg as in 0x5ce610;
   the window 0x4741f0 (0, 0, 0x62d5c4) with the text 0x515960 (0x1e17, the id, 4); click 0 ->
   0x520f90 (4), 0x5ced50 (.., 1) and its handle waited for with 0x4c3ae0 / 0x52fe00, 0x91; the pump
   0x5309a0 (0x657c7) and the exit 0x4758f0. */

int ai_InterrogationWindow1(int param_1, int param_2)  /* @original: the callers push (c, m); the draft body reads its own byte offsets of the parameters */
{
    int cfg[20];
    int win;
    int t;
    int click1;
    int click2;
    int rec;
    int h;
    int i;
    int ret;

    ret = 2;
    if (*(char *)(param_1 + 2) != 6) {
        if ((*(unsigned int *)(param_1 + 0x1c8) & 0x100) != 0)
            return 0x20;
        if (*(int *)(param_1 + 0x194) < 4)
            return 0x20;
        i = gm_FindChar(*(int *)(param_2 + 0x214));
        if (i == 0)
            return 0x60;
        if (*(char *)(i + 0x166) == 0)
            return 0x60;
        ai_Unk5ced50(param_1, i, 1);
        return 0x10;
    }
    for (i = 0; i < 20; i++)
        cfg[i] = 0;
    if ((*(unsigned int *)(param_1 + 0x1c8) & 0x100) != 0) {
        sv_MessageBox(U_I(((unsigned)&ai_MessageFilterTexts + 0x40)));
        return 0x81;
    }
    cfg[1] = 0xc00;
    cfg[2] = 0xa7fdfbb6;
    cfg[3] = 1;
    cfg[4] = 6;
    rec = ai_Unk5bea50(4, cfg, U_I(((unsigned)&ai_MessageFilterTexts + 0x518)), (char *)(unsigned)&ch_EmptyName, 0);
    if (rec == 0)
        return 2;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d5c4);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    t = io_PrintText(0x1e17, *(unsigned short *)rec, 4);
    click1 = d2_GetWinObj(win, 1, t);
    click2 = d2_GetWinObj(win, 1, t + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == click1) {
                i = sv_CheckMoney(param_1, 4);
                if (i != 0) {
                    h = ai_Unk5ced50(param_1, rec, 1);
                    if (h != -1)
                        while (cm_GetSlotState(h) == 0)
                            sv_PumpNet();
                    U_I((unsigned)&sv_PopRequest) = 1;
                    ret = 0x91;
                }
            } else if (U_I((unsigned)&ai_Unk5fe710) == click2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (sv_RunFrame(ai_InterrogationWindow1, 0x657c7) == 0) {
            d2_CloseView(win);
            return ret;
        }
    }
}
