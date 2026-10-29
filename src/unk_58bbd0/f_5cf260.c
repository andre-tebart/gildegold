/* 0x5cf260, the profession chooser "privillegien_CHOOSEPROFESSION2" 0x62d660 (656 B): param_1 the
   request record, param_2 the character record. The price (int)((float)gm_GetWorth (*param_1) * 0.015f
   (0x5ecec8)); the window 0x4741f0 (0, 0, 0x62d660) with the text 0x515960 (0x1a9a, the own byte +9 ?
   0x1aa1 : 0x1aa0, the id, the price); the sixteen 0x5a2 .. 0x5b1 icons in a 5 x 3 grid: the width word
   +8 of the screen record 0xf8f128 + screen * 0x3c4 over 5, the height word +0xa over 3, the offset
   0x1d978 of U_I (0x6ed918) as the margin; each control 0x4731d0 gets the visible flag +0x4c = 1, the
   picture +0x1c0 = 3, the string of the table 0x143a97c at +0xbc and +0x1bc = 0x56. The pump
   0x5309a0 (U_I (0x760638)): a click 0x5a2 + i with U_I (0x1467398) sends the message (type 0x5e, +8 the
   request + 4, +0xc = -1, +0x36 = 0x12, +0x58 = param_1 + 4, +0x5c the byte 0x5043d0 (i + 1, 1),
   +0x60 the price) and returns 1; the exit 0x4758f0. */

int ai_Unk5cf260(unsigned short *param_1, unsigned short *param_2)
{
    int rec[0x3e];
    int win;
    int price;
    int h;
    int w;
    int dx;
    int y;
    int ctrl;
    int i;
    int ret;

    price = (int)((float)gm_GetWorth(*param_1) * 0.015f);
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d660);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    h = *(short *)(((unsigned)&ai_Unkf8f128 + 0x2) + U_I((unsigned)&d2_LastWindow) * 0x3c4);
    w = (*(short *)((unsigned)&ai_Unkf8f128 + U_I((unsigned)&d2_LastWindow) * 0x3c4) - 0x40) / 5;
    dx = (w - *(short *)(U_I((unsigned)&d2_Pics) + 0x1d978)) / 2;
    io_PrintText(0x1a9a, (*(char *)((char *)param_2 + 9) != 0) + 0x1aa0, *param_2, price);
    for (i = 0; i < 0x10; i++) {
        ctrl = d2_WinOpenPic((i % 5) * w + 0x20 + dx,
                            (i / 5) * ((h - 0x84) / 3) + 100 + dx, i + 0x5a2,
                            U_I((unsigned)&d2_LastWindow)) * 0x2ec + U_I((unsigned)&d2_Objs);
        *(int *)(ctrl + 0x4c) = 1;
        *(int *)(ctrl + 0x1c0) = 3;
        strcpy((char *)(ctrl + 0xbc), ((char **)((unsigned)&ai_Unk143a680 + 0x2fc))[i]);
        *(int *)(ctrl + 0x1bc) = 0x56;
    }
    ret = 0;
    for (;;) {
        if (sv_RunFrame(ai_Unk5cf260, U_I((unsigned)&sv_FrameMask)) == 0) {
            d2_CloseView(win);
            return ret;
        }
        if (U_I(((unsigned)&dm_L + 0x20)) == 0) {
            if (U_I(((unsigned)&dm_L + 0x18)) != 0) {
                ret = 1;
                for (i = 0; i < 0x10; i++) {
                    if (U_I((unsigned)&d2_ClickedId) == i + 0x5a2) {
                        *(char *)((char *)rec + 4) = 0x5e;
                        rec[2] = *(int *)(param_2 + 2);
                        rec[3] = -1;
                        *(char *)((char *)rec + 0x36) = 0x12;
                        rec[0x16] = *(int *)(param_1 + 2);
                        *(char *)((char *)rec + 0x5c) = gm_TitleNumber(i + 1, 1);
                        rec[0x18] = price;
                        cm_SendLongMessage(rec);
                        U_I((unsigned)&sv_PopRequest) = 1;
                        break;
                    }
                }
            }
        } else {
            U_I((unsigned)&sv_PopRequest) = 1;
        }
    }
}
