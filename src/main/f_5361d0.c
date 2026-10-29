/* 0x5361d0, the player setup of a network game ("Menu\CHOOSEPLAYER"; mask for the frame loop 0x5309a0, flag = quit on the first escape / enter at stage 0): reads the saved values from game.INI ([Network] Name,
   Familienname, Wappen (+ 0x59a), Geschlecht, Glauben), then six stages (stage): 0 first name edit, 1 family name edit (enter = next), 2 sex (two buttons, byte 0x75e5a8), 3 faith (two buttons, byte 0x75e5a9), 4 the coat
   of arms (eight objects 0x59a .. 0x5a1 in a list; click = next), 5 done (0x75e5a4 = the chosen coat object id, 0x75e5a2 = 0x12); the character record 0x75e5a0 holds the names at +0xa (0x75e5aa) / +0x2a (0x75e5ca).
   Writes the values back to the INI file on leaving; returns 1 when finished (ROUGH) */

int main_Unk5361d0(int mask, int flag)
{
    int stage, redraw, ret, f, e1, e2, ok, list, sx, sfx[2], gb[2], gsel, fsel, i, cols, run, e;
    int ent[8];
    char buf[0x30], txt[0x100], c;
    char *s, *d;
    UINT u;

    stage = 0;
    redraw = 1;
    ret = 0;
    GetPrivateProfileStringA("Network", "Name", "Vorname", (char *)((unsigned)&init_PlayerProfile + 0xa), 0x20, (char *)(unsigned)&ai_OptionsFile);
    GetPrivateProfileStringA("Network", "Familienname", "Nachname", (char *)((unsigned)&init_PlayerProfile + 0x2a), 0x20, (char *)(unsigned)&ai_OptionsFile);
    u = GetPrivateProfileIntA("Network", "Wappen", 0, (char *)(unsigned)&ai_OptionsFile);
    U_I(((unsigned)&init_PlayerProfile + 0x4)) = u + 0x59a;
    u = GetPrivateProfileIntA("Network", "Geschlecht", 0, (char *)(unsigned)&ai_OptionsFile);
    *(unsigned char *)((unsigned)&init_PlayerProfile + 0x8) = (char)u;
    u = GetPrivateProfileIntA("Network", "Glauben", 0, (char *)(unsigned)&ai_OptionsFile);
    *(unsigned char *)((unsigned)&init_PlayerProfile + 0x9) = (char)u;
    f = d2_OpenForm(0, 0, "Menu\\CHOOSEPLAYER");
    d2_CentreViewWindows(f);
    io_TownDisplayName(((unsigned)&ai_DataPath + 0x220), txt);
    d2_SetForm(f, 1);
    io_PrintText(0x1acf, txt);
    e1 = d2_GetWinObj(f, 1, io_PrintText(0x1ad0, 0x1ad2));
    d2_SetForm(f, 2);
    e2 = d2_GetWinObj(f, 2, io_PrintText(0x1ad1, 0x1ad3));
    d2_SetForm(f, 3);
    io_PrintText(0x1ad4);
    sv_Unk52e350(d2_LastWindow, 0x1c, sfx, 2, U_I((unsigned)&full_data_1440d14), U_I((unsigned)&full_data_1440d18));
    gb[0] = sfx[0];
    gb[1] = sfx[1];
    d2_SetForm(f, 4);
    io_PrintText(0x1ad7);
    sv_Unk52e350(d2_LastWindow, 0x1c, sfx, 2, U_I((unsigned)&full_data_1440d20), U_I((unsigned)&full_data_1440d24));
    d2_SetForm(f, 5);
    io_PrintText(0x1adb);
    cols = (*(short *)((unsigned)&ai_Unkf8f128 + d2_LastWindow * 0x3c4) - 0x20) / 0x30;
    list = d2_CreateRadioGroup(0);
    i = 0;
    do {
        e = d2_WinOpenPic((i % cols) * 0x30 + 0x50, (i / cols) * 0x30 + 0x20, i + 0x59a, d2_LastWindow);
        ent[i] = e;
        *(int *)(U_I((unsigned)&d2_Objs) + e * 0x2ec + 0x48) = 1;
        *(int *)(U_I((unsigned)&d2_Objs) + e * 0x2ec + 0x28) = 0;
        *(int *)(U_I((unsigned)&d2_Objs) + e * 0x2ec + 0x2c) = 0;
        *(int *)(U_I((unsigned)&d2_Objs) + e * 0x2ec + 0x50) = 1;
        *(int *)(U_I((unsigned)&d2_Objs) + e * 0x2ec + 0x1c0) = 3;
        d2_RadioAdd(list, e);
        i++;
    } while (i < 8);
    d2_SetForm(f, 6);
    ok = d2_GetWinObj(f, 6, io_PrintText(0x7e));
    d2_SetObjValue(e1, ((unsigned)&init_PlayerProfile + 0xa));
    d2_SetObjValue(e2, ((unsigned)&init_PlayerProfile + 0x2a));
    i = 0;
    do {
        if (*(int *)(U_I((unsigned)&d2_Objs) + 8 + ent[i] * 0x2ec) == U_I(((unsigned)&init_PlayerProfile + 0x4))) {
            d2_RadioSelect(list, i);
            break;
        }
        i++;
    } while (i < 8);
    d2_SetObjMax(e1, 0xc);
    d2_SetObjMax(e2, 0xc);
    gsel = d2_CreateRadioGroup(2, gb[0], gb[1]);
    d2_RadioSelect(gsel, *(unsigned char *)((unsigned)&init_PlayerProfile + 0x8));
    fsel = d2_CreateRadioGroup(2, sfx[0], sfx[1]);
    d2_RadioSelect(fsel, *(unsigned char *)((unsigned)&init_PlayerProfile + 0x9));
    d2_ClearInput();
    *(char *)(unsigned)&kb_Last = 0;
    do {
        if (U_I(((unsigned)&dm_L + 0x20)) == 0 && ((U_I((unsigned)&d2_ClickedId) == -1 || ai_Unk5fe710 != ok) && *(char *)(unsigned)&kb_Last != 1)) {
            if (redraw)
                goto lredraw;
        } else {
            if (stage == 0 && flag == 0) {
                U_I((unsigned)&sv_PopRequest) = 1;
                if (redraw)
                    goto lredraw;
            } else {
                stage--;
lredraw:
                i = 0;
                do {
                    d2_EnableObject(d2_GetWinObj(f, 0, i), i <= stage);
                    i++;
                } while (i < 5);
                sx = e1;
                if (stage == 0 || (sx = e2, stage == 1))
                    d2_FocusInput(sx);
                redraw = 0;
            }
        }
        if (stage == 0) {
            if (*(char *)(unsigned)&kb_Last == 0x1c) {
                s = (char *)d2_GetObjValue(e1);
                d = (char *)((unsigned)&init_PlayerProfile + 0xa);
                do {
                    c = *s++;
                    *d++ = c;
                } while (c != 0);
                stage = 1;
                redraw = 1;
                *(char *)(unsigned)&kb_Last = 0;
            }
        } else if (stage == 1) {
            if (*(char *)(unsigned)&kb_Last == 0x1c) {
                s = (char *)d2_GetObjValue(e2);
                d = (char *)((unsigned)&init_PlayerProfile + 0x2a);
                do {
                    c = *s++;
                    *d++ = c;
                } while (c != 0);
                stage = 2;
                redraw = 1;
                *(char *)(unsigned)&kb_Last = 0;
            }
        } else if (stage == 2) {
            d2_MenuNavigate(gsel);
            if (U_I((unsigned)&d2_ClickedId) != -1) {
                if (ai_Unk5fe710 == gb[0]) {
                    *(unsigned char *)((unsigned)&init_PlayerProfile + 0x8) = 0;
                    stage = 3;
                    redraw = 1;
                } else if (ai_Unk5fe710 == gb[1]) {
                    *(unsigned char *)((unsigned)&init_PlayerProfile + 0x8) = 1;
                    stage = 3;
                    redraw = 1;
                }
            }
        } else if (stage == 3) {
            d2_MenuNavigate(fsel);
            if (U_I((unsigned)&d2_ClickedId) != -1) {
                if (ai_Unk5fe710 == sfx[1]) {
                    *(unsigned char *)((unsigned)&init_PlayerProfile + 0x9) = 1;
                    stage = 4;
                    redraw = 1;
                } else if (ai_Unk5fe710 == sfx[0]) {
                    *(unsigned char *)((unsigned)&init_PlayerProfile + 0x9) = 0;
                    stage = 4;
                    redraw = 1;
                }
            }
        } else if (stage == 4) {
            d2_MenuNavigate(list);
            if (*(char *)(unsigned)&kb_Last == 0x1c) {
                stage = 5;
                redraw = 1;
            }
            if (0x599 < U_I((unsigned)&d2_ClickedId) && U_I((unsigned)&d2_ClickedId) < 0x5a2) {
                i = 0;
                do {
                    e = ent[i] * 0x2ec;
                    if (U_I((unsigned)&d2_Objs) + e != U_I((unsigned)&d2_Unkf8a670)) {
                        *(int *)(U_I((unsigned)&d2_Objs) + e + 0x28) = 0;
                        *(int *)(U_I((unsigned)&d2_Objs) + e + 0x2c) = 0;
                        *(int *)(U_I((unsigned)&d2_Objs) + e + 0x50) = 1;
                    }
                    i++;
                } while (i < 8);
                stage++;
            }
        } else if (stage == 5) {
            *(unsigned char *)(unsigned)&init_PlayerProfile = 0;
            *(unsigned char *)((unsigned)&init_PlayerProfile + 0x1) = 0;
            *(unsigned char *)((unsigned)&init_PlayerProfile + 0x2) = 0x12;
            U_I(((unsigned)&init_PlayerProfile + 0x4)) = *(int *)(U_I((unsigned)&d2_Objs) + 8 + U_I(((unsigned)&d2_RadioSel + 0x4) + (list * 0x23 + U_I((unsigned)&d2_RadioSel + list * 0x8c)) * 4) * 0x2ec);
            ret = 1;
            U_I((unsigned)&sv_PopRequest) = 1;
        }
        run = sv_RunFrame((int)main_Unk5361d0, mask);
        if (run == 0) {
            d2_RadioClear(gsel);
            d2_RadioClear(fsel);
            d2_RadioClear(list);
            d2_CloseView(f);
            WritePrivateProfileStringA("Network", "Name", (char *)((unsigned)&init_PlayerProfile + 0xa), (char *)(unsigned)&ai_OptionsFile);
            WritePrivateProfileStringA("Network", "Familienname", (char *)((unsigned)&init_PlayerProfile + 0x2a), (char *)(unsigned)&ai_OptionsFile);
            _itoa(U_I(((unsigned)&init_PlayerProfile + 0x4)) - 0x59a, buf, 10);
            WritePrivateProfileStringA("Network", "Wappen", buf, (char *)(unsigned)&ai_OptionsFile);
            _itoa(*(unsigned char *)((unsigned)&init_PlayerProfile + 0x8), buf, 10);
            WritePrivateProfileStringA("Network", "Geschlecht", buf, (char *)(unsigned)&ai_OptionsFile);
            _itoa(*(unsigned char *)((unsigned)&init_PlayerProfile + 0x9), buf, 10);
            WritePrivateProfileStringA("Network", "Glauben", buf, (char *)(unsigned)&ai_OptionsFile);
            return ret;
        }
    } while (1);
}
