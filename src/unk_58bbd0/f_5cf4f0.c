/* 0x5cf4f0, the identity window "special_uni" 0x62d6bc (816 B): param_1 unused, param_2 the character
   record. The character's own record 0x770b80 + U_W (0x6f25b8) * 0x21c is copied by 0x4948f0 into
   ident (three 0xc-byte entries); the window 0x4741f0 (0, 0, 0x62d6bc) page 1 with the text 0x515960
   (0x1a9d, the own byte +9 ? 0x1aa1 : 0x1aa0, the id, 5), page 2 the three rows (y = i * 0x5a + 10):
   0x4731d0 (0x14, y, 0x4c6, screen) with the visible flag +0x4c = 1 and +0x1c0 = 3, then four texts
   0x473650 (0xbe, y + 10 / 0x1e / 0x32 / 0x46): the player name 0x7e4234 + i * 0x2f4, the two texts
   ident[i * 3 + 1] + 0x16b7 / ident[i * 3 + 2] + 0x16b7 and the number ident[i * 3] through the format
   0x60d224; page 3 the button 0x515960 (0x1ba8) / 0x475b80. The pump 0x5309a0 (U_I (0x760638)): click
   i of the three rows sends the message (type 0x5f, +8 the request + 4, +0xc = -1, +0x36 = 0x12, +0x58
   the player's character 0x770b84 + U_W (0x6f25b8) * 0x21c, +0x60 = ident[i * 3], +0x64 / +0x65 the two
   bytes of ident[i * 3 + 1] / + 2) and the flag 0x4c4a60 (+0x1c8, 4, 0x10000); the exit 0x4758f0. */

int ai_Unk5cf4f0(int param_1, unsigned short *param_2)
{
    int ident[9];
    int ctrl[4];
    int rec[0x3e];
    char text[0x100];
    int win;
    int t;
    int y;
    int i;
    int ret;

    (void)param_1;
    ai_Unk4948f0((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c, ident);
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d6bc);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1a9d, (*(char *)((char *)param_2 + 9) != 0) + 0x1aa0, *param_2, 5);
    d2_SetForm(win, 2);
    for (i = 0; i < 3; i++) {
        y = i * 0x5a;
        ctrl[i] = d2_WinOpenPic(0x14, y + 10, 0x4c6, U_I((unsigned)&d2_LastWindow));
        *(int *)(ctrl[i] * 0x2ec + 0x4c + U_I((unsigned)&d2_Objs)) = 1;
        *(int *)(ctrl[i] * 0x2ec + 0x1c0 + U_I((unsigned)&d2_Objs)) = 3;
        txt_Format(text, "%s", (char *)(((unsigned)&ai_Unk7e422c + 0x8) + i * 0x2f4));
        d2_CreateWinText(0xbe, y + 10, text, U_I((unsigned)&d2_LastWindow));
        txt_Format(text, "%s", (char *)(ident[i * 3 + 1] + 0x16b7));
        d2_CreateWinText(0xbe, y + 0x1e, text, U_I((unsigned)&d2_LastWindow));
        txt_Format(text, "%s", (char *)(ident[i * 3 + 2] + 0x16b7));
        d2_CreateWinText(0xbe, y + 0x32, text, U_I((unsigned)&d2_LastWindow));
        txt_Format(text, (char *)(unsigned)"%T", ident[i * 3]);
        d2_CreateWinText(0xbe, y + 0x46, text, U_I((unsigned)&d2_LastWindow));
    }
    d2_SetForm(win, 3);
    t = io_PrintText(0x1ba8);
    ctrl[3] = d2_GetWinObj(win, 3, t);
    d2_SetPointerSprite(0, 0);
    U_I((unsigned)&d2_ClickedId) = -1;
    ret = 0;
    for (;;) {
        if (sv_RunFrame(ai_Unk5cf4f0, U_I((unsigned)&sv_FrameMask)) == 0) {
            d2_CloseView(win);
            return ret;
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == ctrl[3]) {
                U_I((unsigned)&sv_PopRequest) = 1;
            } else {
                for (i = 0; i < 3; i++) {
                    if (U_I((unsigned)&ai_Unk5fe710) == ctrl[i]) {
                        *(char *)((char *)rec + 4) = 0x5f;
                        rec[2] = *(int *)(param_2 + 2);
                        rec[3] = -1;
                        *(char *)((char *)rec + 0x36) = 0x12;
                        rec[0x16] = U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x21c);
                        rec[0x18] = ident[i * 3];
                        *(char *)((char *)rec + 0x64) = (char)ident[i * 3 + 1];
                        *(char *)((char *)rec + 0x65) = (char)ident[i * 3 + 2];
                        cm_SendLongMessage(rec);
                        cm_SendSetField(*(int *)(param_2 + 2), 0x1c8, 4, 0x10000, 0);
                        ret = 1;
                        U_I((unsigned)&sv_PopRequest) = 1;
                        break;
                    }
                }
            }
        }
    }
}
