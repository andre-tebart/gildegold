/* 0x5c6aa0, the advertising list "privillegien_werbung1" 0x62d424 (608 B): 0x5c64c0 (param_1, &n, the
   ids); the window 0x4741f0 (0, 0, 0x62d424), 0x475600, page 1 with 0x515960 (0x1ce3), page 2 with the
   rows: x = (i % 3 + 1) * (the width / 4) - 10 + the centre + (the cell + 10) * (i % 3),
   y = (i / 3) * 0x82 + 5; each row (0xe dwords) [0] = 0x505df0 (the id), [0xb] = the rating 0x5c6620
   (the own id, the partner + 4), [0xc] = 1, 0x5be1d0 (the row), the text 0x5176d0 (0x1ce4, the
   rating) and 0x5be3b0 (x, y, screen, 0x56, the row, 1) with 0x5bd0c0 (text, x + the cell / 2,
   y + 0x2b, 0xca, 0x56); the frame loop 0x5309a0 (this, 0x657c7): a click on a row -> 0x4754d0 (win, 0),
   0x5c6910 (the character) -> 0x92 and 0x6f22a0, else 0x4754d0 (win, 1); the exit 0x4758f0, returns
   the result (2 or 0x92) */

int ai_Unk5c6aa0(short *param_1)
{
    int win;
    int n;
    int result;
    int i;
    int v;
    int ids[16];
    int rows[0xe * 16];
    char text[128];

    result = 2;
    ai_Unk5c64c0((int)param_1, &n, ids);
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d424);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1ce3);
    d2_SetForm(win, 2);
    for (i = 0; i < n; i++) {
        int *row = rows + i * 0xe;
        row[4] = (i % 3 + 1) * 0x14 + (i % 3) * 0x1e;
        row[5] = (i / 3) * 0x82 + 5;
        row[0] = gm_FindChar(ids[i]);
        row[0xc] = 1;
        ai_Unk5be1d0(row);
        row[0xb] = ai_Unk5c6620(U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x87), U_I(row[0] + 4));
        txt_Format(text, (char *)0x1ce4, row[0xb]);
        ai_Unk5be3b0(row[4], row[5], U_I((unsigned)&d2_LastWindow), 0x56, row, 1);
        ai_Unk5bd0c0(text, row[4] + 0x20, row[5] + 0x2b, 0xca, 0x56);
    }
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1 && U_I(((unsigned)&dm_L + 0x18)) != 0) {
            for (i = 0; i < n; i++) {
                if (rows[i * 0xe + 1] == U_I((unsigned)&ai_Unk5fe710)) {
                    d2_EnableViewObjects(win, 0);
                    v = ai_Unk5c6910((short *)rows[i * 0xe]);
                    if (v != 0) {
                        result = 0x92;
                        U_I((unsigned)&sv_PopRequest) = 1;
                        break;
                    }
                    d2_EnableViewObjects(win, 1);
                }
            }
        }
        if (sv_RunFrame(ai_Unk5c6aa0, 0x657c7) == 0)
            break;
    }
    d2_CloseView(win);
    return result;
}
