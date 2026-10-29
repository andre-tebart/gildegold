/* 0x595480, the trade search window of the kind 0x14 (1408 B, ROUGH SKELETON from the Ghidra pseudocode): the
   searched goods 0x1c4 / 0x1c5 / 0x1c6, the record 0x4ffe90 (0x2a); the building kind record 0x7e3f30 + kind *
   0x24d: ' ' gives 0x2af then (byte +0x247 >= 2) 0x2b0, 0x1f gives 0x293 / 0x294 / 0x295, other kinds give
   three (or with the byte +0x247 <= 1 two) entries; 0x520fe0; the sums of the stock byte +0xac per kind over the
   buildings 0x53e400 (2, 0, 0x14, 3) / 0x53e300 (matched by their +0xae); the window "HANDEL\HANDEL_SUCHEN"
   (0x4741f0 / 0x475600 / 0x475f70, 0x52e6d0 (0x70f, 0x741), 0x515960 (0x17fe), 0x58bbd0), the controls
   0x4731d0 (the texts 0x143c860, +0x4c = 1, 0x1c0 = 3, 0x1cc = 100, 0x1d0 = 0x10, 0x74 = 0x54, 0x473540,
   0x475e40); per frame the sums again (0x58d720 / 0x58dd00) and the texts 0x5176d0 (0x17ff for 1, 0x5f8318
   for the kind, 0x1800 for more); the click on a control builds the message (type 0x14, the character
   0x770b84, the building, the clock 0x770ae0 .., the kind, param_2 + 2, the six character slots, the count) and
   runs 0x4c2d20 / 0x4c4b70 / 0x4c3ae0 / 0x4c3b10, or 0x521150 (0x14401c4, 4) when no character; the pump
   0x5309a0 (this, 0x677c7); the exit 0x52ae40 / 0x525440 / 0x4758f0 / 0x52d7a0 */

void d2_SetObjEnabled(); void d2_SetCaption(); int d2_WinOpenPic(short x, short y, int pic, int win);

