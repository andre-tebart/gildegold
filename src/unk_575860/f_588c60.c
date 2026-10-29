/* 0x588c60, the drag window of a character (1111 B, ROUGH SKELETON: the window, every call and the text ids are
   on the disassembly; the branch details around them are summarised): ch = the character record (+0 the word,
   +2 the kind, +0xd a byte, +0x164 / +0x166 / +0x169 bytes). The window 0x62a9f8 with 0x475600, the section 1,
   the text 0x62a9ec (the word), the section 4, the record {ch} in 0x5be1d0 and, when it answers, 0x5be720 (0xa6,
   5, U_I (0x6ed938), the line); the section 2, the kind != 0x14 -> 0x505e30 (the word) -> int in 0x515960
   (0x2b). The flags: the word 0x3000 (ebx), the bytes +0x164 / +0x166 / +0x169, the texts 0x2d / 0x2c (the mask
   0x5f + 0x130 + the byte), 0x2f / 0x30 / 0x31 / 0x32 / 0x33 / 0x34 / 0x35 (0x62a9e4) / 0x41, the kind lists
   0x53e400 / 0x53e300, 0x505df0 (ch) and 0x505df0 (ch + 0x68), 0x62a9d8, 0x62a9e4; the line 0x62a56c
   (0x5176d0), 0x473650, 0x52eb50 (0x64, 0x4df) and 0x576040; the sections via 0x475f70; the window */

float gm_CharAge(); int ai_AmountRowStatus(); int ai_Unk5be720(); short * gm_FindChar(int id); int gm_TitlesLeft();

int ai_Unk588c60(char *ch)
{
    char line[0x100];
    char *w;
    int win;
    int v;

    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a9f8);
    w = (char *)win;
    d2_CentreViewWindows(win);
    d2_SetForm(0, 1);
    io_PrintText((unsigned)&full_data_62a9ec, *(unsigned short *)ch);
    d2_SetForm(0, 4);
    *(char **)line = ch;
    if (ai_AmountRowStatus(line) != 0)
        ai_Unk5be720(0xa6, 5, U_I((unsigned)&d2_LastWindow), line);
    d2_SetForm(0, 2);
    if (*(unsigned char *)(ch + 2) != 0x14)
        io_PrintText(0x2b, (int)*(float *)&U_I(((unsigned)&ai_Unk5ee160 + 0x7e8)));
    /* ROUGH: the flags ebx = 0x3000, the bytes +0x164 / +0x166 / +0x169 and the texts 0x2d (the mask 0x5f + 0x130
       + the byte +0x164), 0x2c (both branches), 0x2f / 0x30 / 0x31 (0x505df0 (ch) on the byte), 0x32 / 0x41
       (0x53e400 / 0x53e300), 0x33 / 0x34 / 0x35 (0x62a9e4) / 0x62a9d8 (0x505df0 (ch + 0x68)) */
    io_PrintText(0x2d);
    io_PrintText(0x2c, 0);
    if (gm_FindChar(ch) != 0)
        io_PrintText(0x2f);
    else
        io_PrintText(0x32);
    io_PrintText(0x33);
    io_PrintText(0x34);
    io_PrintText(0x35, (char *)(unsigned)&full_data_62a9e4);
    if (he_FindEvent(2, 0, 0x41) != 0) {
        while (he_NextEvent() != 0)
            ;
    }
    txt_Format(line, (char *)(unsigned)&full_data_62a56c, 0);
    d2_CreateWinText(0, 0, line, U_I((unsigned)&d2_LastWindow));
    sv_Unk52eb50(0x64, 0x4df);
    ai_Unk576040(7, ch, 0, 0);
    d2_SetForm(0, 3);
    (void)w;
    (void)v;
    return win;
}
