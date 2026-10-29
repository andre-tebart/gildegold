/* 0x5c6910, the advertising / marriage proposal confirm (395 B): the rating 0x5c6620 (the own id
   0x770b84 + 0x6f25b8 * 0x87, param_1 + 4); the window "privillegien_werbung2" 0x62d40c, 0x475600,
   0x515960 (0x1ce5, *param_1, the rating); the frame loop 0x5309a0 (this, 0x657c7): a click 0 ->
   the AIMSG 0x41 (memset 0xf8, +4 = 0x41, +8 / +0xc / +0x58 the own id, +0x28 .. +0x34 the clock,
   +0x36 = 2, +0x5c the partner id) through 0x4c4b70, the confirmation 0x4c3ae0 in the 0x52fe00 loop,
   the result 1 and 0x6f22a0; click 1 sets 0x6f22a0; the exit 0x4758f0, 0xf8a668 = -1 and 0x4115b0
   (0x11) */

void sv_PumpNet(); int ai_AmountRowStatus(); int ai_AmountRow();
void d2_EnableViewObjects(); void gm_AddTime(struct GCLOCK *c, int hours, int mins, int secs); 
int sv_CheckGoodAmount(); int sv_MessageBox(); int cm_GetSlotState();

int ai_Unk5c6910(short *param_1)
{
    int win;
    int v;
    int i;
    char msg[0xf8];

    v = ai_Unk5c6620(U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x87), U_I((char *)param_1 + 4));
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d40c);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1ce5, *param_1, v);
    U_I((unsigned)&d2_ClickedId) = -1;
    v = 0;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&d2_ClickedId) == 0) {
                memset(msg, 0, 0xf8);
                *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
                *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                *(int *)(msg + 0x5c) = U_I((char *)param_1 + 4);
                *(unsigned char *)(msg + 4) = 0x41;
                *(unsigned char *)(msg + 0x36) = 2;
                *(int *)(msg + 8) = U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x87);
                *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                *(unsigned short *)(msg + 0x34) = U_W(((unsigned)&ai_Time + 0xc));
                *(int *)(msg + 0xc) = U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x87);
                *(int *)(msg + 0x58) = U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x87);
                i = cm_SendLongMessage(msg);
                while (cm_GetSlotState(i) == 0)
                    sv_PumpNet();
                v = 1;
                U_I((unsigned)&sv_PopRequest) = 1;
            } else if (U_I((unsigned)&d2_ClickedId) == 1) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk5c6910, 0x657c7) == 0)
            break;
    }
    d2_CloseView(win);
    U_I((unsigned)&d2_ClickedId) = -1;
    di_ClearEvents(0x11);
    return v;
}
