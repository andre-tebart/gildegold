/* 0x5992c0, the dark-corner menu of the player's own table (1008 B, ROUGH SKELETON from the Ghidra pseudocode):
   the window "locations\Wirtshaus_DUNKLEECKE" (0x4741f0 / 0x475600 / 0x475f70 (0) / 0x52ea30 / 0x476f80); the
   three offers param_3 + 0x1c (0x4f4d30 with the character record 0x770b80 + player * 0x21c, the value local,
   the goods and the slot), each drawn as a record of 0xe ints by 0x5be1d0 / 0x5be3b0 with the text 0x5176d0
   (0x18a5 the goods * 5 + 0x13fd / 0x18a6 empty) and 0x5bd1f0; the buy button 0x18a7 / 0x18a8; the click builds
   the message (type 0x74, the character, the building, the clock 0x770ae0 ..) through 0x4c4b70 and sets
   0x6f22a0; the pump 0x5309a0 (this, 0x677c7); the exit 0x4758f0 */

int unk_Unk4f4d30(); void ai_Unk5bd1f0(); void ai_Unk4c5d40(void *p); void cm_SendProofExecuted(int a, int b);

void ai_Unk5992c0(int param_1, int param_2, int param_3)
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
    int ctl;
    int b;
    int *p;
    char text[0x200];
    unsigned char msg[0xf8];

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
    ctl = -1;
    while (sv_RunFrame(ai_Unk5992c0, 0x677c7) != 0) {
        for (i = 0; i < 3; i++) {
            r = unk_Unk4f4d30(*(int *)(param_3 + 0x1c + i * 4),
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
                if (slots[i] != *(int *)(param_3 + 0x1c + i * 4))
                    changed = 1;
                slots[i] = *(int *)(param_3 + 0x1c + i * 4);
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
                ai_Unk5be1d0(p);
                ai_Unk5be3b0(p[4], p[5], U_I((unsigned)&d2_LastWindow), 0x54, p, 1);
                if (slots[i] == -1)
                    txt_Format(text, 0x18a6);
                else
                    txt_Format(text, 0x18a5, vals[i] * 5 + 0x13fd, vals[i]);
                ai_Unk5bd1f0(text, w, y, 0xf0, 0x54);
                y += 0x69;
            }
            b = 0;
            for (r = he_FindEvent(1, 0, 0x74); r != 0; r = he_NextEvent())
                if (*(int *)(r + 0xb0) == U_I(param_1 + 1)) {
                    b = 1;
                    break;
                }
            d2_SetForm(win, 2);
            io_PrintText((char *)(unsigned)"$C");
            ctl = -1;
            if (b)
                io_PrintText(0x18a8);
            else if (*(int *)(param_3 + 0x28) < U_I((unsigned)&ai_Time))
                ctl = d2_GetWinObj(win, 2, io_PrintText(0x18a7));
            changed = 0;
        }
        if (U_I((unsigned)&d2_ClickedId) != -1 && U_I((unsigned)&ai_Unk5fe710) == ctl) {
            memset(msg, 0, 0xf8);
            *(unsigned char *)(msg + 4) = 0x74;
            *(unsigned char *)(msg + 0x36) = 2;
            *(int *)(msg + 8) = U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x87);
            *(int *)(msg + 0xc) = U_I(param_1 + 1);
            *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
            *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
            *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
            *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
            *(int *)(msg + 0x5c) = U_I(param_2 + 2);
            cm_SendLongMessage(msg);
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        }
    }
    d2_CloseView(win);
}
