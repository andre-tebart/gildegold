/* 0x59dc00, the callback of the money lender info window "locations\geldleihe\geldleihe_info" (771 B, Ghidra
   missed this start: it is only reached as the address-taken callback of 0x59df10; ROUGH: the two lists of the
   selection are not typed): param_1 is the building record; 0x50a640 (*param_1) and 0x509e60 (param_1) give
   the two values of the text 0x18ef; the window 0x4741f0, the rectangle 0xc / 0x14 / 0xc / 0xc, the texts
   0x18ed, 0x18ee (*param_1), 0x18ef, the two buttons 0x475b80 (0x18f0 / 0x18f1) and the combo 0x475ff0
   (window, 2, 0x100, &a, &b, &c) with 0x52e860 (0x206, 0x167); on a change of the selection (a == 1 / 2) the
   list 0x5bd470 (param_1, 0, 0) / 0x5bd330 (param_1, 9, rows, win, 2) is rebuilt (the texts 0x18f2 / 0x18f3
   when it is empty) or the clicked control selects 1 / 2; the 9 row triples 0x472ce0 / 0x46e710 are freed on
   each rebuild and at the end; the pump 0x5309a0 (0x59dc00, 0x677c7), 0x4758f0 at the end */

int gm_GetWorth(unsigned short id); int ai_Unk5bd470(); int ai_Unk5bd330(); void d2_ClearPaintbox(void);

void ai_Unk59dc00(unsigned short *param_1)
{
    int rows[9 * 3];
    int a;
    int b;
    int c;
    int sel;
    int dirty;
    int win;
    int v;
    int money;
    int ctl1;
    int ctl2;
    int i;

    if (param_1 == 0)
        return;
    for (i = 0; i < 9; i++) {
        rows[i * 3] = -1;
        rows[i * 3 + 1] = -1;
        rows[i * 3 + 2] = 0;
    }
    v = gm_GetWorth(*param_1);
    money = gm_GetGoodsValue((char *)param_1);
    win = d2_OpenForm(0, 0, "locations\\geldleihe\\geldleihe_info");
    d2_SetForm(win, 0);
    i = U_I((unsigned)&d2_LastWindow) * 0x3c4;
    U_I(((unsigned)&ai_Unkf8f39c + 0x11c) + i) = 0xc;
    U_I(((unsigned)&ai_Unkf8f39c + 0x120) + i) = 0x14;
    U_I(((unsigned)&ai_Unkf8f39c + 0x124) + i) = 0xc;
    U_I(((unsigned)&ai_Unkf8f39c + 0x128) + i) = 0xc;
    d2_SetForm(win, 1);
    io_PrintText(0x18ed);
    d2_SetForm(win, 4);
    io_PrintText(0x18ee, *param_1);
    io_PrintText(0x18ef, money, v);
    d2_SetForm(win, 3);
    ctl1 = d2_GetWinObj(win, 3, io_PrintText(0x18f0));
    d2_SetForm(win, 5);
    ctl2 = d2_GetWinObj(win, 5, io_PrintText(0x18f1));
    d2_SetForm(win, 0);
    i = d2_GetViewWin(win, 2, 0x100, &a, &b, &c);
    sv_CreateFormSlider(0x206, 0x167, U_I((unsigned)&d2_LastWindow), i);
    d2_CentreViewWindows(win);
    U_I((unsigned)&d2_ClickedId) = -1;
    U_I((unsigned)&ai_Unk5fe710) = -1;
    sel = 0;
    dirty = 1;
    for (;;) {
        if (dirty) {
            dirty = 0;
            if (sel == 1) {
                for (i = 0; i < 9; i++) {
                    if (rows[i * 3] != -1) {
                        d2_RemoveWindow(rows[i * 3]);
                        d2_RemoveObj(rows[i * 3 + 1]);
                        rows[i * 3] = -1;
                        rows[i * 3 + 1] = -1;
                        rows[i * 3 + 2] = 0;
                    }
                }
            }
            d2_SetForm(win, 2);
            d2_ClearPaintbox();
            d2_RemoveWindowObjects(U_I((unsigned)&d2_LastWindow), 1);
            if (sel == 1) {
                if (ai_Unk5bd470(param_1, 0, 0) <= 0)
                    io_PrintText(0x18f2, *param_1);
            } else if (sel == 2) {
                if (ai_Unk5bd330(param_1, 9, rows, win, 2) == 0)
                    io_PrintText(0x18f3, *param_1);
            }
        }
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == ctl1) {
                if (sel != 1) {
                    sel = 1;
                    dirty = 1;
                }
            } else if (U_I((unsigned)&ai_Unk5fe710) == ctl2) {
                if (sel != 2) {
                    sel = 2;
                    dirty = 1;
                }
            }
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (sv_RunFrame(ai_Unk59dc00, 0x677c7) == 0)
            break;
    }
    for (i = 0; i < 9; i++) {
        if (rows[i * 3] != -1) {
            d2_RemoveWindow(rows[i * 3]);
            d2_RemoveObj(rows[i * 3 + 1]);
        }
    }
    d2_CloseView(win);
}
