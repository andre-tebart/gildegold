/* 0x594bb0, the bribe callback of 0x594db0 (502 B, ROUGH SKELETON from the disassembly; an unnamed Ghidra
   start): rec (0 / +0x27 == 0xffff returns); 0x553cc0 (rec) == 0 gives the text 0x1a92 (0x5176d0 / 0x521150);
   else the window (0x62b3b8, 0x4741f0 / 0x475600 / 0x476f80 / 0x475f70 (1)), the text 0x515960 (0x1a84, rec)
   and the two controls 0x475b80; the click 0xf8a668 / 0x5fe710 drives the message 0x4c4b70, the money
   0x50cc50, 0x4c2d20 and 0x521150; the pump 0x5309a0 (this, 0x677c7); the exit 0x4758f0 */

void gm_SendMoneyDelta(int a, int b);

void ai_Unk594bb0(int param_1, int rec)
{
    char text[0x100];
    char buf[0x400];
    int win;
    int ctl1;
    int ctl2;
    int i;

    if (rec == 0 || *(unsigned short *)(rec + 0x27) == 0xffff)
        return;
    if (he_Unk553cc0(rec) == 0) {
        txt_Format(text, 0x1a92, *(unsigned short *)(rec + 0x27), rec);
        sv_MessageBox(text, 0);
        return;
    }
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62b3b8);
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    i = io_PrintText(0x1a84, rec);
    ctl1 = d2_GetWinObj(win, 1, i);
    ctl2 = d2_GetWinObj(win, 1, i + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    do {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        /* ROUGH: the click on the two controls: the message 0x4c4b70, the money 0x50cc50, 0x4c2d20 and the
           text 0x521150 */
        i = sv_RunFrame(ai_Unk594bb0, 0x677c7);
    } while (i != 0);
    d2_CloseView(win);
}
