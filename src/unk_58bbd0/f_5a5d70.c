/* 0x5a5d70, the city watch window "LOCATIONS\STADTWACHE\STANDARD" (493 B; a Ghidra-missed start, the callback
   of 0x5b2180 in 0x5a5f60): param_2 the record (+1 the owner); without param_2 return 0; the scan of the crew
   byte +0x184 at 0x770d08 over the 0x300 records, none -> the text 0x1440b38 and 0; the window 0x4741f0 /
   0x475600 / 0x476f80 / 0x475f70 (1) with two controls from 0x515960 (0x1a69) / 0x475b80; the pump 0x5309a0
   (this, 0x677c7): a click on the first control sends the AIMSG type 0x43 (+8 the player's character, 2 at
   +0x36, the owner +1 at +0x58, the current building +1 at +0x60, the up to four crew ids +0x70 .. +0x7c) and
   0x4c2d20 / 0x52ae40 with 0x6f22a0 = 2, or only 0x52ae40 with no crew; the second control sets 0x6f22a0 = 1;
   the exit 0x4758f0 / 0x52d7a0, returns 1 */

int ai_Unk5a5d70(int param_1, int param_2)
{
    int list[4];
    int win;
    int ctl1;
    int ctl2;
    int id;
    int n;
    int i;
    char *p;
    char msg[0x108];

    if (param_2 == 0)
        return 0;
    for (i = 0; i < 0x300; i++) {
        if (*(char *)((unsigned)&ai_Selected + i * 0x21c) != 0)
            break;
    }
    if (i == 0x300) {
        sv_MessageBox((char *)U_I((unsigned)&full_data_1440b38), 0);
        return 0;
    }
    win = d2_OpenForm(0, 0, "LOCATIONS\STADTWACHE\STANDARD");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    id = io_PrintText(0x1a69);
    ctl1 = d2_GetWinObj(win, 1, id);
    ctl2 = d2_GetWinObj(win, 1, id + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == ctl1) {
                for (i = 0; i < 4; i++)
                    list[i] = -1;
                n = 0;
                for (p = (char *)(unsigned)&ai_CharInts; p < (char *)((unsigned)((unsigned)ai_CharInts + 0x653fe) + 0x2) && n < 4; p += 0x21c) {
                    if (*(char *)(p + 0x184) != 0) {
                        list[n] = U_I(p);
                        n++;
                    }
                }
                if (n != 0) {
                    *(unsigned char *)(msg + 4) = 0x43;
                    *(unsigned char *)(msg + 0x36) = 2;
                    U_I(msg + 8) = U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x21c);
                    U_I(msg + 0x58) = U_I(param_2 + 1);
                    U_I(msg + 0x60) = U_I(U_I((unsigned)&ai_PlayerBuilding) + 1);
                    for (i = n; i < 4; i++)
                        list[i] = -1;
                    for (i = 0; i < 4; i++)
                        U_I(msg + 0x70 + i * 4) = list[i];
                    cm_SendLongMessage(msg);
                    snd_PlayFavourComment();
                }
                sv_Unk52ae40();
                U_I((unsigned)&sv_PopRequest) = 2;
            } else if (U_I((unsigned)&ai_Unk5fe710) == ctl2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk5a5d70, 0x677c7) == 0)
            break;
    }
    d2_CloseView(win);
    sv_CameraFollow();
    return 1;
}
