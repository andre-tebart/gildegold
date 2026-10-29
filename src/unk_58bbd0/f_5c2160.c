/* 0x5c2160, the law book listener (657 B): the pages 0x111 (Strafrecht), 0x112 (Steuern) and 0x113
   (Verfassung) open the window 0x52f360 (the object names 0x62bde4 / 0x62be00 / 0x62bdc8, the book window
   0x62d294), the section texts 0x13f8 / 0x13f7 / 0x13f6, the entries start .. end (0x11 .. 0x1d, 9 .. 0x11,
   0 .. 9) in the two pages +0x228 / +0x22c (a break after 6 / 4 / 5 rows), each row 0x4f2fe0 + 0x515960
   (0x62d284) + 0x475b80; the click opens 0x5c2410 (base 0x1452 / 0x142a / 0x13fd + i * 5, the kind text of
   the record 0x4f2720 +0x18 by the byte +1: 0 / +0x13dd / +0x13e6 / +0x13ec); the pump 0x5309a0 (this,
   0x677c7) and 0x52f530 / 0x52d7a0 at the end */

int sv_Unk52f360(); int sv_Unk52f600(); int sv_Unk52f530(); int gv_TableRecordText();
void ai_Unk5c2410();

int ai_Unk5c2160(int param_1)
{
    int win;
    int page;
    int start, end, brk;
    int base;
    int text;
    int ctrl[12];
    int i, n;
    char buf[512];
    char pos[0x24];

    if (param_1 == 0x111) {
        win = sv_Unk52f360((char *)(unsigned)"ob_GESETZBUCH_STRAFRECHT", (char *)(unsigned)"Buch", 2, (char *)(unsigned)&full_data_62d294, (char *)(unsigned)&full_data_62d294);
        start = 0x11;
        end = 0x1d;
        base = 0x1452;
        text = 0x13f8;
        brk = 6;
    } else if (param_1 == 0x112) {
        win = sv_Unk52f360((char *)(unsigned)"ob_GESETZBUCH_STEUERN", (char *)(unsigned)"Buch", 2, (char *)(unsigned)&full_data_62d294, (char *)(unsigned)&full_data_62d294);
        start = 9;
        end = 0x11;
        base = 0x142a;
        text = 0x13f7;
        brk = 4;
    } else if (param_1 == 0x113) {
        win = sv_Unk52f360((char *)(unsigned)"ob_GESETZBUCH_VERFASSUNG", (char *)(unsigned)"Buch", 2, (char *)(unsigned)&full_data_62d294, (char *)(unsigned)&full_data_62d294);
        start = 0;
        end = 9;
        base = 0x13fd;
        text = 0x13f6;
        brk = 5;
    } else {
        return 0;
    }
    d2_SetForm(U_I(win + 0x228), 1);
    io_PrintText(text);
    d2_SetForm(U_I(win + 0x228), 2);
    page = U_I(win + 0x228);
    i = 0;
    for (n = start; n < end; n++) {
        if (i == brk) {
            d2_SetForm(U_I(win + 0x22c), 2);
            page = U_I(win + 0x22c);
        }
        gv_TableRecordText(buf, n);
        text = io_PrintText((char *)(unsigned)&full_data_62d284, buf);
        ctrl[i] = d2_GetWinObj(page, 2, text);
        i++;
    }
    d2_SetForm(U_I(win + 0x228), 3);
    io_PrintText(0x1a38, 1);
    d2_SetForm(U_I(win + 0x22c), 3);
    io_PrintText(0x1a38, 2);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            for (i = 0; i < 0xc; i++) {
                if (ctrl[i] == U_I((unsigned)&ai_Unk5fe710)) {
                    gv_GetTableRecord(i + start, (int *)pos);
                    switch (*(unsigned char *)(pos + 1)) {
                    case 0:
                        text = U_I(pos + 0x18);
                        break;
                    case 1:
                        text = U_I(pos + 0x18) + 0x13dd;
                        break;
                    case 2:
                        text = U_I(pos + 0x18) + 0x13e6;
                        break;
                    case 3:
                        text = U_I(pos + 0x18) + 0x13ec;
                        break;
                    default:
                        text = 0;
                        break;
                    }
                    ai_Unk5c2410(base + i * 5, text);
                    break;
                }
            }
        }
        sv_Unk52f600(win);
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (sv_RunFrame(ai_Unk5c2160, 0x677c7) == 0) {
            sv_Unk52f530(win);
            sv_CameraFollow();
            return 0;
        }
    }
}
