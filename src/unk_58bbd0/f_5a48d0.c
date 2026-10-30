/* 0x5a48d0, the pickpocket window "locations\diebesgilde\taschendiebstahl" (596 B, ROUGH): the window
   0x4741f0 / 0x476f80 / 0x475600 / 0x475f70 (1) with the text 0x19ee (the owner slot, param_1) and the two
   buttons 0x475b80 (the text and +1); the pump 0x5309a0 (this, 0x677c7): the first button looks for a carrier
   0x53e400 (3, 0, 0x49, 2, the guild slot, the guild id, 3) / 0x53e300 whose +0xac is param_1 + 1: with one,
   0x6f22a0 = 2 and every live entry (byte +0x188) of the table 0x7603a0 .. 0x760420 gets 0x524260 (0, 0, its
   word, the carrier); without one the AIMSG is built (type 0x49 at +4, the guild's character id at +8, the
   guild id at +0xc, the clock 0x770ae0 .. +0x34, 1 at +0x36, param_1 + 1 at +0x58, the guild id at +0x5c and
   at +0x38 the +4 of up to 8 live table entries) and sent 0x4c4b70 / 0x4c2d20 with 0x6f22a0 = 2, or the text
   0x1440ae4 with none; the second button only sets 0x6f22a0; the exit 0x4758f0; returns 1 after the click */

void sv_MoveToGroup(unsigned char *o, short *it, int c, char *g);

int ai_Unk5a48d0(int param_1)
{
    int win;
    int ctl1;
    int ctl2;
    int id;
    int done = 0;
    int n;
    int i;
    char *guild;
    char *carrier;
    char *e;
    int *tp;
    char msg[0x60];

    win = d2_OpenForm(0, 0, "locations\\diebesgilde\\taschendiebstahl");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    id = io_PrintText(0x19ee, U_W(param_1 + 0x27), param_1);
    ctl1 = d2_GetWinObj(win, 1, id);
    ctl2 = d2_GetWinObj(win, 1, id + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) != -1 && U_I((unsigned)&ai_Unk5fe710) == ctl1) {
            guild = (char *)U_I((unsigned)&ai_PlayerBuilding);
            carrier = (char *)he_FindEvent(3, 0, 0x49, 2, U_W(guild + 0x27),
                                           U_I(guild + 1), 3);
            while (carrier != 0 && U_I(carrier + 0xac) != U_I(param_1 + 1))
                carrier = (char *)he_NextEvent();
            if (carrier != 0) {
                U_I((unsigned)&sv_PopRequest) = 2;
                for (tp = (int *)(unsigned)&ai_Unk7603a0; tp < (int *)(unsigned)((unsigned)ai_Unk7603a0 + 0x80); tp++) {
                    e = (char *)*tp;
                    if (e != 0 && *(unsigned char *)(e + 0x188) != 0)
                        sv_MoveToGroup(0, 0, U_W(e), carrier);
                }
                done = 1;
            } else {
                *(unsigned char *)(msg + 4) = 0x49;
                U_I(msg + 8) = U_I((unsigned)&ai_CharInts + U_W(guild + 0x27) * 0x21c);
                U_I(msg + 0xc) = U_I(guild + 1);
                U_I(msg + 0x28) = U_I((unsigned)&ai_Time);
                U_I(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                U_I(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                U_W(msg + 0x34) = U_W(((unsigned)&ai_Time + 0xc));
                *(unsigned char *)(msg + 0x36) = 1;
                U_I(msg + 0x58) = U_I(param_1 + 1);
                U_I(msg + 0x5c) = U_I(guild + 1);
                for (i = 0; i < 8; i++)
                    U_I(msg + 0x38 + i * 4) = -1;
                n = 0;
                for (tp = (int *)(unsigned)&ai_Unk7603a0; tp < (int *)(unsigned)((unsigned)ai_Unk7603a0 + 0x80); tp++) {
                    e = (char *)*tp;
                    if (e != 0 && *(unsigned char *)(e + 0x188) != 0) {
                        U_I(msg + 0x38 + n * 4) = U_I(e + 4);
                        n++;
                    }
                }
                if (n == 0) {
                    sv_MessageBox((char *)U_I((unsigned)&full_data_1440ae4), 0);
                } else {
                    done = 1;
                    cm_SendLongMessage(msg);
                    snd_PlayFavourComment();
                    U_I((unsigned)&sv_PopRequest) = 2;
                }
            }
        } else if (U_I((unsigned)&ai_Unk5fe710) == ctl2) {
            U_I((unsigned)&sv_PopRequest) = 1;
        }
        if (sv_RunFrame(ai_Unk5a48d0, 0x677c7) == 0)
            break;
    }
    d2_CloseView(win);
    return done;
}
