/* 0x5c2490, the law entry controls (464 B): the checkbox 0x515960 (0x194f, 0, param_6, (flags & 1) + 0x1951)
   and its 0x475cb0 (the flag), then (kind != 5) the three options (flags >> 1 / >> 2 / >> 3) into param_4[1..3],
   the counter 0x515960 (0x1953, param_6, 0x6f1bc5) into *param_2 with 0x475cb0 (*param_3, 1000 .. 9000) and,
   when kind != 5 and the last argument is set, the slider 0x46a960 (0x1002) into the 8th argument with its
   0x50c550 percent; otherwise the 8th argument is -1 */

void ai_Unk5c2490(int param_1, int *param_2, int *param_3, int *param_4, unsigned char *param_5, int param_6,
                  unsigned int param_7, int *param_8, int param_9)
{
    int t;
    unsigned int u;
    char *kind;
    int v;

    d2_SetForm(param_1, 1);
    io_PrintText((unsigned)"$C");
    io_PrintText(0x1949);
    d2_SetForm(param_1, 2);
    io_PrintText((unsigned)&full_data_624cf8);
    t = io_PrintText(0x194f, 0, param_6, (param_7 & 1) + 0x1951);
    t = d2_GetWinObj(param_1, 2, t);
    *param_4 = t;
    d2_SetObjValue(t, param_7 & 1);
    param_4[1] = -1;
    param_4[2] = -1;
    param_4[3] = -1;
    kind = (char *)(U_I((unsigned)&ai_BTypes) + *param_5 * 0x24d);
    if (*kind != 5) {
        u = (param_7 >> 1) & 1;
        t = io_PrintText(0x1954, 0, param_6, u + 0x1955);
        t = d2_GetWinObj(param_1, 2, t);
        param_4[1] = t;
        d2_SetObjValue(t, u);
        u = (param_7 >> 2) & 1;
        t = io_PrintText(0x1957, 0, param_6, u + 0x1958);
        t = d2_GetWinObj(param_1, 2, t);
        param_4[2] = t;
        d2_SetObjValue(t, u);
        u = (param_7 >> 3) & 1;
        t = io_PrintText(0x195a, 0, param_6, u + 0x195b);
        t = d2_GetWinObj(param_1, 2, t);
        param_4[3] = t;
        d2_SetObjValue(t, u);
    }
    t = io_PrintText(0x1953, param_6, *(unsigned char *)(unsigned)&ai_OwnPlayer);
    t = d2_GetWinObj(param_1, 2, t);
    *param_2 = t;
    d2_SetObjValue(t, 1000, 9000, *param_3);
    if (*kind != 5 && param_9 != 0) {
        io_PrintText(0x42c2);
        t = d2_WinOpenSlider(100, 300, 200, 0x32, 0x96, 0x7b, 0x1002, U_I((unsigned)&d2_LastWindow));
        *param_8 = t;
        v = 0; /* ROUGH: the __ftol of the lost float call (the slider percent) */
        d2_SetObjValue(t, 0x32, 0x96, v);
        return;
    }
    *param_8 = -1;
}
