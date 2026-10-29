/* 0x5996b0, the dark-corner buying screen of another table (1008 B, ROUGH SKELETON from the Ghidra pseudocode):
   as 0x5992c0 with the three offers param_2 + 0x1c (the record 0x4f4d30), the rows and the texts 0x18a3 / 0x18a5
   / 0x18a6 (0x5be1d0 / 0x5be3b0 / 0x5bd1f0); the click on a row buys (0x521040, the "buy_dunkle_ecke"
   message 0x4c4540 / 0x4c5d40 / 0x4c4ed0 / 0x4c4830 / 0x4c4580); without an offer the text
   0x62b83c ("Diese Information steht nicht ...") goes to 0x521150; the pump 0x5309a0 (0x5992c0, 0x677c7); the
   exit 0x4758f0 */

void ai_Unk5996b0(int param_1, int param_2)
{
    int slots[3];
    int vals[3];
    int items[0x2a];
    int win;
    int changed;
    int i;
    int r;
    int x;
    int y;
    int w;
    int offer;
    int buf;
    int *p;
    int a[3];
    char text[0x200];

    memset(items, 0, 0xa8);
    slots[0] = -1;
    slots[1] = -1;
    slots[2] = -1;
    vals[0] = 0;
    vals[1] = 0;
    vals[2] = 0;
    win = d2_OpenForm(0, 0, "locations\\Wirtshaus_DUNKLEECKE");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetPointerSprite(0, 0);
    memset(items, 0, 0xa8);
    changed = 1;
    while (sv_RunFrame(ai_Unk5992c0, 0x677c7) != 0) {
        for (i = 0; i < 3; i++) {
            r = unk_Unk4f4d30(*(int *)(param_2 + 0x1c + i * 4),
                             (void *)((unsigned)&ai_Chars + *(unsigned short *)(param_1 + 0x27) * 0x21c),
                             &vals[i], &x, &y);
            if (r == 0) {
                if (slots[i] != -1) {
                    changed = 1;
                    items[i * 0xe] = 0;
                    slots[i] = -1;
                    vals[i] = 0;
                }
            } else {
                if (slots[i] != *(int *)(param_2 + 0x1c + i * 4))
                    changed = 1;
                slots[i] = *(int *)(param_2 + 0x1c + i * 4);
                items[i * 0xe] = (unsigned)&ai_Chars + (x & 0xffff) * 0x21c;
            }
        }
        if (changed) {
            d2_SetForm(win, 1);
            d2_RemoveWindowObjects(U_I((unsigned)&d2_LastWindow), 1);
            io_PrintText(0x18a3);
            y = 0x46;
            w = (int)*(short *)((unsigned)&d2_LastWindowPtr + 8) / 2;
            x = (w - *(short *)((unsigned)&d2_Pics + 0x1ebd8)) - 0x28;
            for (i = 0; i < 3; i++) {
                p = items + i * 0xe;
                p[4] = x;
                p[5] = y - 0x1e;
                p[0xc] = (*p != 0);
                ai_AmountRowStatus(p);
                ai_AmountRow(p[4], p[5], U_I((unsigned)&d2_LastWindow), 0x54, p, 1);
                if (slots[i] == -1)
                    txt_Format(text, 0x18a6);
                else
                    txt_Format(text, 0x18a5, vals[i] * 5 + 0x13fd, vals[i]);
                ai_Unk5bd1f0(text, w, y, 0xf0, 0x54);
                y += 0x69;
            }
            changed = 0;
        }
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            offer = -1;
            for (i = 0; i < 3; i++) {
                if (U_I((unsigned)&ai_Unk5fe710) == items[i * 0xe + 4]) {
                    offer = i;
                    break;
                }
            }
            if (offer >= 0) {
                for (i = 0; i < 3; i++) {
                    if (*(int *)(param_2 + 0x1c + i * 4) == slots[offer]) {
                        /* ROUGH: the buy -- 0x521040 (the character, the value, 0x6f1bc5), the
                           "buy_dunkle_ecke" message (0x4c4540 / 0x4c5d40 / 0x4c4ed0 / 0x4c4830 /
                           0x4c4580) with the goods, the seller and the value */
                        break;
                    }
                }
            } else {
                sv_MessageBox("_Z_NDiese_Information_steht_nicht_zur_Verfuegung", 0);
            }
        } else if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        }
    }
    d2_CloseView(win);
}
