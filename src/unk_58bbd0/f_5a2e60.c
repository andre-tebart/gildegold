/* 0x5a2e60, the sermon window "Locations\Kirche\Predigt" (2138 B, ROUGH SKELETON): only the player's own
   building (+0x27 == the player slot); the room 0x4ffe90 (param_1 + 0x5d, 1, 0, 0x1b5) or a request
   0x4c50b0 / 0x4c3b10 (0x4c3680 / 0x4c3fd0 / 0x4c4190 until 0x4c3b50 answers); the church state local_1cc:
   0 (no priest) / 1 / 2 (a priest, 0x505df0 of the record +0x1e) / 3 (closed), recomputed every 0x65 ticks
   of 0x6333cc; the window 0x4741f0 / 0x476f80 / 0x475600 / 0x475f70 / 0x52ea30 (the screen 0x6ed938) with the
   texts 0x19a5 / 0x19a6 (no priest), 0x19a0 / 0x19a1, 0x198e / 0x198f (the priest) and the buttons 0x475b80;
   0x5be1d0 / 0x5be3b0 draw the priest; 0x475e40 disables a button; the clicks: the collection 0x520f90 (the
   player, 1) with 0x4c5f60 (the id, -1) / 0x4c3ba0 / 0x4c3c00 (1, 1, 0x22) / 0x4c49a0, the request 0x5c1130
   (the record {0, 0x400, 0xa7fdfbb6, 1, 6}, the text 0x19a2) with the random pair 0x42ec00 (8) and
   0x4c3c00 (4, 1, 0x1e) / (1, 1, 0x1c) / (1, 1, 0x1d), and the 0xff reset of the same three fields; the pump
   0x5309a0 (0, 0x677c7) waits for the answers; the exit 0x4758f0 / 0x52d7a0. ROUGH SKELETON: the three case
   bodies keep the calls in order, the widget records are not typed */

void ai_Unk4c50b0(int a, int b, short c); int cm_GetSlotValue(int h); int cm_SendGroupState(); int cm_FlushSendQueue();
int cm_ReceiveMessages(); int cm_ExecCommands(); int cm_SendLongMessage(char *msg); int cm_GetSlotState();
int ai_AmountMenu(); void d2_SetObjEnabled(); char *_itoa(int, char *, int);

