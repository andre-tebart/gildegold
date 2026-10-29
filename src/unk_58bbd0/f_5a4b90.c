/* 0x5a4b90, the pickpocket victim window "locations\diebesgilde\taschendiebstahl" (421 B, ROUGH): the
   building gate 0x520fe0 and 0x525220 (param_1, 0); the window 0x4741f0 / 0x476f80 / 0x475600 / 0x475f70 (1)
   with the text 0x19f0 and the six-entry list (all -1); each frame 0x524990 (param_1) and the pump 0x5309a0
   (this, 0x677c7): a left click (0xf8a668 == 0) collects up to 6 live entries (byte +0x188) of the table
   0x7603a0 .. 0x760420 and builds the AIMSG (type 0x61 at +4, the owner's character id at +8, param_1 + 1 at
   +0xc, the clock +0x28 .. +0x34, 1 at +0x36, the ids at +0x38 and the owner's id again at +0x80) and sends
   0x4c4b70 / 0x4c2d20 / 0x52ae40, or shows the text 0x144094c with none; a right click only sets 0x6f22a0;
   the exit 0x4758f0 / 0x525440 / 0x52d7a0 */

int sv_Unk524990();

void ai_Unk5a4b90(int param_1)
{
    int win;
    int list[6];
    int n;
    int i;
    int e;
    int *tp;
    char msg[0x88];

    if (sv_CheckCharacterFlag(param_1) != 0)
        return;
    sv_OpenPlayerBar(param_1, 0);
    win = d2_OpenForm(0, 0, "locations\\diebesgilde\\taschendiebstahl");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x19f0);
    for (i = 0; i < 6; i++)
        list[i] = -1;
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        sv_Unk524990(param_1);
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&d2_ClickedId) == 0) {
                n = 0;
                for (tp = (int *)(unsigned)&ai_Unk7603a0; tp < (int *)(unsigned)((unsigned)ai_Unk7603a0 + 0x80) && n < 6; tp++) {
                    e = *tp;
                    if (e != 0 && *(char *)(e + 0x188) != 0) {
                        list[n] = U_I(e + 4);
                        n++;
                    }
                }
                if (n == 0) {
                    sv_MessageBox((char *)U_I((unsigned)&full_data_144094c), 0);
                } else {
                    *(unsigned char *)(msg + 4) = 0x61;
                    U_I(msg + 8) = U_I((unsigned)&ai_CharInts + U_W(param_1 + 0x27) * 0x21c);
                    U_I(msg + 0xc) = U_I(param_1 + 1);
                    U_I(msg + 0x28) = U_I((unsigned)&ai_Time);
                    U_I(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                    U_I(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                    U_W(msg + 0x34) = U_W(((unsigned)&ai_Time + 0xc));
                    *(unsigned char *)(msg + 0x36) = 1;
                    for (i = 0; i < 6; i++)
                        U_I(msg + 0x38 + i * 4) = list[i];
                    U_I(msg + 0x80) = U_I(msg + 8);
                    cm_SendLongMessage(msg);
                    snd_PlayFavourComment();
                    sv_ClearSelection();
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            } else if (U_I((unsigned)&d2_ClickedId) == 1) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk5a4b90, 0x677c7) == 0) {
            d2_CloseView(win);
            sv_ClosePlayerBar();
            sv_CameraFollow();
            return;
        }
    }
}
