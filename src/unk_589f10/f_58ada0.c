/* 0x58ada0, the player list window (432 B): control 1 of 0x62aa88 and the text 0x602568; the eight player
   records 0x6f267c[i + 1] (0x4cd380): the city image 0x4731d0 (5, y, the city + 0x59a) and the line
   0x473650 (x + 10, y, "%s %s %s " 0x62aab4 of the honour title, the name +0x12 and the family name +0x22,
   plus the status text 0x144abf4 when +4 is set); the control id into 0x6f5e80[i], y += 5 + the control
   height; the chosen player 0x6f5e78 gives 0x6f5ea0 = i, the others are marked selectable (+0x4c = 1,
   +0x1c0 = 3); the record id equal 0x6f5e00 marks the control (+0x28 = 1)
   @deep Q1 (unit unnamed, @named_by -); B: code 117 + fill 3 + slots 2 (the original's strcat is a rep movs hand loop, note Open work); Q2 (6 callees), Q3 (13 addresses), Q4 U_I + 2 casts */

void ai_Unk58ada0(void)
{
    char text[256];
    char *u;
    int *p;
    int y;
    int i;
    int id;
    int n;

    y = 10;
    d2_SetForm(U_I((unsigned)&full_data_62aa88), 1);
    io_PrintText((char *)(unsigned)"$C");
    for (i = 0; i < 8; i++) {
        p = (int *)ai_Unk4cd380(U_I((unsigned)&full_data_6f267c + 4 + i * 4));
        if (p == 0)
            continue;
        id = d2_WinOpenPic(5, y, p[3] + 0x59a, U_I((unsigned)&d2_LastWindow));
        u = (char *)U_I(((unsigned)&ai_Unk143a610 + 0x4));
        if ((char)p[4] != 0)
            u = (char *)U_I((unsigned)&full_data_143a63c);
        sprintf(text, (char *)(unsigned)&full_data_62aab4, u, (char *)p + 0x12, (char *)p + 0x22);
        if (p[1] != 0)
            strcat(text, (char *)U_I((unsigned)&full_data_144abf4));
        n = id * 0x2ec;
        d2_CreateWinText(*(short *)(n + 0x14 + U_I((unsigned)&d2_Objs)) + 10, y, text, U_I((unsigned)&d2_LastWindow));
        y = y + 5 + *(short *)(n + 0x16 + U_I((unsigned)&d2_Objs));
        U_I((unsigned)&full_data_6f5e80 + i * 4) = id;
        if (p == (int *)U_I((unsigned)&full_data_6f5e78)) {
            U_I((unsigned)&full_data_6f5ea0) = i;
        } else {
            U_I(n + 0x4c + U_I((unsigned)&d2_Objs)) = 1;
            U_I(n + 0x1c0 + U_I((unsigned)&d2_Objs)) = 3;
        }
        if (U_I((unsigned)&full_data_6f5e00) == *p)
            U_I(n + 0x28 + U_I((unsigned)&d2_Objs)) = 1;
    }
}
