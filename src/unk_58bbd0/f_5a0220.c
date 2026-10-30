/* 0x5a0220, the office application window of a room "locations\rathaus\amtsbewerbung2" (870 B, ROUGH: the
   applicant positions are condensed): 0x4e6ec0 (param_2, the applicants); the up to 4 characters
   0x770b80 + i * 0x21c whose byte +0x168 == param_2 become the positions 0x5be1d0 / 0x5be3b0; the window
   0x4741f0, the text 0x1a07 (param_2 + 0x253), the three buttons 0x52e350 (0x14409e0 / e4 / e8), the text
   0x1a0b / 0x1a0c; the buttons open the application 0x4e6180 (param_1, param_2) and wait 0x4c3ae0 /
   0x52fe00, the office 0x4e6f10 / 0x5bea50 (0x52a040) or 0x5c1a80 (param_2, 0); the event 0x576040 (0x2f,
   param_2) on success; returns 1 when the application went through */

int amt_GetFirstOffice(unsigned int k, struct AMTREC *out); int amt_ApplyForOffice(); int amt_KindColumn(); void sv_PumpNet();
int ai_Unk5bea50(); int ai_Unk5c1a80(); int amt_GetOfficeCandidatesFull(); int amt_GetOfficeCandidates();
int ai_AmountRowStatus(); int ai_AmountRow(); int ai_Unk5c2160(); int ai_Unk5bd060();
int amt_CanUseMayorPower2(); int sv_CheckMoney(); int ai_Unk5b2180(); void d2_SetObjEnabled();
int sv_ReturnZero(void);

int ai_Unk5a0220(int param_1, unsigned int param_2)
{
    int chars[4];
    int buf[24];
    int items[0x38];
    int win = -1;
    int n;
    int ok;
    int i;
    int v;
    int outs[3];
    int ctl1 = -1;
    int ctl2 = -1;
    int ctl3 = -1;
    int handle;
    short s;
    int w;
    int r;
    unsigned int k;
    int *p;

    ok = 0;
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (amt_GetFirstOffice(param_2, buf) == 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else {
            n = 0;
            for (p = (int *)(unsigned)&ai_Chars; p < (int *)(unsigned)((unsigned)ai_CharsV + 0x65400) && n < 4; p += 0x10e) {
                if (*p != -1 && *(unsigned char *)(p + 0xb4) == param_2) {
                    chars[n] = (int)p;
                    n++;
                }
            }
            if (i != n) {
                if (win == -1) {
                    win = d2_OpenForm(0, 0, "locations\\rathaus\\amtsbewerbung2");
                    d2_CentreViewWindows(win);
                    d2_SetPointerSprite(0, 0);
                    d2_SetForm(win, 1);
                    io_PrintText(0x1a07, param_2 + 0x253);
                    d2_SetForm(win, 3);
                    sv_ButtonRow(U_I((unsigned)&d2_LastWindow), 0, outs, 3, U_I((unsigned)&full_data_14409e0), U_I((unsigned)&full_data_14409e4),
                                 U_I((unsigned)&full_data_14409e8));
                    ctl1 = outs[0];
                    ctl2 = outs[1];
                    ctl3 = outs[2];
                }
                d2_SetForm(win, 2);
                d2_RemoveWindowObjects(U_I((unsigned)&d2_LastWindow), 1);
                io_PrintText(0x1a0b);
                if (n < 1) {
                    io_PrintText(0x1a0c);
                } else {
                    k = 0;
                    w = *(short *)((unsigned)&d2_LastWindowPtr + 8) + *(short *)((unsigned)&d2_Pics + 0x1ebd8) * -2;
                    for (i = 0; i < n; i++) {
                        s = *(short *)((unsigned)&d2_Pics + 0x1ebd8);
                        items[i * 0xe + 0xc] = 0;
                        items[i * 0xe + 4] = ((k & 1) + 1) * (w / 3) - 10 + (w % 3) / 2 + (s + 10) * (k & 1);
                        items[i * 0xe] = chars[k];
                        items[i * 0xe + 5] = (k / 2) * 0x82 + 0x28;
                        ai_AmountRowStatus(items + i * 0xe);
                        ai_AmountRow(items[i * 0xe + 4], items[i * 0xe + 5], U_I((unsigned)&d2_LastWindow), 0x56,
                                     items + i * 0xe, 1);
                        k++;
                    }
                }
                d2_SetObjEnabled(ctl1, (n < 4 && *(char *)(param_1 + 0x168) == 0) ? 1 : 0);
                i = n;
            }
        }
        if (U_I(((unsigned)&dm_L + 0x20)) == 0) {
            if (U_I((unsigned)&d2_ClickedId) != -1) {
                if (U_I((unsigned)&ai_Unk5fe710) == i) {
                    handle = amt_ApplyForOffice(param_1, param_2);
                    if (handle != -1)
                        while (cm_GetSlotState(handle) == 0)
                            sv_PumpNet();
                    if (cm_GetSlotState(handle) == 1)
                        ok = 1;
                    ai_Unk576040(0x2f, 0, 0, param_2);
                } else if (U_I((unsigned)&ai_Unk5fe710) == ctl2) {
                    r = amt_KindColumn(param_2);
                    ai_Unk5bea50(r, 0, 0, 0, (void *)(unsigned)&sv_ReturnZero);
                } else if (U_I((unsigned)&ai_Unk5fe710) == ctl3) {
                    ai_Unk5c1a80(param_2, 0);
                }
            }
        } else {
            U_I((unsigned)&sv_PopRequest) = 1;
        }
        if (sv_RunFrame(ai_Unk5a0220, 0x677c7) == 0) {
            if (win != -1)
                d2_CloseView(win);
            if (ok != 0) {
                U_I((unsigned)&sv_PopRequest) = 1;
                ai_Unk576040(0x2f, 0, 1, param_2);
            }
            return ok;
        }
    }
}
