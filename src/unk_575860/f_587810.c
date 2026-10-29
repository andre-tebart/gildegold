/* 0x587810, the graveyard grave-detail window (692 B): rec = the record (+1 a dword). n = 0x4fe350 (0x770ae0);
   0x520fe0 (rec) != 0 -> return. U_I (0x6f2448) == 0 and n != 2 -> the line 0x413b, 0x5176d0 / 0x521150 (the
   line, 4), 0x52d7a0 (); else when a running 0x8f event exists (0x53e400 (2, 0, 0x8f, 3, rec + 1)) -> the line
   0x413c, 0x5176d0 / 0x521150 (the line, 4), 0x52d7a0 (). Otherwise the window 0x62a84c with 0x476f80 / 0x475600,
   the sound 0x52ea30, the sections 0 / 1 / 3, the text 0x4138, 0x4139 (0x228) and 0x4139 + 1 / + 2 in 0x475b80
   (window, 3, text) = the three buttons, 0x525220 (rec, 0); the pump loop 0x5309a0 (this window, 0x677c7): the
   escape 0x14673a0 -> 0x6f22a0 = 1; the first selected character (the byte +0x188, i = 0 .. 0x2ff): the three
   buttons by 0x504ec0 (the character, 1 / 2 / 4) < 1.0 in 0x475e40 (the button, the flag); a click on one ->
   sel 1 / 2 / 4: without a character 0x521150 (U_I (0x144a40c), 0), else 0x587700 (rec, the character, sel) != 0
   -> 0x6f22a0 = 1; the pump 0 -> 0x4758f0 (window), 0x525440 (), 0x52ae40 (), 0x52d7a0 () */

float gm_GetSkill(char *c, int skill); int gm_ClockSeason(); char * he_FindEvent(int n, ...); void d2_SetObjEnabled();

void ai_Unk587810(char *rec)
{
    char buf[0x400];
    char *f;
    int win;
    int b1;
    int b2;
    int b3;
    int n;
    int i;
    int sel;

    n = gm_ClockSeason((char *)(unsigned)&ai_Time);
    if (sv_CheckCharacterFlag(rec) != 0)
        return;
    if (U_I((unsigned)&ai_Unk6f2448) != 0 || n == 2) {
        if (he_FindEvent(2, 0, 0x8f, 3, rec + 1) != 0) {
            txt_Format(buf, (char *)0x413c);
            sv_MessageBox(buf, 4);
            sv_CameraFollow();
            return;
        }
    } else {
        txt_Format(buf, (char *)0x413b);
        sv_MessageBox(buf, 4);
        sv_CameraFollow();
        return;
    }
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a84c);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    io_PrintText(0x4138);
    d2_SetForm(win, 3);
    n = io_PrintText(0x4139, 0x228);
    b1 = d2_GetWinObj(win, 3, n);
    b2 = d2_GetWinObj(win, 3, n + 1);
    b3 = d2_GetWinObj(win, 3, n + 2);
    sv_Unk525220(rec, 0);
    while (sv_RunFrame(ai_Unk587810, 0x677c7) != 0) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        f = 0;
        for (i = 0; i < 0x300; i++) {
            if (*(char *)((unsigned)&ai_Chars + i * 0x21c + 0x188)) {
                f = (char *)((unsigned)&ai_Chars + i * 0x21c);
                break;
            }
        }
        if (f != 0) {
            d2_SetObjEnabled(b1, gm_GetSkill(f, 1) < 1.0);
            d2_SetObjEnabled(b2, gm_GetSkill(f, 2) < 1.0);
            d2_SetObjEnabled(b3, gm_GetSkill(f, 4) < 1.0);
        }
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == b1)
                sel = 1;
            else if (U_I((unsigned)&ai_Unk5fe710) == b2)
                sel = 2;
            else if (U_I((unsigned)&ai_Unk5fe710) == b3)
                sel = 4;
            else
                continue;
            if (f == 0) {
                sv_MessageBox((char *)U_I((unsigned)&full_data_144a40c), 0);
            } else if (ai_Unk587700(rec, f, sel) != 0) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
    }
    d2_CloseView(win);
    sv_ClosePlayerBar();
    sv_Unk52ae40();
    sv_CameraFollow();
}
