/* 0x59b630, the money lender's credit list of a second player (592 B): the same window (0x4741f0), the
   rectangle 0xe / 0x18 / 0x18 / 0xe, the text 0x18d9; the pump 0x5309a0 (0x677c7); on rebuild 0x50a150
   (param_2, param_1) gives the offers, each a row 0x4731d0 (0x75a) and the list 0x472aa0 (0x110), the texts
   0x18e0 (the value +0xb4), 0x18e1 (the name +0xc4) and 0x18e2 (the kind +0xc0); 0x4758f0 and 0x52d7a0 */

void ai_Unk59b630(int param_1, int param_2)
{
    int list[9 * 3];
    int offers[25];
    int win;
    int n;
    int i;
    int r;
    int v;
    short h;
    short w;

    for (i = 0; i < 9; i++) {
        list[i * 3] = -1;
        list[i * 3 + 1] = -1;
        list[i * 3 + 2] = 0;
    }
    win = d2_OpenForm(0, 0, "locations\\geldleihe\\geldleihe_kreditaufnehmen");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    n = U_I((unsigned)&d2_LastWindow) * 0x3c4;
    U_I(((unsigned)&ai_Unkf8f39c + 0x11c) + n) = 0xe;
    U_I(((unsigned)&ai_Unkf8f39c + 0x120) + n) = 0x18;
    U_I(((unsigned)&ai_Unkf8f39c + 0x124) + n) = 0x18;
    U_I(((unsigned)&ai_Unkf8f39c + 0x128) + n) = 0xe;
    d2_SetForm(win, 1);
    io_PrintText(0x18d9);
    d2_SetPointerSprite(0, 0);
    r = 1;
    for (;;) {
        if (sv_RunFrame(ai_Unk59b630, 0x677c7) == 0)
            break;
        i = 0;
        if (r) {
            for (i = 0; i < 9; i++) {
                if (list[i * 3] != -1) {
                    d2_RemoveWindow(list[i * 3]);
                    d2_RemoveObj(list[i * 3 + 1]);
                    list[i * 3] = -1;
                    list[i * 3 + 1] = -1;
                    list[i * 3 + 2] = 0;
                }
            }
            n = gm_Unk50a150(param_2, param_1, offers);
            h = *(short *)((unsigned)&d2_Pics + 0x269d8);
            w = *(short *)((unsigned)&d2_Pics + 0x269da);
            for (i = 0; i < n; i++) {
                d2_SetForm(win, 2);
                v = (w + 4) * i;
                list[i * 3] = d2_WinOpenPic(0, v, 0x75a, U_I((unsigned)&d2_LastWindow));
                list[i * 3 + 1] = d2_CreateChildWindow(0, v, h, w, 0x110, U_I((unsigned)&d2_LastWindow));
                list[i * 3 + 2] = offers[i];
                U_I((unsigned)&ai_Unkf8f39c + list[i * 3 + 1] * 0x1e2) = 0x56;
                *(unsigned short *)(((unsigned)&ai_Unkf8f128 + 0x8) + list[i * 3 + 1] * 0x3c4) = 0x10;
                io_PrintText(0x18e0, U_I(offers[i] + 0xb4));
                io_PrintText(0x18e1, offers[i] + 0xc4);
                io_PrintText(0x18e2, U_I(offers[i] + 0xc0));
            }
        }
        r = 0;
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
    }
    d2_CloseView(win);
    sv_CameraFollow();
}