void ai_TradeSearchWindow(unsigned char *param_1, int param_2)
{
    unsigned int kinds[7];
    int sums[3];
    int win;
    int ctrls[4];
    int rec;
    int b;
    int i;
    int n;
    unsigned int kind;
    int char1;
    char text[0x100];
    unsigned char msg[0xf8];

    kinds[0] = 0x1c4;
    kinds[1] = 0x1c5;
    kinds[2] = 0x1c6;
    kinds[6] = gm_Query(*(int *)(param_1 + 0x5d), 2, 6, 0, 0x2a);
    kind = *param_1;
    rec = U_I((unsigned)&ai_BTypes) + kind * 0x24d;
    if (*(char *)rec == ' ') {
        kinds[0] = 0x2af;
        n = 1;
        if (*(unsigned char *)(rec + 0x247) >= 2) {
            kinds[1] = 0x2b0;
            n = 2;
        }
    } else if (*(char *)rec == 0x1f) {
        kinds[0] = 0x293;
        kinds[1] = 0x294;
        kinds[2] = 0x295;
        n = 3;
    } else if (*(char *)rec != 8 && *(unsigned char *)(rec + 0x247) > 1) {
        n = 3;
    } else {
        n = 2;
    }
    if (sv_CheckCharacterFlag(param_1) != 0)
        return;
    sums[0] = 0;
    sums[1] = 0;
    sums[2] = 0;
    for (b = he_FindEvent(2, 0, 0x14, 3, U_I(param_1 + 1)); b != 0; b = he_NextEvent()) {
        for (i = 0; i < 3; i++) {
            if (*(unsigned short *)(b + 0xae) == kinds[i]) {
                sums[i] += *(unsigned char *)(b + 0xac);
                break;
            }
        }
    }
    win = d2_OpenForm(0, 0, "HANDEL\\HANDEL_SUCHEN");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_Unk52e6d0(U_I((unsigned)&d2_LastWindow), 0x70f, 0x741);
    d2_SetForm(win, 3);
    io_PrintText(0x17fe);
    sv_OpenPlayerBar(param_1, 0);
    ai_Unk58bbd0();
    d2_CentreViewWindows(win);
    d2_SetForm(win, 2);
    kind = *(unsigned short *)((unsigned)&ai_Unkf8f128 + U_I((unsigned)&d2_LastWindow) * 0x3c4);
    for (i = 0; i < n; i++) {
        int id = d2_WinOpenPic((int)kind / 2 - 0x80, i * 0x50 + 0x19, kinds[i] + 0xdf, U_I((unsigned)&d2_LastWindow));
        U_I(U_I((unsigned)&d2_Objs) + id * 0x2ec + 0x4c) = 1;
        U_I(U_I((unsigned)&d2_Objs) + id * 0x2ec + 0x1c0) = 3;
        U_I(U_I((unsigned)&d2_Objs) + id * 0x2ec + 0x1cc) = 100;
        U_I(U_I((unsigned)&d2_Objs) + id * 0x2ec + 0x1d0) = 0x10;
        *(unsigned short *)(U_I((unsigned)&d2_Objs) + id * 0x2ec + 0x74) = 0x54;
        d2_SetCaption(id, U_I((unsigned)&ai_Unk143c860 + kinds[i] * 8));
        ctrls[i] = id;
        d2_SetObjEnabled(id, 1);
    }
    d2_SetPointerSprite(0, 0);
    ai_Unk58d720(param_1, kinds[6]);
    ai_Unk58dd00(win, 1, 0, param_1);
    while (sv_RunFrame(ai_TradeSearchWindow, 0x677c7) != 0) {
        ai_Unk58d720(param_1, kinds[6]);
        ai_Unk58dd00(win, 1, 0, param_1);
        sums[0] = 0;
        sums[1] = 0;
        sums[2] = 0;
        for (b = he_FindEvent(2, 0, 0x14, 3, U_I(param_1 + 1)); b != 0; b = he_NextEvent()) {
            for (i = 0; i < 3; i++) {
                if (*(unsigned short *)(b + 0xae) == kinds[i]) {
                    sums[i] += *(unsigned char *)(b + 0xac);
                    break;
                }
            }
        }
        d2_SetForm(win, 2);
        for (i = 0; i < n; i++) {
            d2_SetObjEnabled(ctrls[i], 1);
            if (sums[i] == 1)
                txt_Format(text, 0x17ff, 1);
            else if ((int)sums[i] < 2)
                txt_Format(text, "%s", kinds[i] * 2 + 0x9a8);
            else
                txt_Format(text, 0x1800, sums[i]);
            d2_SetCaption(ctrls[i], text);
        }
        if (U_I((unsigned)&d2_ClickedId) == -1) {
            if (U_I(((unsigned)&dm_L + 0x20)) != 0)
                U_I((unsigned)&sv_PopRequest) = 1;
        } else {
            for (i = 0; i < n; i++) {
                if (ctrls[i] == U_I((unsigned)&ai_Unk5fe710))
                    break;
            }
            if (i < n) {
                memset(msg, 0, 0xf8);
                *(unsigned char *)(msg + 4) = 0x14;
                *(unsigned char *)(msg + 0x36) = 1;
                *(int *)(msg + 8) = U_I((unsigned)&ai_CharInts + *(unsigned short *)(param_1 + 0x27) * 0x87);
                *(int *)(msg + 0xc) = U_I(param_1 + 1);
                *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
                *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
                *(unsigned short *)(msg + 0x5a) = kinds[i];
                *(int *)(msg + 0x5c) = U_I(param_2 + 2);
                memset(msg + 0x38, 0, 0x18);
                kind = 0;
                for (b = 0, i = 0; i < 6; i++) {
                    if (*(char *)((unsigned)&ai_CharInts + i * 0x87) != 0) {
                        *(int *)(msg + 0x38 + b * 4) = U_I((unsigned)&ai_CharInts + i * 0x87);
                        b++;
                    }
                }
                for (; b < 6; b++)
                    *(int *)(msg + 0x38 + b * 4) = -1;
                *(unsigned char *)(msg + 0x58) = b;
                if (b == 0) {
                    sv_MessageBox(U_I((unsigned)&full_data_14401c4), 4);
                } else {
                    snd_PlayFavourComment();
                    sv_ClearSelection();
                    *msg = 0;
                    /* ROUGH: the message goes through 0x4c4b70 / the confirmation 0x4c3ae0 / 0x4c3b10 */
                    kind = cm_SendLongMessage(msg);
                    b = cm_GetSlotState(kind);
                    while (b == 0) {
                        sv_RunFrame(0, 0x20007);
                        b = cm_GetSlotState(kind);
                    }
                    cm_GetSlotValue(kind);
                }
            }
        }
    }
    sv_ClearSelection();
    sv_ClosePlayerBar();
    d2_CloseView(win);
    sv_CameraFollow();
}
