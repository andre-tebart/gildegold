/* 0x58aa30, the city emblem window (368 B): control 2 of 0x62aa88, the text 0x602568; the city name 0x936476
   upcased (0x5ea986) into "..\..\Grafik\STADTWAPPEN\.." 0x6240e0 via 0x5de652, the image 0x468890 (the fallback
   "..\..\Grafik\STADTWAPPEN\BERLIN" 0x6240cc); the control 0x4731d0 (5, 5, the image, screen) and the text
   0x473650 (the control's x + 0xf, 0x19, the city name) and below it the population "%s: %d" 0x62aaa4 with
   U_I (0x144abd4) and the byte 0x936496
   @deep Q1 (unit unnamed, @named_by -); B: code 137 + fill 10 (the two identical city-name branches 0x9364dc == 0x3f are written once, note Open work) + Q2 (7 callees, incl. the text helpers 0x4731d0/0x473650 and 0x5de652), Q3 (10 addresses), Q4 U_I/U_B + 2 casts; needs the control/text prototypes and typed globals */

void ai_Unk58aa30(void)
{
    char a[256];
    char b[256];
    char c[256];
    int id;
    int x;
    int y;

    d2_SetForm(U_I((unsigned)&full_data_62aa88), 2);
    io_PrintText((char *)(unsigned)"$C");
    strcpy(a, (char *)(unsigned)&full_data_936476);
    strcpy(b, (char *)(unsigned)&full_data_936476);
    _strupr(a);
    sprintf(c, (char *)(unsigned)"_STADTWAPPEN_%s", a);
    id = d2_FindPic(c);
    if (id == -1)
        id = d2_FindPic((char *)(unsigned)"_STADTWAPPEN_BERLIN");
    id = d2_WinOpenPic(5, 5, id, U_I((unsigned)&d2_LastWindow));
    x = *(short *)(U_I((unsigned)&d2_Objs) + 0x14 + id * 0x2ec) + 0xf;
    y = d2_CreateWinText(x, 0x19, b, U_I((unsigned)&d2_LastWindow));
    sprintf(a, (char *)(unsigned)&full_data_62aaa4, U_I((unsigned)&full_data_144abd4), U_B((unsigned)&ai_Unk936496));
    d2_CreateWinText(x, *(short *)(U_I((unsigned)&d2_Objs) + 0x16 + y * 0x2ec) + 0x1b, a, U_I((unsigned)&d2_LastWindow));
}
