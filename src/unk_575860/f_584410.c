/* 0x584410, the graveyard confirmation with two buttons (264 B): rec / w the window record (its +0 the window
   handle, +0x18 the form, +0x1c the state object, +0x20 the value passed on, +0x24 the byte passed on). Two
   buttons via 0x52e350 in the form w[6] at height 0, their labels U_I (0x144a620) / U_I (0x144a624); when the
   state object's [0] == 3 the second button is disabled (0x475e40 (the button, 0)); 0xf8a668 = -1; the pump loop
   0x5309a0 (this window, 0x677c7): the escape 0x14673a0 -> 0x6f22a0 = 1; the first button -> 0x475590 (the
   window, 0), 0x583de0 (rec, w[8], w+0x24) == 1 -> 0x6f22a0 = 1, 0x475590 (the window, 1); the second button ->
   the same through 0x584260 (the rent-a-grave window); the window is released by the caller, the pump 0
   returns. Caller: the graveyard handler 0x5871e0 after 0x583b70 (which fills the record w). */

void ai_Unk584410(char *rec, int *w)
{
    int btns[2];

    sv_Unk52e350(w[6], 0, btns, 2, U_I((unsigned)&full_data_144a620), U_I((unsigned)&full_data_144a624));
    if (*(int *)w[7] == 3)
        d2_SetObjEnabled(btns[1], 0);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == btns[0]) {
                d2_SetViewEnabled(w[0], 0);
                if (ai_Unk583de0(rec, w[8], *(unsigned char *)((char *)w + 0x24)) == 1)
                    U_I((unsigned)&sv_PopRequest) = 1;
                d2_SetViewEnabled(w[0], 1);
            } else if (U_I((unsigned)&ai_Unk5fe710) == btns[1]) {
                d2_SetViewEnabled(w[0], 0);
                if (ai_Unk584260(rec, w[8], *(unsigned char *)((char *)w + 0x24)) == 1)
                    U_I((unsigned)&sv_PopRequest) = 1;
                d2_SetViewEnabled(w[0], 1);
            }
        }
        if (sv_RunFrame(ai_Unk584410, 0x677c7) == 0)
            return;
    }
}
