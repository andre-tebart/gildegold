/* 0x5c5310, the player info panel "panel_info_spieler" 0x62d3e0 (4544 B, ROUGH SKELETON): the window
   0x4741f0 (0, 0, 0x62d3e0), 0x476f80, 0x475600, page 1 with 0x515960 (0x1f53, *param_1) and page 2
   with the age / birth texts (0x506000, 0x5176d0 (0x62d3d4 / 0x62d3b8), 0x5060d0, 0x505e30, the texts
   0x1f54 .. 0x1f60 and the formats 0x62d3a0 / 0x62d388 / 0x62d36c / 0x62d354 / 0x62d33c / 0x62d330),
   the five skills (param_1 + 0x34, 0x505df0, 0x515960 (0x62d320)) or the empty 0x62d314, page 3 with
   the five 0x473650 rows (0x16b7 + i, 0x52eb50 (0xaf, y, param_1, i, 0x4e5)) and the flag rows (the
   0x16 bits 0xf .. 0x1e000000, the texts 0x161e .. 0x1626); the bottom buttons (0x1f4b / 0x1f49 /
   0x1f50 through 0x515960 / 0x475b80 (win, 4)); the frame loop 0x5309a0 (this, 0x760638): the gift
   button state 0x509e60, the click 0x5fe710 -> 0x5c6050 (the gift), 0x5cf260 / 0x5cf4f0 (the identity
   windows), 0x4b5310 (the office), the confirmation 0x521150 and 0x4c4770 (the own id) with
   0x6f22a0 = 2; the exit 0x4758f0. ROUGH: the page / row coordinates and the condition ladder of the
   bottom buttons are comments */

int gm_BirthDate(); int gm_BirthDate2(); float gm_CharAge(); int gm_GetGoodsValue();
void ai_ShowMissions(int owner); int ai_Unk5cf260(); int ai_Unk5cf4f0(); int ai_Unk5c6050();

void ai_Unk5c5310(short *param_1, int param_2)
{
    int win;
    int b1;
    int b2;
    int b3;
    int b4;
    int b5;
    int i;
    int v;
    char text[32];
    int call_id;

    b1 = -1;
    b3 = -1;
    b5 = -1;
    b2 = -1;
    b4 = -1;
    v = 0;
    call_id = U_I((unsigned)&sv_FrameMask);
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d3e0);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1f53, *param_1);
    d2_SetForm(win, 2);
    gm_BirthDate(param_1, (char *)0x444);   /* ROUGH: the local record */
    txt_Format(text, (char *)(unsigned)&full_data_62d3d4, 0, 0, 0);
    io_PrintText((char *)(unsigned)&full_data_62d3c4, text);
    if (gm_BirthDate2(param_1, (char *)0x444) == 0) {
        gm_CharAge(*param_1);
        io_PrintText((char *)(unsigned)&full_data_62d3a0, 0x1f54, v);
    } else {
        txt_Format(text, (char *)(unsigned)&full_data_62d3b8, 0, 0, 0);
        io_PrintText((char *)(unsigned)&full_data_62d3c4, text);
    }
    /* ROUGH: the status ladder (param_1 + 0xb2 / +0x165 / +0xb3 / +0x169 the texts 0x1f55 .. 0x1f58
       and the formats 0x62d388 / 0x62d36c / 0x62d354), the profession 0x505df0 (param_1 + 0x2e) and the
       0x53e400 (1, 0, 0x41) walk for the master (0x1f5d .. 0x1f60), the age 0x488 / 0x483 and the
       home 0x1f5a; then page 3: the five skill rows 0x515960 (0x16b7 + i) with 0x473650 (10, y,
       text, screen) and 0x52eb50 (0xaf, y, param_1, i, 0x4e5), and the 0x473650 rows of the flags
       0x16 (the texts 0x161e .. 0x1626) */
    d2_SetForm(win, 3);
    for (i = 0; i < 5; i++) {
        txt_Format(text, (char *)(0x16b7 + i));
        d2_CreateWinText(10, (i + 1) * 0x18 + 1, text, U_I((unsigned)&d2_LastWindow));
        sv_Unk52eb50(0xaf, 2 + i * 0x18, param_1, i, 0x4e5);
    }
    txt_Format(text, (char *)0x1f63);
    d2_CreateWinText(0, (i + 2) * 0x18 + 10, text, U_I((unsigned)&d2_LastWindow));
    /* ROUGH: the flag rows (*(unsigned int *)(param_1 + 0x16) & 0xf / 0xf0 / 0xf00 / 0x3000 / 0x1c000
       / 0xe0000 / 0x700000 / 0x1800000 / 0x1e000000 -> the texts 0x161e .. 0x1626 through 0x473650) */
    if (param_2 == 0) {
        /* ROUGH: the gift button (the own character's +9 byte and +0x30 / +0x32 ids against 0x770b84,
           0x53e400 (2, 2, *param_1, 0, 0x5e), the 0x770d48 bit 0x20, the 0x52e350 pairs and the
           0x1f4b / 0x1a96 / 0x1a97 / 0x1f49 texts into 0x475b80 (win, 4)) */
        b1 = d2_GetWinObj(win, 4, io_PrintText(0x1f49));
    } else {
        d2_SetForm(win, 4);
        b5 = d2_GetWinObj(win, 4, io_PrintText(0x1f50, *param_1));
    }
    U_I((unsigned)&d2_ClickedId) = -1;
    U_I((unsigned)&ai_Unk5fe710) = -1;
    for (;;) {
        if (b3 != -1)
            d2_SetObjEnabled(b3, 0 < gm_GetGoodsValue((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c));
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            d2_SetViewEnabled(win, 0);
            if (U_I((unsigned)&ai_Unk5fe710) == b5) {
                txt_Format(text, (char *)0x1f51, *param_1, *param_1);
                if (sv_MessageBox(text, 1) != 0) {
                    ai_Unk4c4770(U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x87), *(int *)((char *)param_1 + 2));
                    U_I((unsigned)&sv_PopRequest) = 2;
                }
            } else if (U_I((unsigned)&ai_Unk5fe710) == b3) {
                if (ai_Unk5c6050((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c, param_1) != 0)
                    d2_SetObjEnabled(b3, 0);
            } else if (U_I((unsigned)&ai_Unk5fe710) == b2) {
                if (ai_Unk5cf260((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c, param_1) != 0)
                    d2_SetObjEnabled(b2, 0);
            } else if (U_I((unsigned)&ai_Unk5fe710) == b4) {
                if (ai_Unk5cf4f0((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c, param_1) != 0)
                    d2_SetObjEnabled(b4, 0);
            } else if (U_I((unsigned)&ai_Unk5fe710) == b1) {
                ai_ShowMissions(*(int *)((char *)param_1 + 2));
            }
            d2_SetViewEnabled(win, 1);
        }
        if (sv_RunFrame(ai_Unk5c5310, call_id) == 0)
            break;
    }
    d2_CloseView(win);
}
