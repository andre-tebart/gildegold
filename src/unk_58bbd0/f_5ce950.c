/* 0x5ce950, the dismissal handler / its window (1024 B): param_1 is the message record (AIMSG: +2 the
   type, +0x1cc the flags, +0x194 the count, +4 the handle), param_2 the request (+0x214 the target id).
   Other types: the target 0x505df0, its kind byte +0x166 through 0x4e6ec0, 0x4e6680 (out[0], 0, 0, the
   hour 0x770ae4 > 0xd ? -1 : 3, 0xff), the close 0x4c5f60 (-8) and the flag 0x4c4a60 (+0x1cc, 4,
   0x40000); the kinds 6 / 7 also send the text 0x5176d0 (0x1e10, the id, the kind 0x253 / the byte
   +0x27b) through 0x53dda0 (the key "_NACHRICHTEN_NEU_07" 0x62d6a8). Type 6: the config window 0x5c1130
   (cfg, U_I (0x14419f8), 0, 0) with the text 0x515960 (0x1e0f, the id) and two buttons; click 0 ->
   0x520f90 (8) then the same message block and 0x91; the pump 0x5309a0 (0x657c7). */

int ai_DismissalHandler(unsigned short *param_1, int param_2)  /* @original: the callers push (c, m); the draft body reads its own byte offsets of the parameters */
{
    int out[6];
    int cfg[20];
    char text[512];
    int win;
    int t;
    int click1;
    int click2;
    int rec;
    int i;
    int h;
    int ret;

    if (*(char *)((char *)param_1 + 2) != 6) {
        if ((*(unsigned int *)((char *)param_1 + 0x1cc) & 0x40000) != 0)
            return 0x20;
        if (*(int *)((char *)param_1 + 0x194) < 8)
            return 0x20;
        i = gm_FindChar(*(int *)(param_2 + 0x214));
        if (i == 0)
            return 0x60;
        if (*(char *)(i + 0x166) == 0)
            return 0x60;
        h = amt_GetFirstOffice(*(unsigned char *)(i + 0x166), out);
        if (h == 0)
            return 0x40;
        amt_SendOfficeMessage(out[0], 0, 0, (U_W((unsigned)&ai_TimeHour) > 0xd ? -1 : 3), 0xff);
        cm_SendMoneyDelta(*(int *)((char *)param_1 + 4), -8);
        cm_SendSetField(*(int *)((char *)param_1 + 4), 0x1cc, 4, 0x40000, 0);
        if (*(char *)(i + 2) == 6 || *(char *)(i + 2) == 7) {
            h = *(char *)(i + 9) == 0 ? 0x253
                                      : *(unsigned char *)(i + 0x166) + 0x27b;
            txt_Format(text, (char *)0x1e10, *param_1, h);
            he_ShowNews(*(int *)(i + 4), -1, text, 0, 0x5fb, (char *)(unsigned)&full_data_62d6a8);
        }
        return 0x10;
    }
    ret = 2;
    for (i = 0; i < 20; i++)
        cfg[i] = 0;
    if ((*(unsigned int *)((char *)param_1 + 0x1cc) & 0x40000) != 0) {
        sv_MessageBox(U_I(((unsigned)&ai_MessageFilterTexts + 0x40)));
        return 0x81;
    }
    cfg[1] = 0xc00;
    cfg[2] = 0xa7fdfbb6;
    cfg[3] = 1;
    cfg[4] = 6;
    rec = ai_AmountMenu(cfg, U_I(((unsigned)&ai_MessageFilterTexts + 0x4f8)), 0, 0);
    if (rec == 0)
        return 2;
    h = amt_GetFirstOffice(*(unsigned char *)(rec + 0x166), out);
    if (h == 0)
        return 0x40;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d5c4);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    t = io_PrintText(0x1e0f, *(unsigned short *)rec);
    click1 = d2_GetWinObj(win, 1, t);
    click2 = d2_GetWinObj(win, 1, t + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == click1) {
                i = sv_CheckMoney(param_1, 8);
                if (i != 0) {
                    amt_SendOfficeMessage(out[0], 0, 0, (U_W((unsigned)&ai_TimeHour) > 0xd ? -1 : 3), 0xff);
                    cm_SendMoneyDelta(*(int *)((char *)param_1 + 4), -8);
                    i = cm_SendSetField(*(int *)((char *)param_1 + 4), 0x1cc, 4, 0x40000, 0);
                    if (i != -1)
                        while (cm_GetSlotState(i) == 0)
                            sv_PumpNet();
                    U_I((unsigned)&sv_PopRequest) = 1;
                    ret = 0x91;
                    if (*(char *)(rec + 2) == 6 || *(char *)(rec + 2) == 7) {
                        i = *(char *)(rec + 9) == 0 ? 0x253
                                                   : *(unsigned char *)(rec + 0x166) + 0x27b;
                        txt_Format(text, (char *)0x1e10, *param_1, i);
                        he_ShowNews(*(int *)(rec + 4), -1, text, 0, 0x5fb, 0);
                    }
                }
            } else if (U_I((unsigned)&ai_Unk5fe710) == click2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (sv_RunFrame(ai_DismissalHandler, 0x657c7) == 0) {
            d2_CloseView(win);
            return ret;
        }
    }
}
