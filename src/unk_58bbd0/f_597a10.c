/* 0x597a10, the dice animation driver (1085 B, ROUGH SKELETON from the Ghidra pseudocode): param_1 the game
   record, param_2 the dice record, param_3 the mode 1 .. 4, param_4 the window, param_5 the frame, param_6 the
   wait in ms, param_7 the fast flag; the texts 0x5176d0 (0x1883 / 0x1884 / 0x1885 / 0x1886, *param_2 + 0x30,
   the record's +4); the pump 0x5309a0 (0, 0x777c7) while the clock 0x6333cc has not advanced by param_6 and the
   player's byte 0x770b88 is set; the dice drawn by 0x597780 / 0x597810 (growing steps of the frame) or
   0x597920 */

void ai_Unk597a10(int param_1, int *param_2, int mode, int win, int frame, int wait, int fast)
{
    char text[0x100];
    int step;
    unsigned int t0;
    unsigned int end;

    step = 2;
    if (fast == 0) {
        if (mode == 1) {
            txt_Format(text, 0x1883, *param_2 + 0x30);
            end = U_I((unsigned)&ai_Unk6333cc) + wait;
            t0 = U_I((unsigned)&ai_Unk6333cc);
            while (t0 < end) {
                sv_RunFrame(0, 0x777c7);
                if (*(char *)(((unsigned)&ai_CharInts + 0x4) + U_W((unsigned)&ai_PlayerId) * 0x21c) == 0)
                    return;
                if (t0 + step < U_I((unsigned)&ai_Unk6333cc)) {
                    d2_SetForm(win, frame);
                    ai_Unk597780(text);
                    step = step + step / 2;
                    t0 = U_I((unsigned)&ai_Unk6333cc);
                }
            }
        } else if (mode == 2) {
            txt_Format(text, 0x1884, *param_2 + 0x30, U_I(param_1 + 4));
            end = U_I((unsigned)&ai_Unk6333cc) + wait;
            t0 = U_I((unsigned)&ai_Unk6333cc);
            while (t0 < end) {
                sv_RunFrame(0, 0x777c7);
                if (*(char *)(((unsigned)&ai_CharInts + 0x4) + U_W((unsigned)&ai_PlayerId) * 0x21c) == 0)
                    return;
                if (t0 + step < U_I((unsigned)&ai_Unk6333cc)) {
                    d2_SetForm(win, frame);
                    ai_Unk597810(text, param_2);
                    step = step + step / 2;
                    t0 = U_I((unsigned)&ai_Unk6333cc);
                }
            }
        } else if (mode == 3) {
            txt_Format(text, 0x1885, *param_2 + 0x30, U_I(param_1 + 4));
            t0 = U_I((unsigned)&ai_Unk6333cc);
            d2_SetForm(win, frame);
            ai_Unk597920(text, param_2);
            while (t0 + wait > U_I((unsigned)&ai_Unk6333cc)) {
                if (*(char *)(((unsigned)&ai_CharInts + 0x4) + U_W((unsigned)&ai_PlayerId) * 0x21c) == 0)
                    return;
                sv_RunFrame(0, 0x777c7);
            }
        } else if (mode == 4) {
            txt_Format(text, 0x1886, *param_2 + 0x30, U_I(param_1 + 4));
            t0 = U_I((unsigned)&ai_Unk6333cc);
            d2_SetForm(win, frame);
            ai_Unk597920(text, param_2);
            while (t0 + wait > U_I((unsigned)&ai_Unk6333cc)) {
                if (*(char *)(((unsigned)&ai_CharInts + 0x4) + U_W((unsigned)&ai_PlayerId) * 0x21c) == 0)
                    return;
                sv_RunFrame(0, 0x777c7);
            }
        }
        return;
    }
    if (mode == 1)
        txt_Format(text, 0x1883, *param_2 + 0x30);
    else if (mode == 2)
        txt_Format(text, 0x1884, *param_2 + 0x30, U_I(param_1 + 4));
    else if (mode == 3)
        txt_Format(text, 0x1885, *param_2 + 0x30, U_I(param_1 + 4));
    else if (mode == 4)
        txt_Format(text, 0x1886, *param_2 + 0x30, U_I(param_1 + 4));
    t0 = U_I((unsigned)&ai_Unk6333cc);
    d2_SetForm(win, frame);
    ai_Unk597920(text, param_2);
    while (U_I((unsigned)&ai_Unk6333cc) < t0 + wait &&
           *(char *)(((unsigned)&ai_CharInts + 0x4) + U_W((unsigned)&ai_PlayerId) * 0x21c) != 0)
        sv_RunFrame(0, 0x777c7);
}
