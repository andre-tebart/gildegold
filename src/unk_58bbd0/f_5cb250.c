/* 0x5cb250, the counter-espionage window "privillegien\gegenspionage2" 0x62d588 (672 B, ROUGH: no Ghidra
   function; from the disassembly): param_1 is the request record (dword +0: 3 (the message) or 4 (with
   +0x21c the owner)), param_2 the message record (the AIMSG: +0 the id, +2 the type 6 / 7, +4 the handle,
   +0x194 the amount). The owner kind (owner +0x166) picks the character kinds to find: 0xf -> 0xd (the
   first record, local 0x10) and 0xe (the last, ebp); 0x15 -> 0x13 / 0x14; 0x1b -> 0x19 / 0x1a; other kinds
   return 0x60. The walk over the 0x300 character records 0x770b80 .. 0x7d5f80 (stride 0x21c) stops when
   both kinds were found. Types other than 6 / 7: the amount < 6 or a missing kind returns 0x20, else
   0x5cb1e0 (the message, the two characters) decides 0x10 (sent) / 0x20 (failed). Types 6 / 7: a missing
   kind shows the text 0x5176d0 (the format 0x624204, 0x1dcb) and returns 0; else the window 0x4741f0 /
   0x476f80 / 0x475600 / 0x475f70 with the text 0x515960 (0x1dc9, 6, the two ids) and the two buttons
   0x475b80: the first click 0x520f90 (6) runs 0x5cb1e0 and shows 0x5176d0 (0x1dca on success, the format
   0x624204 / 0x1dcb on failure) through 0x521150 and sets 0x6f22a0; the second click sets it; the pump
   0x5309a0 (this, 0x657c7) and the end 0x4758f0, return 2. ROUGH: the character walk and the text by
   offset. */

int ai_CounterEspionageWindow(int *param_1, int param_2)  /* @original: the callers push (c, m); the draft body reads its own byte offsets of the parameters */
{
    char text[0x200];
    int first;
    int second;
    int kind;
    int k1;
    int k2;
    int n;
    int chr;
    int win;
    int btn1;
    int btn2;
    int t;
    int r;

    first = 0;
    second = 0;
    if (*param_1 == 3) {
        kind = *(unsigned char *)(param_2 + 0x166);
    } else {
        if (*param_1 != 4)
            return 0x60;
        kind = param_1[0x87];
        if (kind == 0)
            return 0x60;
        kind = *(unsigned char *)(kind + 0x166);
    }
    if (kind == 0xf) {
        k1 = 0xd;
        k2 = 0xe;
    } else if (kind == 0x15) {
        k1 = 0x13;
        k2 = 0x14;
    } else if (kind == 0x1b) {
        k1 = 0x19;
        k2 = 0x1a;
    } else {
        return 0x60;
    }
    n = 2;
    for (chr = (unsigned)&ai_Chars; chr < (unsigned)((unsigned)ai_CharsV + 0x65400) && n != 0; chr += 0x21c) {
        if (*(unsigned char *)(chr + 0x166) == k1) {
            first = chr;
            n--;
        } else if (*(unsigned char *)(chr + 0x166) == k2) {
            second = chr;
            n--;
        }
    }
    if (*(char *)(param_2 + 2) != 6 && *(char *)(param_2 + 2) != 7) {
        if (*(int *)(param_2 + 0x194) < 6)
            return 0x20;
        if (n > 0)
            return 0x20;
        return ai_Unk5cb1e0(param_2, second, first) ? 0x10 : 0x20;
    }
    if (n > 0) {
        txt_Format(text, (char *)(unsigned)"$Z%s", 0x1dcb);
        sv_MessageBox(text, 0);
        return 0;
    }
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d588);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    t = io_PrintText(0x1dc9, 6, *(unsigned short *)second, *(unsigned short *)first);
    btn1 = d2_GetWinObj(win, 1, t);
    btn2 = d2_GetWinObj(win, 1, t + 1);
    *(int *)(unsigned)&d2_ClickedId = -1;
    do {
        if (*(int *)(unsigned)&d2_ClickedId != -1) {
            if (*(int *)(unsigned)&ai_Unk5fe710 == btn1) {
                if (sv_CheckMoney(param_2, 6) != 0) {
                    r = ai_Unk5cb1e0(param_2, second, first);
                    if (r != 0)
                        txt_Format(text, (char *)0x1dca);
                    else
                        txt_Format(text, (char *)(unsigned)"$Z%s", 0x1dcb);
                    sv_MessageBox(text, 0);
                    *(int *)(unsigned)&sv_PopRequest = 1;
                }
            } else if (*(int *)(unsigned)&ai_Unk5fe710 == btn2) {
                *(int *)(unsigned)&sv_PopRequest = 1;
            }
        }
        if (*(int *)((unsigned)&dm_L + 0x20) != 0)
            *(int *)(unsigned)&sv_PopRequest = 1;
        if (sv_RunFrame(ai_CounterEspionageWindow, 0x657c7) == 0) {
            d2_CloseView(win);
            return 2;
        }
    } while (1);
}
