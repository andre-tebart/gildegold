/* 0x585380, the graveyard "Fog" window (975 B): rec = the building record (+1 the id). The text 0x40c8 (0x40b6)
   into a 0x400 buffer, then a running 0x90 event with the id rec+1 (0x53e400 (2, 0, 0x90, 3, rec + 1)) -> 4c27d0
   (-1, 0, "_NACHRICHTEN_FRIEDHOF_14" 0x62a664, 0, 100), 0x521150 (buf, 0) and out. Else a 0x50-byte query record
   (1, -1, 0xa07c43b4, 1, 6 at +0x10, 0x727 at +0x4c) through 0x5b2180 (1, &q, U_I (0x144a498), 0, 0); 0 ->
   0x52d7a0 (). Else the window "Locations_Friedhof_Fog" (0x62a64c) with 0x476f80 / 0x475600, the sound
   0x52ea30, the text 0x40b7 (0x40b6, its owner word +0x27, res), two sliders 0x46a960 (100, 0xa0 / 0xbe, 200,
   0, 2, 0x7b, 0x82, sound) with 0x475cb0 (0, 2, 0), 0x46bdd0 (0x82), 0x46bc70 (U_I (0x144a4bc) / 0x144a4cc) and
   0x46bca0 (3, U_I (0x144a4c0..c8) / 0x144a4d0..d8), the entries 0x40be / +1 in 0x475b80 (window, 2, text),
   0xf8a668 = -1; each frame 0x475f70 (window, 3), 0x515960 (0x602568), the count (slider values + 2 multiplied)
   as 0x40b9 / 0x40ba, the pump 0x5309a0 (this window, 0x677c7): the escape 0x14673a0 -> 0x6f22a0 = 1; the first
   entry: 0x520f90 (the character, the count) == 1 -> the message (type 0x90 at +4, 2 at +0x36, the character
   0x770b84 at +8, rec + 1 at +0xc, the clock 0x770ae0 .. 0x770aec at +0x28, res + 1 at +0x58, the two slider
   values at +0x5c / +0x60) through 0x4c4b70 and 0x6f22a0 = 1; the second entry -> 0x6f22a0 = 1; the pump 0 ->
   0x4758f0 (window), 0x52d7a0 () */

void d2_SetSliderFlags(); int snd_QueueVoice(int id, int bank, char *text, int var, unsigned int delay); int ai_Unk5b2180();

void ai_Unk585380(char *rec)
{
    char buf[0x400];
    char msg[0xf8];
    char q[0x50];
    char *res;
    int win;
    int slider1;
    int slider2;
    int b1;
    int b2;
    int v;
    int n;

    txt_Format(buf, (char *)0x40c8, 0x40b6);
    if (he_FindEvent(2, 0, 0x90, 3, rec + 1) != 0) {
        snd_QueueVoice(-1, 0, (char *)(unsigned)&full_data_62a664, 0, 100);
        sv_MessageBox(buf, 0);
        return;
    }
    memset(q, 0, 0x50);
    *(int *)(q + 0x00) = 1;
    *(int *)(q + 0x04) = -1;
    *(int *)(q + 0x08) = 0xa07c43b4;
    *(int *)(q + 0x0c) = 1;
    *(int *)(q + 0x10) = 6;
    *(int *)(q + 0x4c) = 0x727;
    res = (char *)ai_Unk5b2180(1, q, U_I((unsigned)&full_data_144a498), 0, 0);
    if (res != 0) {
        win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a64c);
        d2_SetPointerSprite(0, 0);
        d2_CentreViewWindows(win);
        d2_SetForm(win, 0);
        sv_FitForm(U_I((unsigned)&d2_LastWindow));
        d2_SetForm(win, 1);
        io_PrintText(0x40b7, 0x40b6, *(unsigned short *)(res + 0x27), res);
        slider1 = d2_WinOpenSlider(100, 0xa0, 0xc8, 0, 2, 0x7b, 0x82, U_I((unsigned)&d2_LastWindow));
        d2_SetObjValue(slider1, 0, 2, 0);
        d2_SetSliderFlags(slider1, 0x82);
        d2_SetLabel(slider1, U_I((unsigned)&full_data_144a4bc));
        d2_SetList(slider1, 3, U_I((unsigned)&full_data_144a4c0), U_I((unsigned)&full_data_144a4c4), U_I((unsigned)&full_data_144a4c8));
        slider2 = d2_WinOpenSlider(100, 0xbe, 0xc8, 0, 2, 0x7b, 0x82, U_I((unsigned)&d2_LastWindow));
        d2_SetObjValue(slider2, 0, 2, 0);
        d2_SetSliderFlags(slider2, 0x82);
        d2_SetLabel(slider2, U_I((unsigned)&full_data_144a4cc));
        d2_SetList(slider2, 3, U_I((unsigned)&full_data_144a4d0), U_I((unsigned)&full_data_144a4d4), U_I((unsigned)&full_data_144a4d8));
        d2_SetForm(win, 2);
        v = io_PrintText(0x40be);
        b1 = d2_GetWinObj(win, 2, v);
        b2 = d2_GetWinObj(win, 2, v + 1);
        U_I((unsigned)&d2_ClickedId) = -1;
        for (;;) {
            d2_SetForm(win, 3);
            io_PrintText((char *)(unsigned)"$C");
            n = (d2_GetObjValue(slider1) + 2) * (d2_GetObjValue(slider2) + 2);
            io_PrintText(0x40b9);
            io_PrintText(0x40ba, n);
            if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
                U_I((unsigned)&sv_PopRequest) = 1;
            } else if (U_I((unsigned)&d2_ClickedId) != -1) {
                if (U_I((unsigned)&ai_Unk5fe710) == b1) {
                    if (sv_CheckMoney((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, n) == 1) {
                        memset(msg, 0, 0xf8);
                        *(unsigned char *)(msg + 0x04) = 0x90;
                        *(unsigned char *)(msg + 0x36) = 2;
                        *(int *)(msg + 0x08) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c);
                        *(int *)(msg + 0x0c) = U_I(rec + 1);
                        *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
                        *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                        *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                        *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
                        *(int *)(msg + 0x58) = U_I(res + 1);
                        *(int *)(msg + 0x5c) = d2_GetObjValue(slider1);
                        *(int *)(msg + 0x60) = d2_GetObjValue(slider2);
                        cm_SendLongMessage(msg);
                        U_I((unsigned)&sv_PopRequest) = 1;
                    }
                } else if (U_I((unsigned)&ai_Unk5fe710) == b2) {
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            }
            if (sv_RunFrame(ai_Unk585380, 0x677c7) == 0) {
                d2_CloseView(win);
                break;
            }
        }
    }
    sv_CameraFollow();
}
