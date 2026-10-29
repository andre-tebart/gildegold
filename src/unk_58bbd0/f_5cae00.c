/* 0x5cae00, the counter-espionage handler "privillegien\gegenspionage" 0x62d56c (979 B): param_1 is the
   message record (the AIMSG: +2 the type 6 / 7, +4 the handle, +0x194 the amount). Types other than 6 / 7:
   the amount <= 3 returns 0x20; else the AIMSG record (the type byte +4 = 0x18, the handle +8 / +0xc, the
   clock +0x28, the type byte +0x36 = 2, the character +0x70) and the carrier walk 0x53e400 (1, 0, 0x18) /
   0x53e300: a carrier whose +0xc4 is the handle gets the clock at +0x52 and +0xc8, its +0x70 through
   0x4c4c60 (the carrier) and the record sent by 0x4c4b70; the result 2 | 0x10. Types 6 / 7: the window
   0x4741f0 / 0x476f80 / 0x475600 / 0x475f70 with the text 0x515960 (0x1db7, 4) and the two buttons
   0x475b80; the first click 0x520f90 (4) builds the text 0x5176d0 (0x1db8, then 0x1db9 + the character
   index per matching carrier, 0x1dba when none) and the record again (0x4c4c60 (0, the carrier)), shows
   0x521150 and sets 0x6f22a0; the second click sets 0x6f22a0; the pump 0x5309a0 (this, 0x657c7) and the
   end 0x4758f0, return 2. ROUGH: the record and the text buffer by offset. */

int ai_CounterEspionage(int param_1)  /* @original: the callers push (c, m) (0x4f67c0); the draft body reads byte offsets of the one int param */
{
    int rec[0x3e];
    char text[0x200];
    char *p;
    int handle;
    int win;
    int btn1;
    int btn2;
    int t;
    int carrier;
    int found;
    int i;
    int ret;

    ret = 2;
    if (*(char *)(param_1 + 2) != 6 && *(char *)(param_1 + 2) != 7) {
        if (*(int *)(param_1 + 0x194) <= 3)
            return 0x20;
        for (i = 0; i < 0x3e; i++)
            rec[i] = 0;
        handle = *(int *)(param_1 + 4);
        *(char *)((char *)rec + 4) = 0x18;
        *(int *)((char *)rec + 8) = handle;
        *(int *)((char *)rec + 0xc) = handle;
        *(HETIME *)((char *)rec + 0x28) = ai_Time;
        *(char *)((char *)rec + 0x36) = 2;
        cm_SendMoneyDelta(handle, 0xfffffffc);
        carrier = he_FindEvent(1, 0, 0x18);
        while (carrier != 0) {
            if (*(int *)(carrier + 0xc4) == *(int *)(param_1 + 4)) {
                ret |= 0x10;
                *(HETIME *)(carrier + 0x52) = ai_Time;
                *(HETIME *)(carrier + 0xc8) = ai_Time;
                cm_SendExSetHE(*(int *)(carrier + 0x70), carrier);
                *(int *)((char *)rec + 0x70) =
                    U_I((unsigned)&ai_CharInts + U_W(carrier + 8) * 0x21c);
                cm_SendLongMessage(rec);
            }
            carrier = he_NextEvent();
        }
        return ret;
    }
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d56c);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    t = io_PrintText(0x1db7, 4);
    btn1 = d2_GetWinObj(win, 1, t);
    btn2 = d2_GetWinObj(win, 1, t + 1);
    *(int *)(unsigned)&d2_ClickedId = -1;
    do {
        if (*(int *)(unsigned)&d2_ClickedId != -1) {
            if (*(int *)(unsigned)&ai_Unk5fe710 == btn1) {
                if (sv_CheckMoney(param_1, 4) != 0) {
                    found = 0;
                    txt_Format(text + 1, (char *)0x1db8);
                    p = text + 1 + strlen(text + 1);
                    for (i = 0; i < 0x3e; i++)
                        rec[i] = 0;
                    handle = *(int *)(param_1 + 4);
                    *(char *)((char *)rec + 4) = 0x18;
                    *(int *)((char *)rec + 8) = handle;
                    *(int *)((char *)rec + 0xc) = handle;
                    *(HETIME *)((char *)rec + 0x28) = ai_Time;
                    *(char *)((char *)rec + 0x36) = 2;
                    cm_SendMoneyDelta(handle, 0xfffffffc);
                    carrier = he_FindEvent(1, 0, 0x18);
                    if (carrier == 0) {
                        txt_Format(text + 1, (char *)0x1dba);
                    } else {
                        do {
                            if (*(int *)(carrier + 0xc4) == *(int *)(param_1 + 4)) {
                                found = 1;
                                *(HETIME *)(carrier + 0x52) = ai_Time;
                                cm_SendExSetHE(0, carrier);
                                *(int *)((char *)rec + 0x70) =
                                    U_I((unsigned)&ai_CharInts + U_W(carrier + 8) * 0x21c);
                                cm_SendLongMessage(rec);
                                txt_Format(p, (char *)0x1db9, U_W(carrier + 8));
                                p = text + 1 + strlen(text + 1);
                            }
                            carrier = he_NextEvent();
                        } while (carrier != 0);
                        if (!found)
                            txt_Format(text + 1, (char *)0x1dba);
                    }
                    sv_MessageBox(text + 1, 0);
                    *(int *)(unsigned)&sv_PopRequest = 1;
                }
            } else if (*(int *)(unsigned)&ai_Unk5fe710 == btn2) {
                *(int *)(unsigned)&sv_PopRequest = 1;
            }
        }
        if (*(int *)((unsigned)&dm_L + 0x20) != 0)
            *(int *)(unsigned)&sv_PopRequest = 1;
        if (sv_RunFrame(ai_CounterEspionage, 0x657c7) == 0) {
            d2_CloseView(win);
            return 2;
        }
    } while (1);
}
