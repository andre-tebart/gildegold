/* 0x5a28f0, the church top-5 window "Locations\Kirche\Top5" (620 B, Ghidra-missed start: the tail call of
   0x5a1f70; ROUGH): the window 0x4741f0 / 0x475600 / 0x475f70 / 0x52ea30 (the screen 0x6ed938) / 0x476f80;
   each frame the table 0x50b1e0 (five ids, five values) is read and compared with the drawn copy; on a change
   the header 0x196b and the five rows are redrawn: 0x5be1d0 (the record 0x770b80 + id * 0x21c) and 0x5be720
   (x = half - width - 0x87, y), the text 0x196d (the id) or the string 0x143b3f4 when the record is gone, then
   0x5bd1f0, and the value text 0x196e with 0x5bd1f0 at y + 0x19; the pump 0x5309a0 (this, 0x677c7), the exit
   0x52d7a0 / 0x4758f0. ROUGH: the row records (the 0xe-int stride of the frame) are written as arrays */

int gm_Unk50b1e0(); int ai_AmountRowStatus(); int ai_Unk5be720(); void ai_Unk5bd1f0();
int d2_RemoveWindowObjects();

void ai_Top5Window(void)
{
    int id[5];
    int val[5];
    int oldid[5];
    int oldval[5];
    int rec[5];
    char buf[0x200];
    int win;
    int i;
    int x;
    int y;
    int half;
    int changed = 1;
    short w;

    for (i = 0; i < 5; i++) {
        oldid[i] = 0;
        oldval[i] = 0;
    }
    win = d2_OpenForm(0, 0, "Locations\\Kirche\\Top5");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetPointerSprite(0, 0);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        gm_Unk50b1e0(id, val);
        for (i = 0; i < 5; i++) {
            if (oldid[i] != id[i] || oldval[i] != val[i])
                changed = 1;
            oldid[i] = id[i];
            oldval[i] = val[i];
            rec[i] = id[i] != 0xffff ? (int)((unsigned)&ai_Chars + id[i] * 0x21c) : 0;
        }
        if (changed) {
            d2_SetForm(win, 1);
            d2_RemoveWindowObjects(U_I((unsigned)&d2_LastWindow), 1);
            io_PrintText(0x196b);
            half = (int)*(short *)(U_I((unsigned)&d2_LastWindowPtr) + 8) / 2;
            w = *(short *)(U_I((unsigned)&d2_Pics) + 0x1ebd8);
            x = half - 100;
            y = 0x34;
            for (i = 0; i < 5; i++, y += 0x4c) {
                ai_AmountRowStatus((int *)&rec[i]);
                ai_Unk5be720(half - w - 0x87, y - 8, U_I((unsigned)&d2_LastWindow), (int *)&rec[i]);
                if (rec[i] == 0)
                    strcpy(buf, (char *)U_I((unsigned)&ai_Unk143b3f4));
                else
                    txt_Format(buf, 0x196d, id[i]);
                ai_Unk5bd1f0(buf, x, y, (*(short *)(U_I((unsigned)&d2_LastWindowPtr) + 8) - x) + 0x14,
                             0x53);
                txt_Format(buf, 0x196e, val[i]);
                ai_Unk5bd1f0(buf, x, y + 0x19,
                             (*(short *)(U_I((unsigned)&d2_LastWindowPtr) + 8) - x) + 0x14, 0x53);
            }
            changed = 0;
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (sv_RunFrame(ai_Top5Window, 0x677c7) == 0)
            break;
    }
    sv_CameraFollow();
    d2_CloseView(win);
}
