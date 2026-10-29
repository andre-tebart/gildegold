/* 0x586560, the graveyard seance selection window (171 B): a / w the window record (its +0 the window, +0x88 the
   selected entry, +0x28 the eight entries of 3 dwords), out the result; 0xf8a668 = -1; the pump loop 0x5309a0
   (this window, 0x677c7): the escape 0x14673a0 -> 0x6f22a0 = 1; a clicked entry i -> w[0x22] = i, 0x475590 (the
   window, 0), 0x585750 (a, w, out) and 0x475590 (the window, 1); a non-zero result -> 1 and 0x6f22a0 = 1; the
   pump 0 -> the result */


int ai_Unk586560(char *a, int *w, int *out)
{
    int r;
    int i;

    r = 0;
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) == 0) {
            if (U_I((unsigned)&d2_ClickedId) != -1) {
                for (i = 0; i < 8; i++) {
                    if (U_I((unsigned)&ai_Unk5fe710) == w[10 + i * 3]) {
                        w[0x22] = i;
                        d2_SetViewEnabled(w[0], 0);
                        r = ai_Unk585750(a, (char *)w, out);
                        d2_SetViewEnabled(w[0], 1);
                        break;
                    }
                }
                if (r != 0) {
                    r = 1;
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            }
        } else {
            U_I((unsigned)&sv_PopRequest) = 1;
        }
        if (sv_RunFrame(ai_Unk586560, 0x677c7) == 0)
            return r;
    }
}
