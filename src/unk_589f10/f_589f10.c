/* 0x589f10, the network lobby screen (2595 B, ROUGH): clears the player record 0x6f5e20 (0x57 B), reads the
   "Network" profile (name / family name / emblem / gender / religion / profession) into it, opens the lobby
   screen "menu\netlobby" 0x62aa8c, builds the four sub windows (0x58a9d0 / 0x58a940 / 0x58aa30 / 0x58ad20 /
   0x58aba0 / 0x58ada0) and runs its frame loop 0x5309a0 (the callback 0x589f10, mask 0x24087): the input key
   0x1468410 (0x1c = enter) toggles the active text field, the clicked control 0x5fe710 is compared with the
   window ids 0x62aa7c / 0x62aa80 / 0x62aa84 (toggle the selected player, the city and the profession pickers)
   and with the player list 0x6f5e80; the engine state 0x1460ae0 == -1 ends the screen (returns -1); the
   player's choice is written back to the profile ("%d" 0x629188, 0x5de652) and the record 0x75e5a0 ..
   0x75e5ca keeps the last choice. The paths where the record 0x6f5ea4 / 0x6f5e78 is missing open the message
   box 0x60ea78 instead. ROUGH: the text of the sub blocks is transcribed from the disassembly / pseudocode,
   the exact store order and the local frame are not verified
   @deep Q1: unit unnamed (@named_by -), no evidence for the function name; B: code 1020 + regs 12, mine 2448 B vs 2595 B with the original's ebp frame 0x250 (FPO difference), ROUGH draft, the store order and the 0x5a/0x7f paths unverified (unit note Open work); Q2 40 callees, Q3 63 addresses, Q4 U_I/U_B/U_W + 2 casts, Q5 ROUGH; structural byte work, not a meaning fix */

