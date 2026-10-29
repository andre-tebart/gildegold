/* 0x587e00, the item drag-and-drop handler (863 B, ROUGH: the classification and every call are written, the
   branch details around them come from the disassembly): src = 0xf8aac8 (the dragged object), win = 0x62a8cc
   (the drag window). The release: (src == -1 and 0x62a8d0 != -1) or (0x6f266c != 0, 0x14673a8 == 0, win != -1)
   -> 0x4758f0 (win), 0x6f266c = 0, the three 0x62a8cc / 0x62a8d4 / 0x62a8d0 = -1, 0x5f3240 = 1 and
   0x4113f0 (word 0xf8a674, word 0xf8a676). The target 0x6f22e8 (not 0, not 0x6f266c, and 0x14673a8 != 0):
   0x6f22f0's word, 0 -> the "other" record 0x6f22e8 kept; 0x6f266c = 0x6f22e8 and 0x62a8d0 = -1. The widget
   under the pointer (0xf8f080 + U_I (0x5fe710) * 0x2ec + 0x2e8) classifies src: a character (0x770b80 ..
   0x7d5f80, the word 0xffff = none), the goods table (0x889f74 + 0xa900: the byte 0 < 0x58), the buildings
   (0x7e3f30 + 0xca78 = 0x58 records of 0x24d), the 0x7e3f34 table (+0x14f218: the word < 0x2dc), the item table
   0x91aaf0 (+0xb9dc: / 0x41 < 0x2dc) or, without a user record, src itself in the ranges 0xdf .. 0x427 and
   0x427 .. 0x47f. With nothing classified: the tail. Else 0x4758f0 (win) when win != -1 and the drag window
   reset; the item 0x588310 (the kinds 0x17 / 0x20 / 0x25 / 0x27) or 0x588950, the goods 0x588160, the other
   record 0x588bb0, the character 0x588c60 -> win = the new drag window 0x62a8cc = the result; 0x62a8d0 = src,
   0x62a8d4 = 0xfb1024 + win * 0x2ac, 0x5f3240 = 0 when 0x14673a8 != 0. win != -1: 0x6ed9d4 = 1, 0x6f2670 = 1,
   0x4741a0 (win); else the drag reset 0x62a8d0 = -1, 0x6f266c = 0 and 0x6ed9d4 / 0x6f2670 cleared */

int ai_Unk588310(); int ai_Unk588950(); int ai_Unk588160(); int ai_Unk588bb0();
int ai_Unk588c60(); void d2_RaiseViewWindows(int view);

