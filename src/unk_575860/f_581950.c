/* 0x581950, the spy window's pay step (1117 B, ROUGH): arg 1 = the record; the target picked by 0x5c1130 (the
   same 0x50-byte query, 0x144a1b0), 0 for none -> 0x52d7a0 () and return; the level 0x4e7c40 (the byte +0x166) / 3,
   the amounts level + 5 / + 7 / + 10 and the level + 2; the window "Locations_Domizil_kabinett_spy" (0x62a3f8)
   with 0x476f80 / 0x475600, the sound 0x52ea30, the slider 0x46a960 (0x57, 0x14, 200, 0 .. 100, 0x7b, 2) with
   0x46bca0 and the texts 0x4003 / +1 in 0x475b80 (window, 3, text); the pump loop 0x5309a0 (this handler,
   0x677c7): the step, the texts 0x602568 / 0x3ffd / 0x3ffb (the target's word, the level + 2) / 0x3ffe (the
   amount); the escape 0x14673a0 -> 0x6f22a0 = 1; the first entry: 0x520f90 (the character 0x770b80 + 0x6f25b8 *
   0x21c, the amount) == 1 and 0x580fe0 (rec) -> 0x4c5f60 (the character, -the amount), the message (type 0x99,
   2 at +0x36, -1 at +0x64, 1 at +0x68, the target's word at +0x60, the character at +8, the record + 1 at +0xc
   / +0x5c, the time, the step at +0x6c, the value at +0x58) and 0x4c2760 (-7, 0, "KOMMENTARE", 0) then
   0x6f22a0 = 1; the second entry -> 0x6f22a0 = 1; the pump 0 -> 0x4758f0 (window), 0x52d7a0 () */

void ai_Unk581950(char *rec)
{
    char msg[0xf8];
    char q[0x50];
    char *p;
    int amt[3];
    int win;
    int slider;
    int b1;
    int b2;
    int lvl;
    int n;
    int i;
    int k;
    int r;
    int id;
    int lv;

    memset(q, 0, 0x50);
    *(int *)(q + 0x00) = 1;
    *(int *)(q + 0x04) = 0x400;
    *(int *)(q + 0x10) = 6;
    p = (char *)ai_AmountMenu(q, (char *)U_I((unsigned)&full_data_144a1b0), 0, 0);
    if (!p) {
        sv_CameraFollow();
        return;
    }
    lvl = amt_GetKindLevel(*(unsigned char *)(p + 0x166)) / 3;
    n = lvl + 2;
    amt[0] = lvl + 5;
    amt[1] = lvl + 7;
    amt[2] = lvl + 10;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a3f8);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 2);
    k = d2_CreateWinText(0, 3, (char *)U_I((unsigned)&full_data_144a1bc), U_I((unsigned)&d2_LastWindow));
    *(int *)(U_I((unsigned)&d2_Objs) + 0x5c + k * 0x2ec) = 1;
    *(short *)(U_I((unsigned)&d2_Objs) + 0x14 + k * 0x2ec) = *(short *)((unsigned)&ai_Unkf8f128 + U_I((unsigned)&d2_LastWindow) * 0x3c4);
    d2_SetObjFont(k, 0x54);
    slider = d2_WinOpenSlider(0x57, 0x14, 200, 0, 100, 0x7b, 2, U_I((unsigned)&d2_LastWindow));
    d2_SetObjValue(slider, 0, 2, 0, 1);
    d2_SetList(slider, 3, U_I((unsigned)&full_data_144a1c0), U_I((unsigned)&full_data_144a1c4), U_I((unsigned)&full_data_144a1c8));
    d2_SetForm(win, 3);
    k = io_PrintText(0x4003);
    b1 = d2_GetWinObj(win, 3, k);
    b2 = d2_GetWinObj(win, 3, k + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        i = d2_GetObjValue(slider);
        d2_SetForm(win, 1);
        io_PrintText((char *)(unsigned)"$C");
        io_PrintText(0x3ffd, 0x3ffb, *(unsigned short *)p, n);
        io_PrintText(0x3ffe, amt[i]);
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == b1) {
                if (sv_CheckMoney((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, amt[i]) == 1 &&
                    ai_Unk580fe0(rec) != 0) {
                    cm_SendMoneyDelta(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), -amt[i]);
                    memset(msg, 0, 0xf8);
                    *(unsigned char *)(msg + 0x04) = 0x99;
                    *(unsigned char *)(msg + 0x36) = 2;
                    *(int *)(msg + 0x64) = -1;
                    *(int *)(msg + 0x68) = 1;
                    *(int *)(msg + 0x08) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(rec + 0x27) * 0x21c);
                    *(int *)(msg + 0x0c) = U_I(rec + 1);
                    *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
                    *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                    *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                    *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
                    *(int *)(msg + 0x60) = *(int *)(p + 4);
                    *(int *)(msg + 0x5c) = U_I(rec + 1);
                    *(int *)(msg + 0x6c) = i;
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
                    id = cm_SendLongMessage(msg);
                    if (id != -1) {
                        while (cm_GetSlotState(id) == 0)
                            sv_RunFrame(0, 0x20007);
                    }
                    snd_PlayVoice(-7, 0, (char *)(unsigned)&full_data_62a3ec, 0);
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            } else if (U_I((unsigned)&ai_Unk5fe710) == b2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk581950, 0x677c7) == 0) {
            d2_CloseView(win);
            sv_CameraFollow();
            return;
        }
    }
}
