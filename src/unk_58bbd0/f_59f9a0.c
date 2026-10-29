/* 0x59f9a0, the Rathaus guild hall window "locations\Rathaus\Rathaus_Geb_Info" (592 B): the window 0x4741f0,
   the text 0x17f6, the buttons 0x5e56c4 counts; on rebuild 0x501190 (1, 6) walks the citizens with the byte
   +0x5a & 2, each with a carrier 0x53e400 (1, 0, 0x1b) gets the text 0x17f5 (*param_1 + 0x427, the slot,
   the record) and a control 0x475b80; the clicked control 0x5fe710 of another player's record 0x521040 (the
   player, the carrier + 0xb0, the player byte) and 0x521960 (the record, -1, text 0x631090); the pump
   0x5309a0 (0x59f9a0, 0x677c7) and 0x4758f0 */

void ai_Unk59f9a0(void)
{
    int list[512];
    int win;
    int n;
    int i;
    int ctl;
    int rec;
    int carrier;
    unsigned short flag;
    int *e;

    win = d2_OpenForm(0, 0, "locations\\Rathaus\\Rathaus_Geb_Info");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 2);
    i = U_I((unsigned)&d2_LastWindow);
    d2_SetForm(win, 0);
    d2_CreateScrollButtons(-1, -1, U_I((unsigned)&d2_LastWindow), i, 0x767);
    d2_SetPointerSprite(0, 0);
    U_I(((unsigned)&ai_Unkf8f13c + 0x248) + i * 0x3c4) = 0x5a;
    n = 0;
    for (;;) {
        if (n == 0) {
            d2_SetForm(win, 1);
            io_PrintText(0x17f6);
            d2_SetForm(win, 2);
            io_PrintText("$C");
            d2_RemoveWindowObjects(U_I((unsigned)&d2_LastWindow), 1);
            n = 0;
            rec = gm_BuildQuery(1, 6);
            e = list;
            while (rec != 0) {
                if ((*(unsigned char *)(rec + 0x5a) & 2) != 0) {
                    carrier = he_FindEvent(1, 0, 0x1b);
                    while (carrier != 0) {
                        if (U_I(carrier + 0xac) == U_I(rec + 1)) {
                            e[1] = carrier;
                            e[2] = rec;
                            e[3] = *(unsigned short *)(rec + 0x5a);
                            e[0] = d2_GetWinObj(win, 2, io_PrintText(0x17f5, *(unsigned char *)rec + 0x427,
                                                                        *(unsigned short *)(rec + 0x27), rec,
                                                                        U_I(carrier + 0xb0)));
                            n++;
                            e += 4;
                            break;
                        }
                        carrier = he_NextEvent();
                    }
                }
                rec = gm_BuildNext();
            }
        }
        i = 0;
        while (i < n) {
            if (U_I(list[i * 4 + 3]) != *(unsigned short *)(U_I(list[i * 4 + 2]) + 0x5a)) {
                n = 0;
                break;
            }
            i++;
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
            sv_CameraFollow();
        }
        if (U_I((unsigned)&d2_ClickedId) != -1 && n > 0) {
            for (i = 0; i < n; i++) {
                ctl = list[i * 4];
                rec = list[i * 4 + 2];
                carrier = list[i * 4 + 1];
                if (U_I((unsigned)&ai_Unk5fe710) == ctl && *(unsigned short *)(rec + 0x27) != U_W((unsigned)&ai_PlayerId)) {
                    if (sv_CheckGoodAmount((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c, U_I(carrier + 0xb0),
                                     U_I((unsigned)&ai_OwnPlayer)) != 0)
                        sv_SelectPanelObject(rec, -1, (char *)(unsigned)&ch_EmptyName);
                }
            }
        }
        if (sv_RunFrame(ai_Unk59f9a0, 0x677c7) == 0) {
            d2_CloseView(win);
            return;
        }
    }
}