void ai_Unk587e00(void)
{
    char *src;
    char *win;
    char *p;
    char *sel;
    char *ch;
    int item;

    src = *(char **)(unsigned)&d2_Unkf8aac8;
    win = *(char **)(unsigned)&ai_Unk62a8cc;
    item = 0;
    sel = 0;
    ch = 0;
    if ((src == (char *)-1 && U_I((unsigned)&full_data_62a8d0) != -1) ||
        (U_I((unsigned)&full_data_6f266c) != 0 && U_I(((unsigned)&dm_L + 0x28)) == 0 && win != (char *)-1)) {
        d2_CloseView(win);
        U_I((unsigned)&full_data_6f266c) = 0;
        U_I((unsigned)&ai_Unk62a8cc) = -1;
        U_I((unsigned)&full_data_62a8d4) = -1;
        U_I((unsigned)&full_data_62a8d0) = -1;
        ch = 0;
        U_I((unsigned)&di_Absolute) = 1;
        di_SetMousePos(*(short *)(unsigned)&d2_MouseX, *(short *)(unsigned)&d2_MouseY);
        src = *(char **)(unsigned)&d2_Unkf8aac8;
        win = *(char **)(unsigned)&ai_Unk62a8cc;
    }
    if (U_I((unsigned)&ai_Unk6f22e8) != 0 && U_I((unsigned)&full_data_6f266c) != U_I((unsigned)&ai_Unk6f22e8) && U_I(((unsigned)&dm_L + 0x28)) != 0) {
        if (U_I((unsigned)&full_data_6f22f0) == 0 || *(unsigned short *)U_I((unsigned)&full_data_6f22f0) == 0)
            sel = (char *)U_I((unsigned)&ai_Unk6f22e8);
        U_I((unsigned)&full_data_6f266c) = U_I((unsigned)&ai_Unk6f22e8);
        U_I((unsigned)&full_data_62a8d0) = -1;
    }
    if (src != (char *)-1 && (char *)U_I((unsigned)&full_data_62a8d0) != src) {
        p = *(char **)(U_I((unsigned)&d2_Objs) + U_I((unsigned)&ai_Unk5fe710) * 0x2ec + 0x2e8);
        if (p == 0) {
            if (src >= (char *)0xdf && src < (char *)0x427)
                item = (int)src - 0xdf;
            else if (src >= (char *)0x427 && src < (char *)0x47f)
                item = (int)src - 0x427;
        } else if (p >= (char *)(unsigned)&ai_Chars && p < (char *)(unsigned)&gm_PlayerRecords) {
            ch = (*(unsigned short *)p != 0xffff) ? p : 0;
        } else if (p >= (char *)U_I((unsigned)&ai_Blds) && p < (char *)U_I((unsigned)&ai_Blds) + 0xa900) {
            item = *(unsigned char *)p;
            if (item >= 0x58)
                item = 0;
        } else if (p >= (char *)U_I((unsigned)&ai_BTypes) && p < (char *)U_I((unsigned)&ai_BTypes) + 0xca78) {
            item = (int)(p - (char *)U_I((unsigned)&ai_BTypes)) / 0x24d;
        } else if (p >= (char *)U_I((unsigned)&ai_Objects) && p < (char *)U_I((unsigned)&ai_Objects) + 0x14f218) {
            item = *(unsigned short *)p;
            if (item >= 0x2dc)
                item = 0;
        } else if (p >= (char *)U_I((unsigned)&ai_Goods) && p < (char *)U_I((unsigned)&ai_Goods) + 0xb9dc) {
            item = (int)(p - (char *)U_I((unsigned)&ai_Goods)) / 0x41;
        }
        U_I((unsigned)&full_data_6f266c) = 0;
    }
    if (item == 0 && sel == 0 && ch == 0)
        goto tail;
    if (win != (char *)-1) {
        d2_CloseView(win);
        src = *(char **)(unsigned)&d2_Unkf8aac8;
        win = 0;
        U_I((unsigned)&ai_Unk62a8cc) = -1;
        U_I((unsigned)&full_data_62a8d4) = -1;
    }
    if (item != 0) {
        if (*(unsigned char *)(U_I((unsigned)&ai_Goods) + item * 0x41) == 0x20 ||
            *(unsigned char *)(U_I((unsigned)&ai_Goods) + item * 0x41) == 0x17 ||
            *(unsigned char *)(U_I((unsigned)&ai_Goods) + item * 0x41) == 0x25 ||
            *(unsigned char *)(U_I((unsigned)&ai_Goods) + item * 0x41) == 0x27)
            win = (char *)ai_Unk588310(item);
        else
            win = (char *)ai_Unk588950(item);
    } else if (sel != 0) {
        win = (char *)ai_Unk588bb0(sel);
    } else if (ch != 0) {
        win = (char *)ai_Unk588c60(ch);
    }
    src = *(char **)(unsigned)&d2_Unkf8aac8;
    U_I((unsigned)&ai_Unk62a8cc) = (int)win;
    if (win != (char *)-1) {
        U_I((unsigned)&full_data_62a8d0) = (int)src;
        U_I((unsigned)&full_data_62a8d4) = *(int *)(((unsigned)&d2_Views + 0x4) + (int)win * 0x2ac);
        if (U_I(((unsigned)&dm_L + 0x28)) != 0)
            U_I((unsigned)&di_Absolute) = 0;
        U_I((unsigned)&d2_CutsceneActive) = 1;
        U_I((unsigned)&full_data_6f2670) = 1;
        d2_RaiseViewWindows(win);
        return;
    }
    U_I((unsigned)&full_data_62a8d0) = -1;
    U_I((unsigned)&full_data_6f266c) = 0;
tail:
    if (U_I((unsigned)&full_data_6f2670) != 0) {
        U_I((unsigned)&d2_CutsceneActive) = 0;
        U_I((unsigned)&full_data_6f2670) = 0;
    }
}
