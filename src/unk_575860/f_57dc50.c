/* 0x57dc50, a tutorial skill window (520 B): arg 1 = the record, arg 2 = the value put into the message, arg 3 =
   the skill index. The lookup 0x4eb180 (record, &a, &b, 0) != 0 or index 3 clears the answer flag; the window
   "Locations_Domizil_skill" (0x62a2c4) with 0x476f80 (0, 0) / 0x475600, the sound 0x52ea30 (0x6ed938), the
   texts 0x4045 (index + 0x4172), index + 0x4176 and 0x4046 (0x182, only with the answer), the two entries
   0x475b80 (window, 2, 0x515960 (0x4047) / +1) and 0x475e40 (the first entry, the answer), 0xf8a668 = -1.
   Then the pump loop 0x5309a0 (this handler, 0x677c7): the click 0x14673a0 -> 0x6f22a0 = 1; the window
   0x5fe710 == the first entry -> the message (0xf8 bytes: type 0x97 at +4, 1 at +0x36, the record +1 dword at
   +0xc, the character 0x770b84 [word record +0x27] at +8, the game time 0x770ae0 at +0x28, arg[0] = *arg 2 at
   +0x58, arg[1] = the index at +0x5c, arg[4] = random 0x42ec00 (0x5a0) & 0xffff + 1000 at +0x68) and
   0x6f22a0 = 1; the second entry -> 0x6f22a0 = 1; the pump 0 -> 0x4758f0 (window), 1 */



int ai_Unk57dc50(char *rec, int *arg, int idx)
{
    char msg[0xf8];
    char *win;
    int a;
    int b;
    int t1;
    int t2;
    int ok;

    ok = 1;
    if (ahm_ResearchState(rec, &a, &b, 0) != 0)
        ok = 0;
    if (idx == 3)
        ok = 0;
    win = (char *)d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a2c4);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    io_PrintText(0x4045, idx + 0x4172);
    io_PrintText(idx + 0x4176);
    if (ok)
        io_PrintText(0x4046, 0x182);
    d2_SetForm(win, 2);
    a = io_PrintText(0x4047);
    t1 = d2_GetWinObj(win, 2, a);
    t2 = d2_GetWinObj(win, 2, a + 1);
    d2_SetObjEnabled(t1, ok);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == t1) {
                memset(msg, 0, 0xf8);
                *(unsigned char *)(msg + 0x04) = 0x97;
                *(unsigned char *)(msg + 0x36) = 1;
                *(int *)(msg + 0x0c) = *(int *)(rec + 1);
                *(int *)(msg + 0x08) = ((int *)(unsigned)&ai_CharInts)[*(unsigned short *)(rec + 0x27) * 0x87];
                *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
                *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
                *(int *)(msg + 0x58) = *arg;
                *(int *)(msg + 0x5c) = idx;
                *(int *)(msg + 0x68) = (util_Random(0x5a0) & 0xffff) + 1000;
                cm_SendLongMessage(msg);
                U_I((unsigned)&sv_PopRequest) = 1;
            } else if (U_I((unsigned)&ai_Unk5fe710) == t2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk57dc50, 0x677c7) == 0) {
            d2_CloseView(win);
            return 1;
        }
    }
}
