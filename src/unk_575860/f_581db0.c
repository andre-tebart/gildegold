/* 0x581db0, the cabinet spy window (1047 B, ROUGH: the amount is one float -> int conversion (0x5de518) whose
   argument Ghidra lost): arg 1 = the record; the target picked by 0x5c1130 (the 0x50-byte query, 0x144a270);
   0x56f3b0 (target) 0 -> the text 0x4030 through 0x5176d0 (the target's word) and 0x521150 (text, 0); else the
   skill points 0x50a640 (0x6f25b8) + 0x50a640 (the target's word) and the amount; the window
   "Locations_Domizil_kabinett" (0x62a418) with 0x476f80 / 0x475600, the sound 0x52ea30, the texts 0x602568 /
   0x402e / 0x402b (the amount, the target's word) and the entries 0x402f / +1 in 0x475b80 (window, 2, text); the
   pump loop 0x5309a0 (this handler, 0x677c7): the escape 0x14673a0 -> 0x6f22a0 = 1; the first entry: 0x521040
   (the character 0x770b80 + 0x6f25b8 * 0x21c, the amount, 0x6f1bc5), 0x520f90 (the character, 0x10) == 1,
   0x580fe0 (rec) -> 0x506430 (the character) +0x28 += the amount, 0x4c4830 (-1, the character, 0x6f1bc5, the
   amount), 0x4c5f60 (the character, -0x10), the message (type 0x99, 2 at +0x36, -1 at +0x64, 4 at +0x68, 0 at
   +0x6c, the target's word at +0x60, the time, the value at +0x58) and 0x4c2760 (-7, 0, "KOMMENTARE", -1) then
   0x6f22a0 = 1; the second entry -> 0x6f22a0 = 1; the pump 0 -> 0x4758f0 (window), 0x52d7a0 () */

int he_Unk56f3b0(); void txt_Format(char *out, int id0, ...); int sv_MessageBox();

void ai_Unk581db0(char *rec)
{
    char msg[0xf8];
    char q[0x50];
    char *p;
    int win;
    int b1;
    int b2;
    int total;
    int amount;
    int i;
    int r;
    int lv;

    total = gm_GetWorth(U_I((unsigned)&ai_PlayerId));
    memset(q, 0, 0x50);
    *(int *)(q + 0x00) = 1;
    *(int *)(q + 0x04) = 0x400;
    *(int *)(q + 0x10) = 6;
    p = (char *)ai_AmountMenu(q, (char *)U_I((unsigned)&full_data_144a270), 0, 0);
    if (!he_Unk56f3b0(p)) {
        txt_Format(msg, (char *)0x4030, *(unsigned short *)p);
        sv_MessageBox(msg, 0);
        return;
    }
    if (!p)
        return;
    total += gm_GetWorth(*(unsigned short *)p);
    amount = 0;   /* ROUGH: the 0x5de518 conversion's argument is lost */
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a418);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    io_PrintText((char *)(unsigned)"$C");
    io_PrintText(0x402e, 0x402b, amount, *(unsigned short *)p);
    d2_SetForm(win, 2);
    i = io_PrintText(0x402f);
    b1 = d2_GetWinObj(win, 2, i);
    b2 = d2_GetWinObj(win, 2, i + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == b1) {
                if (sv_CheckGoodAmount((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, amount, *(unsigned char *)(unsigned)&ai_OwnPlayer) != 0 &&
                    sv_CheckMoney((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, 0x10) == 1 &&
                    ai_Unk580fe0(rec) != 0) {
                    r = gm_PlayerRecord((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c);
                    if (r != 0)
                        *(int *)(r + 0x28) += amount;
                    cm_SendMoveGoodsUnchecked(-1, *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), *(unsigned char *)(unsigned)&ai_OwnPlayer, amount);
                    cm_SendMoneyDelta(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), -0x10);
                    memset(msg, 0, 0xf8);
                    *(unsigned char *)(msg + 0x04) = 0x99;
                    *(unsigned char *)(msg + 0x36) = 2;
                    *(int *)(msg + 0x64) = -1;
                    *(int *)(msg + 0x68) = 4;
                    *(int *)(msg + 0x6c) = 0;
                    *(int *)(msg + 0x08) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(rec + 0x27) * 0x21c);
                    *(int *)(msg + 0x0c) = U_I(rec + 1);
                    *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
                    *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                    *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                    *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
                    *(int *)(msg + 0x60) = *(int *)(p + 4);
                    *(int *)(msg + 0x5c) = U_I(rec + 1);
                    lv = -1;
                    if (*(int *)(p + 0x170) == 0) {
                        if (*(int *)(p + 0x16c) == 0)
                            AI_4A63F0_7((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, 0, (char *)0x7b4c4a0, 0, 0, 100.0f, &lv);
                        else
                            lv = *(int *)(*(int *)(p + 0x16c) + 1);
                    } else {
                        lv = *(int *)(*(int *)(p + 0x170) + 1);
                    }
                    *(int *)(msg + 0x58) = lv;
                    i = cm_SendLongMessage(msg);
                    if (i != -1) {
                        while (cm_GetSlotState(i) == 0)
                            sv_RunFrame(0, 0x20007);
                    }
                    snd_PlayVoice(-7, 0, (char *)(unsigned)&full_data_62a3ec, -1);
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            } else if (U_I((unsigned)&ai_Unk5fe710) == b2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk581db0, 0x677c7) == 0) {
            d2_CloseView(win);
            sv_CameraFollow();
            return;
        }
    }
}
