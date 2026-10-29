/* 0x59ad60, the new credit window "locations\geldleihe\geldleihe_neuerkredit" (720 B): the three offers
   0x50a250 (param_1, the player 0x770b80 + player * 0x21c, 3) with a positive first value are copied into the
   table of 5-int records (value, kind, amount, control, index); the window 0x4741f0, the texts 0x18d0, 0x18d1
   (short param_1 + 0x25, param_1), per offer 0x18d2 (value, amount, kind, 0x5d) into the control 0x475b80 and
   0x18d3; the pump 0x5309a0 (0x677c7) waits for a click 0x5fe710 on an offer control and sends the AIMSG 0x23
   (the player and param_1 + 1, the clock, +0x36 = 0x12, the payment 0x4c4830 (the player, param_1 + 1, the
   player byte, the value), the value / kind / amount at +0x60 / +0x64 / +0x6c), waits for 0x4c3ae0, hides the
   window (0x4754d0), shows 0x521150 (0x1440510, 4), re-shows it and clears the offer's slot
   param_1 + 0x71 + index * 0xc; 0xf8a668 == 1 only raises 0x6f22a0 */

void ai_Unk59ad60(int param_1)
{
    int offers[15];
    int found[9];
    char msg[0xf8];
    int win;
    int i;
    int n;
    int r;
    int handle;
    int *e;

    for (i = 0; i < 15; i++)
        offers[i] = 0;
    gm_Unk50a250(param_1, (char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c, 3, found);
    n = 0;
    for (i = 0; i < 3; i++) {
        if (found[i * 3] > 0) {
            offers[n * 5] = found[i * 3];
            offers[n * 5 + 1] = found[i * 3 + 1];
            offers[n * 5 + 2] = found[i * 3 + 2];
            offers[n * 5 + 4] = i;
            n++;
        }
    }
    win = d2_OpenForm(0, 0, "locations\\geldleihe\\geldleihe_neuerkredit");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x18d0);
    d2_SetForm(win, 2);
    io_PrintText(0x18d1, *(unsigned short *)(param_1 + 0x25), param_1);
    for (i = 0; i < 3; i++) {
        e = offers + i * 5;
        if (e[0] > 0)
            e[3] = d2_GetWinObj(win, 2, io_PrintText(0x18d2, e[0], e[2], e[1], 0x5d));
    }
    io_PrintText(0x18d3);
    d2_SetPointerSprite(0, 0);
    for (;;) {
        if (sv_RunFrame(ai_Unk59ad60, 0x677c7) == 0) {
            d2_CloseView(win);
            return;
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) == 0) {
            for (i = 0; i < 3; i++) {
                e = offers + i * 5;
                if (e[0] != 0 && U_I((unsigned)&ai_Unk5fe710) == e[3]) {
                    memset(msg, 0, 0xf8);
                    *(unsigned char *)(msg + 4) = 0x23;
                    *(int *)(msg + 8) = U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x87);
                    *(int *)(msg + 0xc) = U_I(param_1 + 1);
                    *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
                    *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                    *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                    *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
                    *(unsigned char *)(msg + 0x36) = 0x12;
                    *(int *)(msg + 0x58) = U_I(param_1 + 1);
                    *(int *)(msg + 0x5c) = U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x87);
                    *(int *)(msg + 0x60) = e[0];
                    *(int *)(msg + 0x64) = e[1];
                    *(int *)(msg + 0x6c) = e[2];
                    cm_SendMoveGoodsUnchecked(U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x87), U_I(param_1 + 1),
                                 *(unsigned char *)(unsigned)&ai_OwnPlayer, e[0]);
                    handle = cm_SendLongMessage(msg);
                    while (cm_GetSlotState(handle) == 0)
                        sv_RunFrame(0, 0x677c7);
                    d2_EnableViewObjects(win, 0);
                    sv_MessageBox((char *)U_I((unsigned)&full_data_1440510), 4);
                    d2_EnableViewObjects(win, 1);
                    U_I(param_1 + 0x71 + e[4] * 0xc) = -1;
                    goto done;
                }
            }
        } else if (U_I((unsigned)&d2_ClickedId) == 1) {
done:
            U_I((unsigned)&sv_PopRequest) = 1;
        }
    }
}
