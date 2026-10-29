/* 0x5cca40, the espionage amount window 0x62d634 (455 B): param_1 is the message record (the AIMSG: +2
   the type 6, +4 the handle, +0x194 the amount), param_2 the request (dword +4 the value 1 .. 9). Types
   other than 6: the value outside 1 .. 9 or above the record +0x194 returns 0x60; the setter 0x4f3a30
   (the handle, the value) and the close 0x4c5f60 (-the value), return 0. Type 6: the window 0x4741f0 /
   0x476f80 / 0x475600 / 0x475f70 with the text 0x515960 (0x1dea) and the three controls 0x475b80
   (t+1, t+2 and the slider t); the slider 0x475cb0 (1, 9, 1); the first click 0x475980 and 0x520f90
   (the value) close 0x4c5f60 (-the value), set 0x4f3a30 and show 0x5176d0 (0x1deb, the result) /
   0x521150, return 1; the second click sets 0x6f22a0; the pump 0x5309a0 (this, 0x657c7) and the end
   0x4758f0. */

int ai_EspionageAmountWindow(int param_1, int *param_2)  /* @original: the callers push (c, m); the draft body reads its own byte offsets of the parameters */
{
    char text[0x100];
    int win;
    int btn1;
    int btn2;
    int slider;
    int t;
    int value;
    int v;
    int ret;

    ret = 0;
    if (*(char *)(param_1 + 2) != 6) {
        value = param_2[1];
        if (value < 1 || value > 9 || value > *(int *)(param_1 + 0x194))
            return 0x60;
        unk_Unk4f3a30(*(int *)(param_1 + 4), value);
        cm_SendMoneyDelta(*(int *)(param_1 + 4), -value);
        if (*(char *)(param_1 + 2) != 6)
            return 0;
    }
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d634);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    t = io_PrintText(0x1dea);
    btn1 = d2_GetWinObj(win, 1, t + 1);
    btn2 = d2_GetWinObj(win, 1, t + 2);
    slider = d2_GetWinObj(win, 1, t);
    d2_SetObjValue(slider, 1, 9, 1);
    *(int *)(unsigned)&d2_ClickedId = -1;
    do {
        if (*(int *)(unsigned)&d2_ClickedId != -1) {
            if (*(int *)(unsigned)&ai_Unk5fe710 == btn1) {
                value = d2_GetObjValue(slider);
                if (sv_CheckMoney(param_1, value) != 0) {
                    cm_SendMoneyDelta(*(int *)(param_1 + 4), -value);
                    v = unk_Unk4f3a30(*(int *)(param_1 + 4), value);
                    txt_Format(text, (char *)0x1deb, v);
                    sv_MessageBox(text, 0);
                    ret = 1;
                    *(int *)(unsigned)&sv_PopRequest = 1;
                }
            } else if (*(int *)(unsigned)&ai_Unk5fe710 == btn2) {
                *(int *)(unsigned)&sv_PopRequest = 1;
            }
        }
        if (*(int *)((unsigned)&dm_L + 0x20) != 0)
            *(int *)(unsigned)&sv_PopRequest = 1;
        if (sv_RunFrame(ai_EspionageAmountWindow, 0x657c7) == 0) {
            d2_CloseView(win);
            return ret;
        }
    } while (1);
}
