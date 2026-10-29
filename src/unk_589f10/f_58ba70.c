/* 0x58ba70, the chat lines of the lobby (352 B): control 5 of 0x62aa88 and the text 0x602568; the chat records
   from 0x4cd480 / 0x4cd4c0 (0x6f2684 counts them): the speaker 0x4cd380 (the record +0x68), its id (+0xc) or 9;
   the line "$%dFF%s: %s$A" 0x62aac0 (the name +0x40, the text) for a record without +100, or
   "$%dFF%s (%s): %s$A" 0x62aad0 with the shout text U_I (0x144abf8) when +100 is the chosen player 0x6f5e78;
   each line through 0x515960; the newest line count grows the chat box (0x6f2684 > 0x6f2680 and the control
   height 0xf8f12a < the box) -> 0xf8f364 = +0x18, 0xf8f368 = the delta, 0x46f840 (the screen)
   @deep Q1 (unit unnamed, @named_by -); B: code 151 + regs 14 (the chat lines); Q2 (7 callees), Q3 (12 addresses), Q4 U_I + 2 casts */

void ai_Unk58ba70(void)
{
    char text[352];
    int p;
    int q;
    int id;
    int top;
    int n;

    id = -1;
    p = ai_Unk4cd480();
    d2_SetForm(U_I((unsigned)&full_data_62aa88), 5);
    q = U_I(((unsigned)&ai_Unkf8f13c + 0x228) + U_I((unsigned)&d2_LastWindow) * 0x3c4);
    io_PrintText((char *)(unsigned)"$C");
    U_I((unsigned)&full_data_6f2684) = 0;
    while (p != 0) {
        U_I((unsigned)&full_data_6f2684)++;
        n = ai_Unk4cd380(U_I(p + 0x68));
        if (n == 0 || U_I(n) != U_I(p + 0x68))
            id = 9;
        else
            id = U_I(n + 0xc);
        if (U_I(p + 100) == 0) {
            crt_Sprintf(text, (char *)(unsigned)&full_data_62aac0, id, (char *)(p + 0x40), (char *)p);
        } else if ((U_I((unsigned)&full_data_6f5e78) != 0 && U_I(p + 100) == U_I((unsigned)&full_data_6f5e78)) ||
                   U_I(p + 0x68) == U_I((unsigned)&full_data_6f5e78)) {
            crt_Sprintf(text, (char *)(unsigned)&full_data_62aad0, id, (char *)(p + 0x40), U_I((unsigned)&full_data_144abf8), (char *)p);
        }
        id = io_PrintText(text);
        p = ai_Unk4cd4c0();
    }
    if (U_I((unsigned)&full_data_6f2680) < U_I((unsigned)&full_data_6f2684) && -1 < id &&
        *(short *)(((unsigned)&ai_Unkf8f128 + 0x2) + U_I((unsigned)&d2_LastWindow) * 0x3c4) < q) {
        q = q + 0x18;
        U_I((unsigned)&full_data_6f2680) = U_I((unsigned)&full_data_6f2684);
        U_I(((unsigned)&ai_Unkf8f13c + 0x228) + U_I((unsigned)&d2_LastWindow) * 0x3c4) = q;
        U_I(((unsigned)&ai_Unkf8f13c + 0x22c) + U_I((unsigned)&d2_LastWindow) * 0x3c4) = q - *(short *)(((unsigned)&ai_Unkf8f128 + 0x2) + U_I((unsigned)&d2_LastWindow) * 0x3c4);
        d2_ScrollWindow(U_I((unsigned)&d2_LastWindow));
    }
}
