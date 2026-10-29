/* 0x5a5fd0, the city watch window with six crew slots (533 B; a Ghidra-missed start, the callback of 0x5b2180 in
   0x5a61f0): like 0x5a5d70 (param_1 the record, the scan of the crew byte at 0x770d08, the window
   "LOCATIONS\STADTWACHE\STANDARD", the two controls from 0x515960 (0x1a6d, param_1)) but the AIMSG type 0x44
   with the up to six crew ids at +0x38 and, after 0x4c4b70 / 0x4c2d20, the count of every crew character: above
   six the text 0x19ef (6) shown; 0x6f22a0 = 2; the exit 0x4758f0 / 0x52d7a0, returns 1 */

int ai_Unk5a5fd0(int param_1, int param_2)
{
    int list[8];
    int win;
    int ctl1;
    int ctl2;
    int id;
    int n;
    int i;
    int total;
    char *p;
    char msg[0x108];
    char buf[0x400];

    if (param_1 == 0)
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
    id = io_PrintText(0x1a6d, param_1);
    ctl1 = d2_GetWinObj(win, 1, id);
    ctl2 = d2_GetWinObj(win, 1, id + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == ctl1) {
                for (i = 0; i < 8; i++)
                    list[i] = -1;
                n = 0;
                for (p = (char *)(unsigned)&ai_CharInts; p < (char *)((unsigned)((unsigned)ai_CharInts + 0x653fe) + 0x2) && n < 6; p += 0x21c) {
                    if (*(char *)(p + 0x184) != 0) {
                        list[n] = U_I(p);
                        n++;
                    }
                }
                if (n != 0) {
                    *(unsigned char *)(msg + 4) = 0x44;
                    *(unsigned char *)(msg + 0x36) = 2;
                    U_I(msg + 8) = U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x21c);
                    U_I(msg + 0x58) = U_I(param_1 + 1);
                    U_I(msg + 0x60) = U_I(U_I((unsigned)&ai_PlayerBuilding) + 1);
                    for (i = 0; i < 6; i++)
                        U_I(msg + 0x38 + i * 4) = list[i];
                    cm_SendLongMessage(msg);
                    snd_PlayFavourComment();
                    total = 0;
                    for (p = (char *)(unsigned)&ai_Selected; p < (char *)((unsigned)((unsigned)ai_Selected + 0x6527a) + 0x186); p += 0x21c) {
                        if (*(char *)p != 0)
                            total++;
                    }
                    if (total > 6) {
                        txt_Format(buf, 0x19ef, 6);
                        sv_MessageBox(buf, 0);
                    }
                }
                U_I((unsigned)&sv_PopRequest) = 2;
            } else if (U_I((unsigned)&ai_Unk5fe710) == ctl2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk5a5fd0, 0x677c7) == 0)
            break;
    }
    d2_CloseView(win);
    sv_CameraFollow();
    return 1;
}
