/* 0x5aa0d0, a window of the Domizil box (232 B): the picture control 0x5db (0x4731d0) visible, the two texts
   0x1440ac8 and 0x631090 (0x473650 at x - 0x36, y - 0x11 / y + 0x44) at +5 / +6; the record: x at +0, y at +1,
   the screen 0x6ed958 at +0x16, the ids -1 at 0xa .. 0x15 and 0 at +2 / +9 */

void ai_Unk5aa0d0(int param_1, int param_2, int *param_3)
{
    int id;
    int i;

    id = d2_WinOpenPic(param_1, param_2, 0x5db, U_I((unsigned)&d2_LastWindow));
    param_3[3] = id;
    U_I((unsigned)&d2_Objs + 0x4c + id * 0x2ec) = 1;
    id = d2_CreateWinText(param_1 - 0x36, param_2 - 0x11, U_I((unsigned)&ai_Unk1440ac8), U_I((unsigned)&d2_LastWindow));
    param_3[5] = id;
    U_W((unsigned)&d2_Objs + 0x14 + id * 0x2ec) = 0xa0;
    U_I((unsigned)&d2_Objs + 0x5c + param_3[5] * 0x2ec) = 1;
    id = d2_CreateWinText(param_1 - 0x36, param_2 + 0x44, (unsigned)&ch_EmptyName, U_I((unsigned)&d2_LastWindow));
    param_3[6] = id;
    U_W((unsigned)&d2_Objs + 0x14 + id * 0x2ec) = 0xa0;
    U_I((unsigned)&d2_Objs + 0x5c + param_3[6] * 0x2ec) = 1;
    param_3[7] = -1;
    param_3[4] = -1;
    param_3[2] = 0;
    param_3[9] = 0;
    *param_3 = param_1;
    param_3[0x16] = U_I((unsigned)&d2_CurrentView);
    param_3[1] = param_2;
    for (i = 0; i < 6; i++) {
        param_3[0xa + i] = -1;
        param_3[0x10 + i] = -1;
    }
}
