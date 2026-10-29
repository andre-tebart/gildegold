/* 0x536980, the history choice of the new game ("Menu\CHOOSEHISTORY": the texts 0x1ac0 ..; three or four buttons by 0x5190a0: the level 0x4b1f90 (1 / 2 / 0), the group value 0x6f365c from 0x6f0724 shown by 0x46c630), then the
   start menu pages: 0x4b4040 (the history choice, hist), 0x5361d0 (the player setup, cb mask 0xc6), the character creation 0x536d80 / 0x5359f0 (1 = continue with the family choice, 0 = quick start with the model
   "patrizier_MANN2" (0x75e628 = index + 0x639), -1 = back) and the scripted start menu pages "Startmenu/B_Persoenliches.esc" / "Startmenu/A_Stadtwahl.esc" (0x4e2bf0 / 0x4e2c70, 0x760638 = 0x12d6); returns
   non-zero when the character was created, sets the mode flag 0x6f23f8 |= 8 then (ROUGH) */

int main_Unk536d80(void);

int main_Unk536980(void)
{
    int again, back, ret, f, b0, b1, b2, grp, run, hist, h, r, v;
    int *p;
    char loc[8];

    again = 0;
    ret = 0;
    b1 = -1;
    hist = 0xff;
    back = 0;
    f = d2_OpenForm(0, 0, "Menu\\CHOOSEHISTORY");
    d2_CentreViewWindows(f);
    d2_SetForm(f, 0);
    if (ls_GetLanguage() == 0) {
        h = io_PrintText(0x1ac0, 0x1ac2, 0x1ac3, 0x1ac4, 0x1ac2, 0x1ac3, 0x1ac4, 1);
        b0 = d2_GetWinObj(f, 0, h);
        b1 = d2_GetWinObj(f, 0, h + 1);
        b2 = d2_GetWinObj(f, 0, h + 2);
        grp = d2_CreateRadioGroup(4, b0, b1, b2, d2_GetWinObj(f, 0, h + 3));
    } else {
        h = io_PrintText(0x1ac0, 0x1ac2, 0x1ac4, 0x1ac2);
        b0 = d2_GetWinObj(f, 0, h);
        b2 = d2_GetWinObj(f, 0, h + 1);
        grp = d2_CreateRadioGroup(3, b0, b2, d2_GetWinObj(f, 0, h + 2));
    }
    if (U_I((unsigned)&ai_OptionHistorie) == 0)
        d2_RadioSelect(grp, 2);
    else if (U_I((unsigned)&ai_OptionHistorie) == 1)
        d2_RadioSelect(grp, 0);
    else if (U_I((unsigned)&ai_OptionHistorie) == 2)
        d2_RadioSelect(grp, 1);
    run = sv_RunFrame((int)main_Unk536980, 0xc6);
    do {
        if (run == 0) {
            d2_RadioClear(grp);
            d2_CloseView(f);
            if (ret == 0) {
                U_I((unsigned)&ai_GameFlags) = 0;
                return 0;
            }
            U_I((unsigned)&ai_GameFlags) |= 8;
            return ret;
        }
        d2_MenuNavigate(grp);
        if (U_I(((unsigned)&dm_L + 0x20)) != 0 || *(char *)(unsigned)&kb_Last == 1)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) == 0 || *(char *)(unsigned)&kb_Last == 0x1c || again) {
            if (back)
                goto lback;
            if (ai_Unk5fe710 == b0)
                ai_Unk4b1f90(1);
            else if (ai_Unk5fe710 == b1)
                ai_Unk4b1f90(2);
            else if (ai_Unk5fe710 == b2 || *(char *)(unsigned)&kb_Last == 0x1c)
                ai_Unk4b1f90(0);
            U_I((unsigned)&ai_OptionHistorie) = *(unsigned char *)(unsigned)&hst_TextsOn;
            if (U_I((unsigned)&ai_OptionHistorie) == 0)
                d2_RadioSelect(grp, 2);
            else if (U_I((unsigned)&ai_OptionHistorie) == 1)
                d2_RadioSelect(grp, 0);
            else if (U_I((unsigned)&ai_OptionHistorie) == 2)
                d2_RadioSelect(grp, 1);
            d2_SetViewEnabled(f, 0);
            hist = ai_ChooseMission();
            d2_SetViewEnabled(f, 1);
lback:
            d2_SetViewEnabled(f, 0);
            again = 0;
            if (hist != 0xff || back) {
                U_I((unsigned)&full_data_75e5ec) = hist;
                if (!back) {
                    U_I((unsigned)&sv_FrameMask) = 0x12d6;
                    v = ai_Unk4e2bf0("Startmenu/B_Persoenliches.esc");
                    ai_Unk4e2c70(v);
                }
                back = 0;
                ret = main_Unk5361d0(0xc6, 0);
                if (ret == 0) {
                    U_I((unsigned)&sv_FrameMask) = 0x12d6;
                    v = ai_Unk4e2bf0("Startmenu/A_Stadtwahl.esc");
                    ai_Unk4e2c70(v);
                    again = 1;
                } else {
                    r = ((int (*)(int))main_Unk536d80)(0);
                    if (r == 1) {
                        U_I((unsigned)&d2_CutsceneActive) = r;
                        U_I((unsigned)&full_data_624938) = sv_OpenMessageBox(U_I((unsigned)&full_data_1440da4), 0);
                        if (main_Unk5359f0(0) == 0) {
                            d3if_SelectScene(0, 0);
lback2:
                            back = 1;
                        } else {
lok:
                            ret = 1;
                            U_I((unsigned)&sv_PopRequest) = 1;
                        }
                    } else if (r == 0) {
                        ret = ai_Unk58b880(0xc6);
                        if (ret != 0) {
                            p = (int *)gm_Unk503ee0(loc, *(unsigned char *)((unsigned)&init_PlayerProfile + 0x3));
                            U_I((unsigned)&full_data_75e5f0) = *p;
                            *(unsigned char *)(unsigned)&full_data_75e5f4 = (unsigned char)*(unsigned short *)(p + 1);
                            h = unk_Unk4fb770("patrizier_MANN2");
                            U_I((unsigned)&full_data_75e628) = h + 0x639;
                            goto lok;
                        }
                        d3if_SelectScene(0, 0);
                        ret = 0;
                        again = 1;
                    } else if (r == -1) {
                        ret = 0;
                        goto lback2;
                    }
                }
            }
            d2_SetViewEnabled(f, 1);
        } else {
            if (back)
                goto lback;
            again = 0;
            if (U_I((unsigned)&d2_ClickedId) == 1)
                U_I((unsigned)&sv_PopRequest) = U_I((unsigned)&d2_ClickedId);
        }
        run = sv_RunFrame((int)main_Unk536980, 0xc6);
    } while (1);
}
