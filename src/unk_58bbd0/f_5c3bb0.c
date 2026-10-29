/* 0x5c3bb0, the law selection window "Gesetze\Auswahl2" 0x62d2ec (1360 B, ROUGH): the character ids of
   param_1 (+0x25 / +0x27, then the characters of the kinds 1 / 2 whose +0xb8 owner is param_1, and of the
   kinds 4 .. 9 / 0xb .. 0xe / 0x10 / 0x12 .. 0x16 whose +0xb6 owner is param_1), up to 0x10; the window
   0x4741f0 (0, 0, 0x62d2ec), 0x476f80, 0x475600, the caption param_3 when nonzero (0x475f70 (win, 1),
   0x515960 (param_3)) and the 0x72270 frame when there are more than two ids; the rows (0x4731d0 (0x96, y,
   0x5db) the icon, 0x50c550 the relation (__ftol), 0x4731d0 (0x98, y + 1, 0x770d0c + id * 0x21c) the
   picture, 0x5176d0 (text, 0x62a564, the id), 0x473650 (the x / y of the icon, the text, screen),
   0x4760b0 (ctrl, 0x56), 0x46a960 (0x8c, y + 4, 0x40, 0, 100, 0x75, 0x442) the slider, 0x475cb0,
   0x475e40 (slider, 0) and 0x46cda0); the filter 0x5c33c0 (0x6f25b8, the id, param_2, param_1) hides the
   row; a click runs the callback param_4 (the character record, param_1) or sets 0x6f22a0; the radio
   0x46c490 / 0x46c560 / 0x46c600, the pump 0x5309a0 (this, 0x677c7) and 0x4758f0 (win). ROUGH: the row
   x / y arithmetic of 0x46cda0 and the exact guards of the 0x72270 block are from Ghidra */

void *ai_Unk5c3bb0(unsigned char *param_1, int *param_2, int param_3, void (*param_4)())
{
    int win;
    int ids[16];
    int ctrls[16];
    char text[32];
    void *result;
    int radio;
    int n;
    int i;
    int y;
    int draw;
    unsigned short a;
    unsigned short b;
    unsigned char kind;
    unsigned int id;
    int v;

    result = 0;
    radio = -1;
    draw = 1;
    if (param_1 == 0)
        return 0;
    a = *(unsigned short *)(param_1 + 0x25);
    n = 0;
    if (a != 0xffff) {
        ids[0] = a;
        n = 1;
    }
    b = *(unsigned short *)(param_1 + 0x27);
    if (b != 0xffff && a != b) {
        ids[n] = b;
        n++;
    }
    kind = *(unsigned char *)(U_I((unsigned)&ai_BTypes) + *param_1 * 0x24d);
    if (kind == 1 || kind == 2) {
        for (i = 0; i < 0x300 && n < 0x10; i++) {
            if (*(unsigned char **)((unsigned)&ai_Chars + i * 0x21c + 0x170) == param_1 &&
                *(unsigned short *)((unsigned)&ai_Chars + i * 0x21c) != a &&
                *(unsigned short *)((unsigned)&ai_Chars + i * 0x21c) != b)
                ids[n++] = *(unsigned short *)((unsigned)&ai_Chars + i * 0x21c);
        }
    }
    if ((kind > 3 && kind < 10) || (kind > 10 && kind < 0xf) || kind == 0x10 || (kind > 0x11 && kind < 0x17)) {
        for (i = 0; i < 0x300 && n < 0x10; i++) {
            if (*(unsigned char **)((unsigned)&ai_Chars + i * 0x21c + 0x16c) == param_1 &&
                *(unsigned short *)((unsigned)&ai_Chars + i * 0x21c) != a &&
                *(unsigned short *)((unsigned)&ai_Chars + i * 0x21c) != b)
                ids[n++] = *(unsigned short *)((unsigned)&ai_Chars + i * 0x21c);
        }
    }
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d2ec);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    if (param_3 != 0) {
        d2_SetForm(win, 1);
        io_PrintText(param_3);
    }
    if (n > 2) {
        d2_SetForm(win, 2);
        d2_SetForm(win, 0);
        d2_CreateScrollButtons(-1, -1, U_I((unsigned)&d2_LastWindow), U_I((unsigned)&d2_LastWindow), 0x767);
    }
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (draw) {
            d2_SetForm(win, 2);
            d2_RemoveWindowObjects(U_I((unsigned)&d2_LastWindow), 1);
            y = 0x1e;
            for (i = 0; i < n; i++) {
                v = 1;
                if (param_2 != 0 && param_2[1] != 0) {
                    v = ai_Unk5c33c0(U_W((unsigned)&ai_PlayerId), ids[i], param_2, param_1);
                    if (*param_2 == 0 && v == 0)
                        continue;   /* ROUGH: the hidden row builds no controls */
                }
                ctrls[i] = d2_WinOpenPic(0x96, y, 0x5db, U_I((unsigned)&d2_LastWindow));
                if (v != 0) {
                    if (radio == -1)
                        radio = d2_CreateRadioGroup(1);
                    else
                        d2_RadioAdd(radio, ctrls[i]);
                }
                id = ids[i];
                d2_WinOpenPic(0x98, y + 1, U_I(((unsigned)&ai_Selected + 0x4) + id * 0x87), U_I((unsigned)&d2_LastWindow));
                txt_Format(text, (char *)(unsigned)&full_data_62a564, id);
                /* ROUGH: the text control 0x473650 (x, y, text, screen), 0x4760b0 (ctrl, 0x56) and the
                   slider 0x46a960 (0x8c, y + 4, 0x40, 0, 100, 0x75, 0x442) with 0x475cb0 / 0x475e40 /
                   0x46cda0 */
                y += 0x5a;
            }
            draw = 0;
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            for (i = 0; i < n; i++) {
                if (U_I((unsigned)&ai_Unk5fe710) == ctrls[i]) {
                    result = (void *)((unsigned)&ai_Chars + ids[i] * 0x21c);
                    if (param_4 == 0) {
                        U_I((unsigned)&sv_PopRequest) = 1;
                    } else {
                        param_4(result, param_1);
                        draw = 1;
                        if (radio != -1)
                            d2_RadioClear(radio);
                        radio = -1;
                    }
                    break;
                }
            }
        }
        if (sv_RunFrame(ai_Unk5c3bb0, 0x677c7) == 0)
            break;
    }
    if (radio != -1)
        d2_RadioClear(radio);
    d2_CloseView(win);
    return result;
}
