/* 0x59c450, draws the rows of the money lender's exchange window (608 B, ROUGH): for each 7-int record with a
   good (+8) a button (0x469ee0 or 0x4731d0 at x = (0x6ed980 + 8 - 0x30) / 2, y = i * 0x4c), the text
   (0x473650) with the good name 0x143c860 via 0x5176d0, the city name "%s" 0x5f8318 (0x7e3f40 + the record
   byte +0xc * 0x2f4) and the good text; the controls get 0x54 / 0xbe, the flag +0x5c and +0x44 */

void ai_Unk59c450(int param_1, int param_2, int param_3, int param_4)
{
    char text[0x80];
    int *rec;
    int i;
    int y;
    int v;

    i = 0;
    y = 0;
    rec = (int *)(param_1 + 4);
    for (;;) {
        if (rec[2] == 0)
            return;
        d2_SetForm(param_2, param_3);
        if (param_4 == 0) {
            v = d2_WinOpenPic((*(short *)((unsigned)&d2_LastWindowPtr + 8) - 0x30) >> 1, i * 0x4c, rec[2] + 0xdf,
                             U_I((unsigned)&d2_LastWindow));
            rec[-1] = v;
            U_I(U_I((unsigned)&d2_Objs) + 0x4c + v * 0x2ec) = 1;
        } else {
            v = d2_WinOpenInputIcon((*(short *)((unsigned)&d2_LastWindowPtr + 8) - 0x30) >> 1, y, rec[2] + 0xdf, 0x82,
                             U_I((unsigned)&d2_LastWindow));
            rec[-1] = v;
        }
        U_I(U_I((unsigned)&d2_Objs) + 0x44 + rec[-1] * 0x2ec) = 1;
        v = d2_CreateWinText((*(short *)((unsigned)&d2_LastWindowPtr + 8) - 0xbe) >> 1, i * 0x4c + 0x30, (char *)(unsigned)&ch_EmptyName,
                         U_I((unsigned)&d2_LastWindow));
        *rec = v;
        txt_Format(text, "%S", U_I((unsigned)&ai_Unk143c860 + rec[2] * 8));
        d2_SetObjText(*rec, text);
        d2_SetObjFont(rec[-1], 0x54);
        d2_SetObjFont(*rec, 0x54);
        U_I(U_I((unsigned)&d2_Objs) + 0x5c + *rec * 0x2ec) = 1;
        *(unsigned short *)(U_I((unsigned)&d2_Objs) + 0x14 + *rec * 0x2ec) = 0xbe;
        v = d2_CreateWinText((*(short *)((unsigned)&d2_LastWindowPtr + 8) - 0xbe) >> 1, i * 0x4c + 0x3f, (char *)(unsigned)&ch_EmptyName,
                         U_I((unsigned)&d2_LastWindow));
        rec[1] = v;
        txt_Format(text, "%s", (char *)(unsigned)&ai_PlayerSlots + *(unsigned char *)(rec + 3) * 0x2f4);
        d2_SetObjText(rec[1], text);
        d2_SetObjFont(rec[1], 0x54);
        y += 0x4c;
        U_I(U_I((unsigned)&d2_Objs) + 0x5c + rec[1] * 0x2ec) = 1;
        i++;
        *(unsigned short *)(U_I((unsigned)&d2_Objs) + 0x14 + rec[1] * 0x2ec) = 0xbe;
        rec += 7;
        if (y >= 0x4c0)
            return;
    }
}