void ai_Unk5a2e60(int param_1)
{
    int win;
    int rec;
    int obj;
    int state = 0;
    int i;
    int h;
    int req[20];
    int a[4];
    char *priest = 0;
    char text[0x20];
    int btn1 = -1;
    int btn2 = -1;
    int btn3 = -1;
    int cancel = -1;
    int changed = 1;
    int half;
    short w;
    char r1;
    char r2;
    char v;

    if (param_1 == 0)
        return;
    if (U_W(param_1 + 0x27) == 0xffff || U_W(param_1 + 0x27) != U_W((unsigned)&ai_PlayerId))
        return;
    rec = gm_Query(U_I(param_1 + 0x5d), 1, 0, 0x1b5);
    if (rec == 0) {
        h = ((int (*)(int, int, int))ai_Unk4c50b0)(U_I(param_1 + 1), -1, 0xffff);
        cm_GetSlotValue(h);
        while (cm_SendGroupState() == 0) {
            cm_FlushSendQueue();
            cm_ReceiveMessages();
            cm_ExecCommands();
        }
        rec = cm_GetSlotValue(h);
    }
    obj = he_FindEvent(2, 0, 0x2b, 3, U_I(param_1 + 1));
    if (obj == 0) {
        v = 0x2b;
        h = cm_SendLongMessage((char *)(unsigned)&ai_Time);
        if (h != -1) {
            while (cm_GetSlotState(h) == 0)
                sv_RunFrame(0, 0x677c7);
        }
        obj = cm_GetSlotValue(h);
    }
    if (*(char *)(rec + 0x22) == 0) {
        state = 0;
    } else if (*(char *)(rec + 0x1c) != -1 && *(char *)(rec + 0x1d) != -1 &&
               (priest = (char *)gm_FindChar(U_I(rec + 0x1e))) != 0) {
        state = 2;
    } else {
        state = 1;
    }
    win = d2_OpenForm(0, 0, "Locations\\Kirche\\Predigt");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (changed && state != 3) {
            d2_SetForm(win, 1);
            d2_RemoveWindowObjects(U_I((unsigned)&d2_LastWindow), 1);
            io_PrintText((char *)(unsigned)"$C");
            if (state == 0) {
                io_PrintText(0x19a5, 1, obj + 0x52);
                d2_SetForm(win, 2);
                io_PrintText((char *)(unsigned)"$C");
                i = io_PrintText(0x19a6);
                btn1 = d2_GetWinObj(win, 2, i);
                cancel = d2_GetWinObj(win, 2, i + 1);
            } else if (state == 1) {
                io_PrintText(0x19a0, obj + 0x52);
                d2_SetForm(win, 2);
                io_PrintText((char *)(unsigned)"$C");
                i = io_PrintText(0x19a1);
                btn2 = d2_GetWinObj(win, 2, i);
                cancel = d2_GetWinObj(win, 2, i + 1);
                d2_SetObjEnabled(btn2, U_I(obj + 0x70) < 1);
            } else if (state == 2) {
                half = (int)*(short *)(U_I((unsigned)&d2_LastWindowPtr) + 8) / 2;
                w = *(short *)(U_I((unsigned)&d2_Pics) + 0x1ebd8);
                io_PrintText(0x198e, obj + 0x52,
                             *(char *)(rec + 0x1c) + 0x1990,
                             *(char *)(rec + 0x1d) + 0x1998, U_W(priest));
                a[0] = (int)priest;
                ai_AmountRowStatus(a);
                ai_AmountRow(half - w / 2, 0xc3, U_I((unsigned)&d2_LastWindow), 0x54, a, 1);
                d2_SetForm(win, 2);
                d2_RemoveWindowObjects(U_I((unsigned)&d2_LastWindow), 1);
                io_PrintText((char *)(unsigned)"$C");
                i = io_PrintText(0x198f);
                btn3 = d2_GetWinObj(win, 2, i);
                cancel = d2_GetWinObj(win, 2, i + 1);
                d2_SetObjEnabled(btn3, U_I(obj + 0x70) < 1);
            }
            changed = 0;
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0 || state == 3) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == btn1) {
                if (sv_CheckMoney((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c, 1) != 0) {
                    v = 1;
                    cm_SendMoneyDelta(U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x21c), -1);
                    cm_BeginFieldBlock((char *)rec, U_I(rec + 2));
                    cm_AddFieldDelta(1, 1, 0x22, &v);
                    i = ((int (*)())cm_SendFieldDeltas)();
                    changed = 1;
                    state = 1;
                    if (i != -1) {
                        while (cm_GetSlotState(i) == 0)
                            sv_RunFrame(0, 0x677c7);
                    }
                }
            } else if (U_I((unsigned)&ai_Unk5fe710) == btn2) {
                for (i = 0; i < 20; i++)
                    req[i] = 0;
                req[1] = 0x400;
                req[4] = 6;
                req[2] = 0xa7fdfbb6;
                req[3] = 1;
                txt_Format(text, 0x19a2);
                h = ai_AmountMenu(req, text, 0, 0);
                rec = gm_Query(U_I(param_1 + 0x5d), 1, 0, 0x1b5);
                if (h != 0 && U_I(h + 4) != -1 && rec != 0) {
                    r1 = util_Random(8);
                    r2 = util_Random(8);
                    cm_BeginFieldBlock((char *)rec, U_I(rec + 2));
                    cm_AddFieldDelta(4, 1, 0x1e, (char *)(h + 4));
                    cm_SendFieldDeltas();
                    cm_BeginFieldBlock((char *)rec, U_I(rec + 2));
                    cm_AddFieldDelta(1, 1, 0x1c, &r1);
                    cm_SendFieldDeltas();
                    cm_BeginFieldBlock((char *)rec, U_I(rec + 2));
                    cm_AddFieldDelta(1, 1, 0x1d, &r2);
                    i = ((int (*)())cm_SendFieldDeltas)();
                    state = 2;
                    changed = 1;
                    if (i != -1) {
                        while (cm_GetSlotState(i) == 0)
                            sv_RunFrame(0, 0x677c7);
                    }
                }
            } else if (U_I((unsigned)&ai_Unk5fe710) == btn3) {
                r1 = 0xff;
                r2 = 0xff;
                v = 0xff;
                cm_BeginFieldBlock((char *)rec, U_I(rec + 2));
                cm_AddFieldDelta(4, 1, 0x1e, &v);
                cm_SendFieldDeltas();
                cm_BeginFieldBlock((char *)rec, U_I(rec + 2));
                cm_AddFieldDelta(1, 1, 0x1c, &r1);
                cm_SendFieldDeltas();
                cm_BeginFieldBlock((char *)rec, U_I(rec + 2));
                cm_AddFieldDelta(1, 1, 0x1d, &r2);
                i = ((int (*)())cm_SendFieldDeltas)();
                changed = 1;
                state = 1;
                if (i != -1) {
                    while (cm_GetSlotState(i) == 0)
                        sv_RunFrame(0, 0x677c7);
                }
            } else if (U_I((unsigned)&ai_Unk5fe710) == cancel) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk5a2e60, 0x677c7) == 0) {
            d2_CloseView(win);
            sv_CameraFollow();
            return;
        }
    }
}
