/* 0x59b160, the money lender's credit list "locations\geldleihe\geldleihe_Kreditaufnehmen" (1232 B, ROUGH:
   the row records / controls of the list are condensed): the window 0x4741f0, the rectangle 0xf8f4b8 ..
   (0x18), the text 0x18c9 (the city's good price 0x7e3f94 + city * 0x2f4 word * 2 + 0x9a8); 0x476f80 and the
   pump 0x5309a0 (0x677c7); on rebuild the two 0x50a150 queries (player, 0 / player, param_1) count the offers
   (0x50a150); below 9 the rows 0x4731d0 (0x75a) and their lists 0x472aa0 (0x10), the texts 0x18d6 / 0x18ca ..
   0x18cd (with the "$C" page markers), the buttons 0x52e350 (screen, 0, out, 1 / 2, 0x14404fc / 0x14404f8) and
   the selection 0x5fe710: the first button opens 0x59ad60 (param_1), the second 0x59b030 (param_1, the offer) */

void ai_Unk59b160(int param_1)
{
    int list[9 * 3];
    int offers[0x10];
    int recs[9];
    int win;
    int ctl;
    int ctl2;
    int out[2];
    int n;
    int i;
    int k;
    int money;
    int rebuild;
    int v;
    int r;

    for (i = 0; i < 9; i++) {
        list[i * 3] = -1;
        list[i * 3 + 1] = -1;
        list[i * 3 + 2] = 0;
        recs[i] = 0;
    }
    win = d2_OpenForm(0, 0, "locations\\geldleihe\\geldleihe_kreditaufnehmen");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    n = U_I((unsigned)&d2_LastWindow) * 0x3c4;
    U_I(((unsigned)&ai_Unkf8f39c + 0x11c) + n) = 0x18;
    U_I(((unsigned)&ai_Unkf8f39c + 0x120) + n) = 0x18;
    U_I(((unsigned)&ai_Unkf8f39c + 0x124) + n) = 0x18;
    U_I(((unsigned)&ai_Unkf8f39c + 0x128) + n) = 0x18;
    d2_SetForm(win, 1);
    io_PrintText(0x18c9, (unsigned int)U_W((unsigned)&ai_Unk7e3f94 + U_I((unsigned)&ai_OwnPlayer) * 0x2f4) * 2 + 0x9a8);
    d2_SetPointerSprite(0, 0);
    ctl = -1;
    ctl2 = -1;
    rebuild = 1;
    n = 0;
    for (;;) {
        if (sv_RunFrame(ai_Unk59b160, 0x677c7) == 0)
            break;
        if (rebuild) {
            for (i = 0; i < 9; i++) {
                if (list[i * 3] != -1) {
                    d2_RemoveWindow(list[i * 3]);
                    d2_RemoveObj(list[i * 3 + 1]);
                    list[i * 3] = -1;
                    list[i * 3 + 1] = -1;
                    list[i * 3 + 2] = 0;
                }
            }
            money = gm_Unk50a150(U_I((unsigned)&ai_PlayerId), 0, offers);
            n = gm_Unk50a150(U_I((unsigned)&ai_PlayerId), param_1, offers);
            d2_SetForm(win, 4);
            io_PrintText("$C");
            if (n < 9) {
                if (n < 3) {
                    if (n == 0)
                        io_PrintText(0x18cd, *(unsigned short *)(param_1 + 0x27), param_1);
                    else
                        io_PrintText(0x18ca, *(unsigned short *)(param_1 + 0x27), param_1, n, money);
                    rebuild = 0;
                } else {
                    io_PrintText(0x18cc);
                    rebuild = 1;
                }
            } else {
                io_PrintText(0x18cb);
                rebuild = 1;
            }
            d2_SetForm(win, 3);
            io_PrintText("$C");
            if (rebuild) {
                sv_ButtonRow(U_I((unsigned)&d2_LastWindow), 0, out, 1, U_I((unsigned)&full_data_14404fc));
                ctl = -1;
                ctl2 = out[0];
            } else {
                sv_ButtonRow(U_I((unsigned)&d2_LastWindow), 0, out, 2, U_I((unsigned)&full_data_14404f8));
                ctl = out[0];
                ctl2 = out[1];
            }
            for (i = 0; i < n; i++) {
                d2_SetForm(win, 2);
                v = (*(short *)((unsigned)&d2_Pics + 0x269da) + 8) * i;
                list[i * 3] = d2_WinOpenPic(
                    (*(short *)((unsigned)&d2_LastWindowPtr + 8) - *(short *)((unsigned)&d2_Pics + 0x269d8)) >> 1, v, 0x75a,
                    U_I((unsigned)&d2_LastWindow));
                list[i * 3 + 1] = d2_CreateChildWindow(
                    (*(short *)((unsigned)&d2_LastWindowPtr + 8) - *(short *)((unsigned)&d2_Pics + 0x269d8)) >> 1, v,
                    *(short *)((unsigned)&d2_Pics + 0x269d8), *(short *)((unsigned)&d2_Pics + 0x269da), 0x10, U_I((unsigned)&d2_LastWindow));
                recs[i] = offers[i];
                list[i * 3 + 2] = offers[i];
                U_I((unsigned)&ai_Unkf8f39c + list[i * 3 + 1] * 0x1e2) = 0x56;
                *(unsigned short *)(((unsigned)&ai_Unkf8f128 + 0x8) + list[i * 3 + 1] * 0x3c4) = 0x10;
                io_PrintText(0x18d6, U_I(offers[i] + 0xb4), U_I(offers[i] + 0xc0), offers[i] + 0xc4);
            }
        }
        rebuild = 0;
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (ctl2 != -1) {
            if (n < 1 || U_I((unsigned)&d2_RadioSel + ctl2 * 0x23 * 4) == -1)
                k = 0;
            else
                k = 1;
            d2_SetObjEnabled(ctl2, k);
        }
        if (U_I((unsigned)&d2_ClickedId) != -1 && U_I((unsigned)&d2_ClickedId) == 0) {
            if (U_I((unsigned)&ai_Unk5fe710) == ctl && ctl != -1) {
                d2_SetViewEnabled(win, 0);
                ai_Unk59ad60(param_1);
                d2_SetViewEnabled(win, 1);
                rebuild = 1;
            } else if (U_I((unsigned)&ai_Unk5fe710) == ctl2 && ctl2 != -1) {
                d2_SetViewEnabled(win, 0);
                ai_Unk59b030(param_1, recs[U_I((unsigned)&d2_RadioSel + ctl2 * 0x23 * 4)]);
                rebuild = 1;
                d2_SetViewEnabled(win, 1);
            }
        }
    }
    d2_CloseView(win);
    if (ctl2 != -1)
        d2_RadioClear(ctl2);
    sv_CameraFollow();
}
