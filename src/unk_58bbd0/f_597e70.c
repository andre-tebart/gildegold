/* 0x597e70, draws both dice sets of a dice window (785 B, ROUGH SKELETON from the Ghidra pseudocode): the two
   records param_1 + 0xc / + 0x30 (the caption at +0x30, the count at +0x10 / +0x34, the painted dice at
   +0x18 / +0x3c, the plain pictures 0x761 when +0x28 == 4), the frames 0x475f70 (2 / 3 / 4), 0x472d20 (the
   screen, 1), the dice cells 0x4731d0 (x = 0x1c + i * 0x3a, 0 every 8, y = 0x1d / 0x46); the exit texts
   0x515960 */

void ai_Unk597e70(int param_1, int win)
{
    unsigned int i;
    int x;
    int y;
    int rec;
    int pic;

    d2_SetForm(win, 2);
    d2_RemoveWindowObjects(U_I((unsigned)&d2_LastWindow), 1);
    d2_SetForm(win, 3);
    d2_RemoveWindowObjects(U_I((unsigned)&d2_LastWindow), 1);
    rec = *(int *)(param_1 + 0xc);
    if (*(char *)(rec + 2) == 6) {
        d2_SetForm(win, 3);
        io_PrintText("_C_Z_%s", rec + 0x30);
        i = 0;
        x = 0x1c;
        y = 0x1d;
        while ((int)i < (int)*(unsigned char *)(param_1 + 0x10)) {
            d2_WinOpenPic(x, y, *(unsigned char *)(param_1 + 0x18 + i) + 0x75a, U_I((unsigned)&d2_LastWindow));
            i++;
            if (i % 8 == 0)
                x = 0;
            else
                x += 0x3a;
            y = ((i < 8) - 1U & 0x29) + 0x1d;
        }
    } else {
        d2_SetForm(win, 2);
        io_PrintText("_C_Z_%s", rec + 0x30);
        i = 0;
        x = 0x1c;
        y = 0x1d;
        while ((int)i < (int)*(unsigned char *)(param_1 + 0x10)) {
            if (U_I(param_1 + 0x28) == 4)
                pic = 0x761;
            else
                pic = *(unsigned char *)(param_1 + 0x18 + i) + 0x75a;
            d2_WinOpenPic(x, y, pic, U_I((unsigned)&d2_LastWindow));
            i++;
            if (i % 8 == 0)
                x = 0;
            else
                x += 0x3a;
            y = ((i < 8) - 1U & 0x29) + 0x1d;
        }
    }
    rec = *(int *)(param_1 + 0x30);
    if (*(char *)(rec + 2) == 6) {
        d2_SetForm(win, 3);
        io_PrintText("_C_Z_%s", rec + 0x30);
        i = 0;
        x = 0x1c;
        y = 0x1d;
        while ((int)i < (int)*(unsigned char *)(param_1 + 0x34)) {
            d2_WinOpenPic(x, y, *(unsigned char *)(param_1 + 0x3c + i) + 0x75a, U_I((unsigned)&d2_LastWindow));
            i++;
            if (i % 8 == 0)
                x = 0;
            else
                x += 0x3a;
            y = ((i < 8) - 1U & 0x29) + 0x1d;
        }
    } else {
        d2_SetForm(win, 2);
        io_PrintText("_C_Z_%s", rec + 0x30);
        i = 0;
        x = 0x1c;
        y = 0x1d;
        while ((int)i < (int)*(unsigned char *)(param_1 + 0x34)) {
            if (U_I(param_1 + 0x28) == 4)
                pic = 0x761;
            else
                pic = *(unsigned char *)(param_1 + 0x3c + i) + 0x75a;
            d2_WinOpenPic(x, y, pic, U_I((unsigned)&d2_LastWindow));
            i++;
            if (i % 8 == 0)
                x = 0;
            else
                x += 0x3a;
            y = ((i < 8) - 1U & 0x29) + 0x1d;
        }
    }
    d2_SetForm(win, 4);
    d2_RemoveWindowObjects(U_I((unsigned)&d2_LastWindow), 1);
    if ((U_I(param_1 + 8) == 5 && *(char *)(*(int *)(param_1 + 0xc) + 2) == 6) ||
        (U_I(param_1 + 8) == 6 && *(char *)(*(int *)(param_1 + 0x30) + 2) == 6))
        ((int (*)())io_PrintText)();
    else
        ((int (*)())io_PrintText)();
}
