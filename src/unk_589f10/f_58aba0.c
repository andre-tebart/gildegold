/* 0x58aba0, the current player's info window (384 B): the player record 0x6f267c[0x6f5ea0 + 1] (0x4cd380)
   gives the city (+0xc); control 3 of 0x62aa88 and the text 0x602568; the city image control 0x4731d0
   (5, 10, city + 0x59a) and the name / family name text 0x473650 (x + 0xf, 10); then "%s: %s" 0x62aaac
   with the honour title of the gender (U_I (0x143a614), U_I (0x143a63c) for the female byte 0x6f5e30) and
   the profession (the tables 0x143a7fc / 0x143a680 by 0x6f5e2b) and the religion (0x143b3d8 by 0x6f5e31),
   each below the last
   @deep Q1 (unit unnamed, @named_by -); B: code 117 + slots 12 + fill 3; Q2 (6 callees incl. 0x4731d0/0x473650/0x5de652), Q3 (21 addresses), Q4 U_I/U_B + 2 casts; the same missing control API and typed player record */

void ai_Unk58aba0(void)
{
    char text[256];
    char *u;
    int p;
    int id;
    int x;
    int y;

    p = ai_Unk4cd380(U_I((unsigned)&full_data_6f267c + 4 + U_I((unsigned)&full_data_6f5ea0) * 4));
    if (p != 0)
        U_I((unsigned)&full_data_6f5e2c) = U_I(p + 0xc);
    d2_SetForm(U_I((unsigned)&full_data_62aa88), 3);
    io_PrintText((char *)(unsigned)"$C");
    id = d2_WinOpenPic(5, 10, U_I((unsigned)&full_data_6f5e2c) + 0x59a, U_I((unsigned)&d2_LastWindow));
    x = *(short *)(U_I((unsigned)&d2_Objs) + 0x14 + id * 0x2ec) + 0xf;
    u = (char *)U_I(((unsigned)&ai_Unk143a610 + 0x4));
    if (U_B((unsigned)&full_data_6f5e30) != 0)
        u = (char *)U_I((unsigned)&full_data_143a63c);
    crt_Sprintf(text, (char *)(unsigned)"%s %s %s", u, (char *)(unsigned)&full_data_6f5e32, (char *)(unsigned)&full_data_6f5e42);
    id = d2_CreateWinText(x, 10, text, U_I((unsigned)&d2_LastWindow));
    y = *(short *)(U_I((unsigned)&d2_Objs) + 0x16 + id * 0x2ec);
    if (U_B((unsigned)&full_data_6f5e30) == 1)
        u = (char *)U_I(((unsigned)&ai_Unk143a680 + 0x17c) + U_B((unsigned)&full_data_6f5e2b) * 4);
    else
        u = (char *)U_I((unsigned)&ai_Unk143a680 + U_B((unsigned)&full_data_6f5e2b) * 4);
    crt_Sprintf(text, (char *)(unsigned)&full_data_62aaac, U_I((unsigned)&full_data_144abe0), u);
    d2_CreateWinText(x, y + 0xc, text, U_I((unsigned)&d2_LastWindow));
    crt_Sprintf(text, (char *)(unsigned)&full_data_62aaac, U_I((unsigned)&full_data_144abe4), U_I((unsigned)&ai_Unk143b3d8 + U_B((unsigned)&full_data_6f5e31) * 4));
    d2_CreateWinText(x, y + 2 + y + 0xc, text, U_I((unsigned)&d2_LastWindow));
}
