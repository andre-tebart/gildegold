/* 0x59c120, fills one side of the money lender's exchange window (816 B): 0x4ffe90 (param_1, 1, 4, 9) walks the
   offers; each offer's good (word at +0) not already in the 16 records (param_2 + 3) either extends an existing
   record (the list -1 / 0 and the value) or takes the first free record (0x475f70 (param_3, param_4), the
   button 0x469ee0 (x, row * 0x4c, the good + 0xdf, 0x82), the text 0x473650 (0x631090) and 0x4760b0); a click
   on the control 0x5fe710 turns the other records off; the values come from 0x475980 and 0x508640 (0xe / 2)
   and 0x5176d0 "%I %s" (0x62bb30) into 0x4734d0; returns the selected record or -1 */

int ai_Unk59c120(int param_1, int *param_2, int param_3, int param_4, char param_5)
{
    char text[0x20];
    int sel;
    int changed;
    unsigned short *good;
    int i;
    int j;
    int v;
    int r;

    sel = -1;
    changed = 0;
    good = (unsigned short *)gm_Query(param_1, 1, 4, 9);
    while (good != 0) {
        for (i = 0; i < 16; i++) {
            if (param_2[i * 7 + 3] == *good)
                break;
        }
        if (i == 16) {
            for (i = 0; i < 16; i++) {
                if (param_2[i * 7 + 3] == 0) {
                    param_2[i * 7 + 3] = *good;
                    d2_SetForm(param_3, param_4);
                    v = d2_WinOpenInputIcon((*(short *)((unsigned)&d2_LastWindowPtr + 8) - 0x30) >> 1, i * 0x4c, *good + 0xdf,
                                     0x82, U_I((unsigned)&d2_LastWindow));
                    param_2[i * 7] = v;
                    U_I(U_I((unsigned)&d2_Objs) + 0x44 + v * 0x2ec) = 1;
                    v = d2_CreateWinText((*(short *)((unsigned)&d2_LastWindowPtr + 8) - 0xbe) >> 1, i * 0x4c + 0x2e,
                                     (char *)(unsigned)&ch_EmptyName, U_I((unsigned)&d2_LastWindow));
                    param_2[i * 7 + 1] = v;
                    d2_SetObjFont(param_2[i * 7], 0x54);
                    d2_SetObjFont(param_2[i * 7 + 1], 0x54);
                    U_I(U_I((unsigned)&d2_Objs) + 0x5c + param_2[i * 7 + 1] * 0x2ec) = 1;
                    *(unsigned short *)(U_I((unsigned)&d2_Objs) + 0x14 + param_2[i * 7 + 1] * 0x2ec) = 0xbe;
                    if (param_5 != 0)
                        U_I(U_I((unsigned)&d2_Objs) + 0x3c + param_2[i * 7] * 0x2ec) = 1;
                    break;
                }
            }
        }
        changed = 1;
        good = (unsigned short *)gm_QueryNext();
    }
    if (param_5 != 0) {
        for (i = 0; i < 16; i++)
            if (param_2[i * 7] != -1)
                d2_SetObjEnabled(param_2[i * 7], 1);
    }
    for (i = 0; i < 16; i++) {
        if (param_2[i * 7] != -1) {
            v = d2_GetObjValue(param_2[i * 7]);
            param_2[i * 7 + 4] = v;
            r = gm_Query(param_1, 1, 0, param_2[i * 7 + 2]);
            if (r == 0 || gm_Unk508640(U_I(r + 0xe), param_2[i * 7 + 2]) == 0) {
                d2_RemoveObj(param_2[i * 7 + 1]);
                param_2[i * 7 + 1] = -1;
                d2_RemoveObj(param_2[i * 7]);
                param_2[i * 7] = -1;
                param_2[i * 7 + 5] = 0;
                param_2[i * 7 + 2] = 0;
                changed = 1;
            } else {
                r = gm_Unk508640(U_I(r + 0xe), param_2[i * 7 + 2]);
                d2_SetObjValue(param_2[i * 7], 0, r, v);
                txt_Format(text, "%I %s", r, U_I((unsigned)&ai_Unk143c860 + param_2[i * 7 + 2] * 8));
                d2_SetObjText(param_2[i * 7 + 1], text);
                *(unsigned short *)(U_I((unsigned)&d2_Objs) + 0x14 + param_2[i * 7] * 0x2ec) = 0xbe;
                param_2[i * 7 + 5] = r;
            }
            if (param_5 != 0 && v != 0) {
                d2_AddPointerItem(param_2[i * 7 + 2] + 0xdf, v);
                v = 0;
                j = 0;
                while (j < 16) {
                    if (param_2[j * 7] != -1 && i != v)
                        d2_SetObjEnabled(param_2[j * 7], 0);
                    v = j + 1;
                    j = j + 1;
                    sel = i;
                }
            }
        }
    }
    if (changed) {
        d2_SetForm(param_3, param_4);
        ai_Unk59be90(param_2);
    }
    return sel;
}
