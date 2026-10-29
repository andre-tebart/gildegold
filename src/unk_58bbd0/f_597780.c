/* 0x597780, draws three dice (134 B): 0x472d20 (the screen, 1), the caption 0x515960 ("_C_Z_%s", param_1);
   the three dice cells at x = 0x1c + i * 0x3a (0 every 8) and y = 0x1d (0x46 after 8), each 0x4731d0 with the
   random picture 0x75b + (0x42ec00 (6, the screen) & 0xffff) */

void ai_Unk597780(int param_1)
{
    unsigned int i;
    unsigned int r;
    int x;
    int y;

    d2_RemoveWindowObjects(U_I((unsigned)&d2_LastWindow), 1);
    io_PrintText("_C_Z_%s", param_1);
    i = 0;
    x = 0x1c;
    y = 0x1d;
    do {
        r = ((int (*)(int, int))util_Random)(6, U_I((unsigned)&d2_LastWindow));
        d2_WinOpenPic(x, y, (r & 0xffff) + 0x75b, U_I((unsigned)&d2_LastWindow));
        i++;
        if (i % 8 == 0)
            x = 0;
        else
            x += 0x3a;
        y = ((i < 8) - 1U & 0x29) + 0x1d;
    } while (i < 3);
}
