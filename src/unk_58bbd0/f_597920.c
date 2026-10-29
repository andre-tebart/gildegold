/* 0x597920, draws the dice of a dice record without the extra cell (230 B): as 0x597810, but the random 0x75b
   cell only in the record branch */

void ai_Unk597920(int param_1, int *param_2)
{
    unsigned int i;
    int x;
    int y;

    d2_RemoveWindowObjects(U_I((unsigned)&d2_LastWindow), 1);
    io_PrintText("_C_Z_%s", param_1);
    i = 0;
    x = 0x1c;
    y = 0x1d;
    if (*(char *)(*param_2 + 2) == 6) {
        if ((char)param_2[1] != 0) {
            do {
                d2_WinOpenPic(x, y, *(unsigned char *)((int)param_2 + i + 0xc) + 0x75a,
                             U_I((unsigned)&d2_LastWindow));
                i++;
                if (i % 8 == 0)
                    x = 0;
                else
                    x += 0x3a;
                y = ((i < 8) - 1U & 0x29) + 0x1d;
            } while ((int)i < (int)(unsigned char)param_2[1]);
        }
    } else if ((char)param_2[1] != 0) {
        do {
            d2_WinOpenPic(x, y, 0x761, U_I((unsigned)&d2_LastWindow));
            i++;
            if (i % 8 == 0)
                x = 0;
            else
                x += 0x3a;
            y = ((i < 8) - 1U & 0x29) + 0x1d;
        } while ((int)i < (int)(unsigned char)param_2[1]);
    }
}
