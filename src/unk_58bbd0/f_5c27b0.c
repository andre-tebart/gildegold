/* 0x5c27b0, the master letter window's callback (608 B; param_1 = the window record, param_2 = the message):
   the message 0x10 builds the one control (the texts 0x1949 / 0x194b / 0x194d onto 0x475b80 (win, 1, ..)),
   the message 0x20 the two controls (0x1938 and its +1 char); any other message closes the window. A click on
   the first control walks the carriers 0x53e400 (2, 0, 7, 3, the record +1) / 0x53e300; a carrier whose +0xac
   is 0 sets 0x6f22a0, the empty chain sends the AIMSG type 7 (the kind record 0x7e3f30 + kind * 0x24d byte
   +0x223 into the text 0x1939 (+0x130), the character 0x770b84 + the word 0x6f25b8 * 0x87, the record +1, the
   clock 0x770ae0 .., +0x58 = 0 and +0x60 = -1 or 0x6f2310 + 2) and shows the text 0x521150 (text, 0); a click
   on the second sets 0x6f22a0. The pump 0x5309a0 (this, 0x677c7) and 0x4758f0 (win) at the exit */

void ai_Unk5c27b0(char *param_1, int param_2)
{
    int ctrl2;
    int ctrl1;
    char msg[0xf8];
    char text[0x200];
    char *t;
    char *carrier;
    int win;

    ctrl2 = -1;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d2bc);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    if (param_2 == 0x10) {
        io_PrintText(0x1949);
        io_PrintText(0x194b);
        ctrl1 = d2_GetWinObj(win, 1, io_PrintText(0x194d));
    } else if (param_2 == 0x20) {
        t = (char *)io_PrintText(0x1938);
        ctrl1 = d2_GetWinObj(win, 1, t);
        ctrl2 = d2_GetWinObj(win, 1, t + 1);
    } else {
        d2_CloseView(win);
        return;
    }
    do {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == ctrl1) {
                carrier = (char *)he_FindEvent(2, 0, 7, 3, U_I(param_1 + 1));
                while (carrier != 0 && *(unsigned char *)(carrier + 0xac) != 0)
                    carrier = (char *)he_NextEvent();
                if (carrier != 0) {
                    U_I((unsigned)&sv_PopRequest) = 1;
                } else {
                    memset(msg, 0, 0xf8);
                    *(unsigned char *)(msg + 4) = 7;
                    *(unsigned char *)(msg + 0x36) = 1;
                    *(int *)(msg + 8) = U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x87);
                    *(int *)(msg + 0xc) = U_I(param_1 + 1);
                    *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
                    *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                    *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                    *(unsigned short *)(msg + 0x34) = U_W(((unsigned)&ai_Time + 0xc));
                    *(unsigned char *)(msg + 0x58) = 0;
                    if (gm_IsHomeType(param_1) != 0)
                        *(int *)(msg + 0x60) = -1;
                    else
                        *(int *)(msg + 0x60) = U_I(U_I((unsigned)&sv_PanelShown) + 2);
                    cm_SendLongMessage(msg);
                    txt_Format(text, 0x1939,
                        *(unsigned char *)(U_I((unsigned)&ai_BTypes) + *param_1 * 0x24d + 0x223) + 0x130);
                    d2_ClearInput();
                    sv_MessageBox(text, 0);
                }
            } else if (U_I((unsigned)&ai_Unk5fe710) == ctrl2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
    } while (sv_RunFrame(ai_Unk5c27b0, 0x677c7) != 0);
    d2_CloseView(win);
}
