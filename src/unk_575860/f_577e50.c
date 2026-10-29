/* 0x577e50, the tutorial chapter form (632 B): r = S+0x18; -4 without it; the step S+0x40 against the step
   count r+0x50 (unsigned): done -> the callback r+0x54, the form S+0x48 released (0x4758f0), S+0x4c = -1,
   S+0x44 = 4, 0. Else the step record (r+0x58, 0x10 B each: +4 the chapter kind 0 .. 3, +8 the text, +0xc a
   window value): when the chapter S+0x44 is unchanged the form's controls 1 / 2 are cleared (0x475f70 +
   the empty text 0x602568); else the old form is released, the kind re-read (-4 when >= 4) and the new form
   created as 0x4741f0 (0, 0, kind 0 "tutorial\left_form" 0x629368, 1 "tutorial\right_form" 0x629354,
   2 "tutorial\top_form" 0x629340, 3 "tutorial\bottom_form" 0x629328), 0x475850 (id, 3), the chapter S+0x44
   set. Then control 1 = the step text, control 2 = 0x515960 (0x624204 "$Z%s", 0x7d) into 0x475b80 (id, 2,
   text) into S+0x4c, the window S+0x14 = 0x4c2760 (0xfffffff9, 0, the step's +0xc, -1) when that is set, the
   callback r+0x54, 0 */

int ai_Unk577e50(void)
{
    char *st;
    char *r;
    int step;
    int kind;
    int cb;
    int v;

    st = S;
    r = *(char **)(st + 0x18);
    if (!r)
        return -4;
    step = *(int *)(st + 0x40);
    if (step >= (unsigned int)*(int *)(r + 0x50)) {
        cb = *(int *)(r + 0x54);
        if (cb)
            ((void (*)())cb)();
        if (U_I(S + 0x48) != -1) {
            d2_CloseView(U_I(S + 0x48));
            U_I(S + 0x48) = -1;
        }
        U_I(S + 0x4c) = -1;
        U_I(S + 0x44) = 4;
        return 0;
    }
    kind = *(int *)(*(int *)(*(char **)(S + 0x18) + 0x58) + U_I(S + 0x40) * 0x10 + 4);
    if (U_I(S + 0x44) == kind) {
        d2_SetForm(U_I(S + 0x48), 1);
        io_PrintText((char *)(unsigned)"$C");
        d2_SetForm(U_I(S + 0x48), 2);
        io_PrintText((char *)(unsigned)"$C");
    } else {
        if (U_I(S + 0x48) != -1) {
            d2_CloseView(U_I(S + 0x48));
            U_I(S + 0x48) = -1;
        }
        U_I(S + 0x4c) = -1;
        U_I(S + 0x44) = 4;
        kind = *(int *)(*(int *)(*(char **)(S + 0x18) + 0x58) + U_I(S + 0x40) * 0x10 + 4);
        if (kind >= 4)
            return -4;
        switch (kind) {
        case 0: U_I(S + 0x48) = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_629368); break;
        case 1: U_I(S + 0x48) = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_629354); break;
        case 2: U_I(S + 0x48) = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_629340); break;
        case 3: U_I(S + 0x48) = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_629328); break;
        }
        d2_PlaceMainWindow(U_I(S + 0x48), 3);
        U_I(S + 0x44) = *(int *)(*(int *)(*(char **)(S + 0x18) + 0x58) + U_I(S + 0x40) * 0x10 + 4);
    }
    d2_SetForm(U_I(S + 0x48), 1);
    io_PrintText(*(int *)(*(int *)(*(char **)(S + 0x18) + 0x58) + U_I(S + 0x40) * 0x10 + 8));
    d2_SetForm(U_I(S + 0x48), 2);
    U_I(S + 0x4c) = d2_GetWinObj(U_I(S + 0x48), 2, io_PrintText((char *)(unsigned)"$Z%s", 0x7d));
    v = *(int *)(*(int *)(*(char **)(S + 0x18) + 0x58) + U_I(S + 0x40) * 0x10 + 0xc);
    if (v) {
        if (U_I(S + 0x14)) {
            if (sb_IsPlaying(U_I(S + 0x14)))
                sb_Stop(U_I(S + 0x14), 0);
            U_I(S + 0x14) = 0;
        }
        U_I(S + 0x14) = snd_PlayVoice(0xfffffff9, 0, v, -1);
    }
    cb = *(int *)(*(char **)(S + 0x18) + 0x54);
    if (cb)
        ((void (*)())cb)();
    return 0;
}
