/* 0x5890c0, the drag window of a character of a lower kind (823 B, ROUGH SKELETON: the window, every call and
   the text ids are on the disassembly, the branch details around them are summarised): ch = the character
   record (+0 the word, +2 the kind). The kind 0xa and up -> return 0. The window 0x62a9f8 with 0x475600, the
   section 1, the text 0x62a9ec (the word), the section 4, the 0x38-byte record {ch} in 0x5be1d0 and, when it
   answers, 0x5be720 (0xa6, 5, U_I (0x6ed938), the line); the section 2; the kind != 0x14 -> 0x505e30 (the word)
   -> int in 0x515960 (0x2b); the flags and the texts 0x2c .. 0x41, the kind lists 0x53e400 / 0x53e300 and
   0x505df0 (ch) as in 0x588c60; the entries 0 .. 0x4d step 0xf: the line 0x62a56c (0x5176d0, i + 0x16b7),
   0x473650 (0, i * 0xf, the line, U_I (0x6ed938)) and 0x52eb50 (0x64, i * 0xf, ch, i, 0x4df); then the pump
   loop 0x5309a0 (this handler, 0x677c7): the escape 0x14673a0 -> 0x6f22a0 = 1; 0x520f20 () and
   0x4758f0 (the window); 0 */


int ai_Unk5890c0(char *ch)
{
    char line[0x100];
    char rec[0x38];
    int win;
    int i;
    int k;

    if (*(unsigned char *)(ch + 2) >= 0xa)
        return 0;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a9f8);
    d2_CentreViewWindows(win);
    d2_SetForm(0, 1);
    io_PrintText((unsigned)&full_data_62a9ec, *(unsigned short *)ch);
    d2_SetForm(0, 4);
    memset(rec, 0, 0x38);
    *(char **)rec = ch;
    if (ai_AmountRowStatus(rec) != 0)
        ai_Unk5be720(0xa6, 5, U_I((unsigned)&d2_LastWindow), line);
    d2_SetForm(0, 2);
    if (*(unsigned char *)(ch + 2) != 0x14)
        io_PrintText(0x2b, (int)*(float *)&U_I(((unsigned)&ai_Unk5ee160 + 0x7e8)));
    io_PrintText(0x2d);
    io_PrintText(0x2c, 0);
    if (gm_FindChar(ch) != 0)
        io_PrintText(0x2f);
    io_PrintText(0x31);
    io_PrintText(0x33);
    io_PrintText(0x34);
    io_PrintText(0x35, (char *)(unsigned)&full_data_62a9e4);
    if (he_FindEvent(2, 0, 0x41) != 0) {
        while (he_NextEvent() != 0)
            ;
    }
    for (i = 0, k = 2; k < 0x4d; i++, k += 0xf) {
        txt_Format(line, (char *)(unsigned)&full_data_62a56c, (char *)(i + 0x16b7));
        d2_CreateWinText(0, i * 0xf, line, U_I((unsigned)&d2_LastWindow));
        sv_Unk52eb50(0x64, i * 0xf, ch, i, 0x4df);
    }
    while (sv_RunFrame(ai_Unk5890c0, 0x677c7) != 0) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
    }
    sv_ResetScreenClip();
    d2_CloseView(win);
    return 0;
}