int ai_Unk589f10(void)
{
    char buf[256];
    char text[256];
    char name[72];
    int ret;
    int state;
    int win;
    int id;
    int i;
    int n;
    char *p;
    char *r;

    ret = 0;
    ai_Unk4cd500();
    memset((char *)(unsigned)&full_data_6f5e20, 0, 0x57);
    U_I((unsigned)&full_data_6f267c) = 0;
    U_I((unsigned)&full_data_6f5e00) = -1;
    U_I((unsigned)&full_data_6f5ea4) = ai_Unk4cd600();
    if (U_I((unsigned)&full_data_6f5ea4) == 0) {
        win = d2_OpenForm(0, 0, (char *)(unsigned)"misc\\Messagebox_NONE_PERGA");
        d2_CentreViewWindows(win);
        io_PrintText(0x87);
        U_I((unsigned)&d2_ClickedId) = -1;
        while (U_I(((unsigned)&dm_L + 0x20)) == 0 && U_B((unsigned)&kb_Last) != 1) {
            if (sv_RunFrame((int)ai_Unk589f10, 0x24087) == 0) {
                d2_CloseView(win);
                return 0;
            }
        }
        U_I((unsigned)&sv_PopRequest) = 1;
        d2_CloseView(win);
        return 0;
    }
    GetPrivateProfileStringA((char *)(unsigned)"Network", (char *)(unsigned)"Name", (char *)(unsigned)"Vorname", (char *)(unsigned)&full_data_6f5e32, 0x20,
                             (char *)(unsigned)&ai_OptionsFile);
    GetPrivateProfileStringA((char *)(unsigned)"Network", (char *)(unsigned)"Familienname", (char *)(unsigned)"Nachname", (char *)(unsigned)&full_data_6f5e42, 0x20,
                             (char *)(unsigned)&ai_OptionsFile);
    U_I((unsigned)&full_data_6f5e2c) = GetPrivateProfileIntA((char *)(unsigned)"Network", (char *)(unsigned)"Wappen", 0, (char *)(unsigned)&ai_OptionsFile);
    U_B((unsigned)&full_data_6f5e30) = (char)GetPrivateProfileIntA((char *)(unsigned)"Network", (char *)(unsigned)"Geschlecht", 0, (char *)(unsigned)&ai_OptionsFile);
    U_B((unsigned)&full_data_6f5e31) = (char)GetPrivateProfileIntA((char *)(unsigned)"Network", (char *)(unsigned)"Glauben", 0, (char *)(unsigned)&ai_OptionsFile);
    U_B((unsigned)&full_data_6f5e2b) = (char)GetPrivateProfileIntA((char *)(unsigned)"Network", (char *)(unsigned)&full_data_62aa9c, 0, (char *)(unsigned)&ai_OptionsFile);
    U_B((unsigned)&full_data_6f5e2b) = gm_TitleNumber(gm_TitleGroup(U_B((unsigned)&full_data_6f5e2b)), 1);
    U_B(((unsigned)&init_PlayerProfile + 0x2)) = 0x10;
    U_B((unsigned)&full_data_6f5e2a) = 0x10;
    U_B((unsigned)&full_data_6f5e57) = 0x46;
    U_B((unsigned)&full_data_6f5e59) = 0x46;
    U_I((unsigned)&full_data_6f5e52) = -1;
    U_B((unsigned)&full_data_6f5e56) = 0x5a;
    U_B((unsigned)&full_data_6f5e58) = 0x78;
    U_B((unsigned)&full_data_6f5e5a) = 0x5a;
    if (U_I((unsigned)&ai_ModeMask) == 0x7f) {
        U_B((unsigned)&full_data_6f5e2b) = U_B(((unsigned)&ai_Unk9364e0 + 0x2e0));
        U_B((unsigned)&full_data_6f5e30) = U_B(((unsigned)&ai_Unk9364e0 + 0x2e4));
        U_I((unsigned)&full_data_6f5e2c) = U_I(((unsigned)&ai_Unk9364e0 + 0x2ec)) - 0x59a;
        U_B((unsigned)&full_data_6f5e31) = U_B(((unsigned)&ai_Unk9364e0 + 0x2e8));
        strcpy((char *)(unsigned)&full_data_6f5e32, (char *)((unsigned)&ai_Unk9364e0 + 0x2c0));
        strcpy((char *)(unsigned)&full_data_6f5e42, (char *)((unsigned)&ai_Unk9364e0 + 0x2d0));
    }
    id = ((int (*)(void *))ai_Unk4cd760)((char *)(unsigned)&full_data_6f5e20);
    if (id == -1)
        return -1;
    while (cm_GetSlotState(id) == 0)
        sv_PumpNet();
    if (cm_GetSlotState(id) == 2) {
        txt_Format(buf, 0x88);
        sv_MessageBox(buf, 0x100);
        return -1;
    }
    id = ((int (*)())ai_Unk4cd610)();
    if (id == -1)
        return -1;
    while (cm_GetSlotState(id) == 0)
        sv_PumpNet();
    d2_ShowPicture(0x7d6);
    U_I((unsigned)&full_data_6f5e78) = ai_Unk4cd5f0();
    if (U_I((unsigned)&full_data_6f5e78) == 0) {
        win = d2_OpenForm(0, 0, (char *)(unsigned)"misc\\Messagebox_NONE_PERGA");
        d2_CentreViewWindows(win);
        io_PrintText(0x87);
        U_I((unsigned)&d2_ClickedId) = -1;
        while (U_I(((unsigned)&dm_L + 0x20)) == 0 && U_B((unsigned)&kb_Last) != 1) {
            if (sv_RunFrame((int)ai_Unk589f10, 0x24087) == 0) {
                d2_CloseView(win);
                return 0;
            }
        }
        U_I((unsigned)&sv_PopRequest) = 1;
        d2_CloseView(win);
        return 0;
    }
    U_I((unsigned)&full_data_62aa88) = d2_OpenForm(0x3c, 0, (char *)(unsigned)&full_data_62aa8c);
    d2_CentreViewWindows(U_I((unsigned)&full_data_62aa88));
    ai_Unk58a9d0();
    ai_Unk58a940();
    ai_Unk58aa30();
    ai_Unk58ad20();
    ai_Unk58aba0();
    ai_Unk58ada0();
    d2_SetForm(U_I((unsigned)&full_data_62aa88), 1);
    id = d2_GetWinObj(U_I((unsigned)&full_data_62aa88), 0, (char *)0);
    d2_SetObjMax(id, 0x40);
    state = 0;
    d2_SetObjValue(id, (char *)(unsigned)&ch_EmptyName);
    i = id * 0x2ec;
    *(short *)(i + 0x14 + U_I((unsigned)&d2_Objs)) = 0x26c;
    n = *(int *)(i + 0x78 + U_I((unsigned)&d2_Objs));
    d2_Inputs[n].flags |= 0x200;
    d2_Inputs[n].w = 0x26c;
    U_I((unsigned)&d2_ClickedId) = -1;
    d2_Inputs[n].maxw = 0x26c;
    while (1) {
        if (state == 0) {
            if (U_I((unsigned)&d2_FocusedInput) == 0) {
                if (U_B((unsigned)&kb_Last) == 0x1c) {
                    d2_FocusInput(id);
                    state = 1;
                }
            } else {
                state = 1;
            }
        } else if (U_I((unsigned)&d2_FocusedInput) == 0 || U_B((unsigned)&kb_Last) == 0x1c) {
            state = 0;
            p = (char *)d2_GetObjValue(id);
            strcpy(name, p);
            d2_SetObjValue(id, (char *)(unsigned)&ch_EmptyName);
            if (strlen(name) != 0)
                ai_Unk4cd6e0(U_I((unsigned)&full_data_6f5e00) == -1 ? 0 : U_I((unsigned)&full_data_6f5e00), name);
        }
        if (U_I((unsigned)&ai_Unk1460ae0) == -1) {
            ret = -1;
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (ai_Unk4cd510() == 0) {
            if (U_I((unsigned)&d2_ClickedId) != -1) {
                if (U_I((unsigned)&ai_Unk5fe710) == U_I((unsigned)&full_data_62aa84)) {
                    n = ((int (*)())ai_Unk4cd6a0)();
                    while (cm_GetSlotState(n) == 0)
                        sv_PumpNet();
                    U_I((unsigned)&sv_PopRequest) = 1;
                    ret = -1;
                } else if (U_I((unsigned)&ai_Unk5fe710) == U_I((unsigned)&full_data_62aa7c)) {
                    n = ((int (*)(int))ai_Unk4cd650)(U_I((unsigned)&full_data_6f5e78 + 4) == 0);
                    while (cm_GetSlotState(n) == 0)
                        sv_PumpNet();
                    U_I((unsigned)&d2_Objs + 0x28 + U_I((unsigned)&full_data_62aa7c) * 0x2ec) = U_I((unsigned)&full_data_6f5e78 + 4);
                    if (ai_Unk4cd510() != 0)
                        U_I((unsigned)&sv_PopRequest) = 1;
                } else if (U_I((unsigned)&ai_Unk5fe710) == U_I((unsigned)&full_data_62aa80)) {
                    if (U_I((unsigned)&full_data_6f5e78 + 4) != 0) {
                        n = ((int (*)(int))ai_Unk4cd650)(0);
                        while (cm_GetSlotState(n) == 0)
                            sv_PumpNet();
                        U_I((unsigned)&d2_Objs + 0x28 + U_I((unsigned)&full_data_62aa7c) * 0x2ec) = U_I((unsigned)&full_data_6f5e78 + 4);
                    }
                    if (U_I((unsigned)&full_data_6f5e78 + 4) == 0 && ai_Unk58af50(0x24087, 1) != 0) {
                        strcpy((char *)(unsigned)&full_data_6f5e32, (char *)((unsigned)&init_PlayerProfile + 0xa));
                        strcpy((char *)(unsigned)&full_data_6f5e42, (char *)((unsigned)&init_PlayerProfile + 0x2a));
                        U_I((unsigned)&full_data_6f5e2c) = U_I(((unsigned)&init_PlayerProfile + 0x4)) - 0x59a;
                        U_B((unsigned)&full_data_6f5e30) = U_B(((unsigned)&init_PlayerProfile + 0x8));
                        U_B((unsigned)&full_data_6f5e31) = U_B(((unsigned)&init_PlayerProfile + 0x9));
                        n = ((int (*)(void *))ai_Unk4cd7c0)((char *)(unsigned)&full_data_6f5e20);
                        while (cm_GetSlotState(n) == 0)
                            sv_PumpNet();
                        if (ai_Unk58b880(0x24087) != 0) {
                            U_B((unsigned)&full_data_6f5e2b) = U_B(((unsigned)&init_PlayerProfile + 0x3));
                            n = ((int (*)(void *))ai_Unk4cd7c0)((char *)(unsigned)&full_data_6f5e20);
                            while (cm_GetSlotState(n) == 0)
                                sv_PumpNet();
                        }
                    }
                } else {
                    /* the player list: the clicked entry 0x6f5e80[i] is the only one selected, the id
                       0x6f267c[i + 1] goes to 0x6f5e00 */
                    for (i = 0; i < U_I((unsigned)&full_data_6f267c); i++) {
                        if (i != U_I((unsigned)&full_data_6f5ea0) && U_I((unsigned)&ai_Unk5fe710) == U_I((unsigned)&full_data_6f5e80 + i * 4)) {
                            n = U_I((unsigned)&d2_Objs + 0x28 + U_I((unsigned)&full_data_6f5e80 + i * 4) * 0x2ec) == 0;
                            U_I((unsigned)&d2_Objs + 0x28 + U_I((unsigned)&full_data_6f5e80 + i * 4) * 0x2ec) = n;
                            if (n == 0) {
                                U_I((unsigned)&full_data_6f5e00) = -1;
                            } else {
                                U_I((unsigned)&full_data_6f5e00) = U_I((unsigned)&full_data_6f267c + (i + 1) * 4);
                            }
                            if (-1 < i) {
                                for (n = 0; n < U_I((unsigned)&full_data_6f267c); n++)
                                    if (i != n)
                                        U_I((unsigned)&d2_Objs + 0x28 + U_I((unsigned)&full_data_6f5e80 + n * 4) * 0x2ec) = 0;
                            }
                            break;
                        }
                    }
                }
            }
        } else {
            U_I((unsigned)&sv_PopRequest) = 1;
        }
        U_I((unsigned)&d2_ClickedId) = -1;
        if (U_I((unsigned)&full_data_6f5ea4 + 4) != 0) {
            if (ai_Unk4cd510() == 0) {
                n = ((int (*)())ai_Unk4cd610)();
                while (cm_GetSlotState(n) == 0)
                    sv_PumpNet();
                U_I((unsigned)&full_data_6f267c) = ai_Unk4cd5d0();
                ai_Unk58aba0();
                ai_Unk58ada0();
                ai_Unk58ba70();
                U_I((unsigned)&full_data_6f5ea4 + 4) = 0;
            } else {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame((int)ai_Unk589f10, 0x24087) == 0) {
            d2_CloseView(U_I((unsigned)&full_data_62aa88));
            r = (char *)U_I((unsigned)&full_data_6f5e78);
            U_B(((unsigned)&init_PlayerProfile + 0x2)) = 0x12;
            U_B(((unsigned)&init_PlayerProfile + 0x3)) = U_B(r + 0xb);
            U_W(((unsigned)&init_PlayerProfile + 0x8)) = U_W(r + 0x10);
            U_I(((unsigned)&init_PlayerProfile + 0x4)) = U_I(r + 0xc);
            strcpy((char *)((unsigned)&init_PlayerProfile + 0x2a), r + 0x22);
            strcpy((char *)((unsigned)&init_PlayerProfile + 0xa), r + 0x12);
            WritePrivateProfileStringA((char *)(unsigned)"Network", (char *)(unsigned)"Name", (char *)((unsigned)&init_PlayerProfile + 0xa),
                                       (char *)(unsigned)&ai_OptionsFile);
            WritePrivateProfileStringA((char *)(unsigned)"Network", (char *)(unsigned)"Familienname", (char *)((unsigned)&init_PlayerProfile + 0x2a),
                                       (char *)(unsigned)&ai_OptionsFile);
            sprintf(text, (char *)(unsigned)"%d", U_I(((unsigned)&init_PlayerProfile + 0x4)));
            WritePrivateProfileStringA((char *)(unsigned)"Network", (char *)(unsigned)"Wappen", text, (char *)(unsigned)&ai_OptionsFile);
            sprintf(text, (char *)(unsigned)"%d", U_B(((unsigned)&init_PlayerProfile + 0x8)));
            WritePrivateProfileStringA((char *)(unsigned)"Network", (char *)(unsigned)"Geschlecht", text, (char *)(unsigned)&ai_OptionsFile);
            sprintf(text, (char *)(unsigned)"%d", U_B(((unsigned)&init_PlayerProfile + 0x9)));
            WritePrivateProfileStringA((char *)(unsigned)"Network", (char *)(unsigned)"Glauben", text, (char *)(unsigned)&ai_OptionsFile);
            sprintf(text, (char *)(unsigned)"%d", U_B(((unsigned)&init_PlayerProfile + 0x3)));
            WritePrivateProfileStringA((char *)(unsigned)"Network", (char *)(unsigned)&full_data_62aa9c, text, (char *)(unsigned)&ai_OptionsFile);
            d3io_NoopCallback();
            return ret;
        }
    }
}
