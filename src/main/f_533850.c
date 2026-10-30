/* 0x533850, "main_Menue": the main menu ("MENU\MAIN_MENU"): opens the city scene and the player selection scene "scenes/*spielerauswahl.ed3", builds the 9 buttons (0x5337f0: new game 0x537930, tutorial, load a ".INI"
   city / network save (debug builds of the data: "Create Save with INI" / "Create Save for Network" / "Check Textfile"), load game 0x5d4080, network 0x5335b0, options 0x5d5fd0 / 0x5d6580 / 0x5d6950, credits
   0x5d7ba0, quit), starts one of three menu songs (0x423a90) and runs the screen loop 0x5309a0 until it ends; ROUGH */

float d3_VecAngle();
int sv_RegisterNamePlate();

void main_Unk533850(void)
{
    unsigned char *fade;
    int r, f, i;
    char cv;
    int bnew, bload, btut, bnet, bopt3, bcred, bquit, bopt1, bopt2, bini, bnetsave, bcheck;
    int grp;
#ifdef REA_START
    int rea_entry;
    int rea_delay;                         /* frames to let the menu run before the entry is taken */
    char *rea_want;
#endif
    char again;
    char c1[260], c2[260], c3[260], c4[260];
    char name1[0x60], name2[0x60];
    char *song;
    int *p;

    bopt1 = -1;
    bopt2 = -1;
    bini = -1;
    bnetsave = -1;
    bcheck = -1;
    sv_PumpMessages();
    di_GetMouse();
    *(char *)(unsigned)&kb_Last = 0;
    evt_ResetNoGroup();
    fade = (unsigned char *)d2_StartFade(0, 0, (int)*(short *)(unsigned)&d2_ScreenW, (int)*(short *)(unsigned)&d2_ScreenH, "BLACK", 0, 1);
    while ((*fade & 4) == 0)
        sv_RunFrame(0, 0x24087);
    U_I((unsigned)&d2_CutsceneActive) = 1;
    d3if_SelectScene(0, 0);
    d3if_Reset();
    song = ls_GetLanguage() == 0 ? "scenes/*ChooseCity.ed3" : "scenes/*ChooseCity_Europa.ed3";
    d3_LoadScene(song, 0, 0);
    sv_PumpMessages();
    d3if_SelectScene(1, 0);
    d3if_Reset();
    cv = d3_LoadScene("scenes/*spielerauswahl.ed3", 0, 0);
    if (cv == 0)
        err_Message("main_Menue:Could not load 3D-scene...");
    U_I((unsigned)&sv_FrameBase) = U_I((unsigned)&sv_FrameDepth) + 1;
    U_I((unsigned)&ai_GameFlags) = 0;
    memset((char *)(unsigned)&init_PlayerProfile, 0, 0x8c);
    U_I((unsigned)&ai_Unkfb9060) = *(short *)(unsigned)&d2_ScreenW;
    U_I((unsigned)&ai_Unkfb906c) = *(short *)(unsigned)&d2_ScreenH;
    U_I((unsigned)&d2_CutsceneActive) = 0;
    U_I((unsigned)&ai_Unkfb9068) = 0;
    U_I((unsigned)&ai_Unkfb9064) = 0;
    d2_ClearScreen();
    d2_ShowPicture(0x797);
    again = 0;
    f = d2_OpenForm(0, 0, "MENU\\MAIN_MENU");
    d2_PlaceMainWindow(f, 3);
    d2_SetForm(f, 0);
    bnew = main_Unk5337f0(0, U_I((unsigned)&full_data_144134c), d2_LastWindow);
    bload = main_Unk5337f0(1, U_I((unsigned)&ai_TextLoadGame), d2_LastWindow);
    btut = main_Unk5337f0(2, U_I((unsigned)&full_data_1449e24), d2_LastWindow);
    bnet = main_Unk5337f0(3, U_I((unsigned)&full_data_1441350), d2_LastWindow);
    bopt3 = main_Unk5337f0(4, U_I((unsigned)&ai_TextGameOptions), d2_LastWindow);
    bcred = main_Unk5337f0(7, U_I((unsigned)&full_data_1441354), d2_LastWindow);
    bquit = main_Unk5337f0(8, U_I((unsigned)&full_data_1441358), d2_LastWindow);
    if (U_I((unsigned)&full_data_6f245c) == 0) {
        bopt1 = main_Unk5337f0(5, U_I((unsigned)&ai_TextGfxOptions), d2_LastWindow);
        bopt2 = main_Unk5337f0(6, U_I((unsigned)&ai_TextSfxOptions), d2_LastWindow);
        grp = d2_CreateRadioGroup(9, bnew, bload, btut, bnet, bopt3, bopt1, bopt2, bcred, bquit);
    } else {
        bini = main_Unk5337f0(4, (int)"Create Save with INI", d2_LastWindow);
        bnetsave = main_Unk5337f0(5, (int)"Create Save for Network", d2_LastWindow);
        bcheck = main_Unk5337f0(6, (int)"Check Textfile", d2_LastWindow);
        grp = d2_CreateRadioGroup(9, bnew, bload, btut, bnet, bopt3, bini, bnetsave, bcred, bquit);
    }
    if (U_I((unsigned)&snd_MusicEnabled) != 0) {
        r = rand() % 3;
        if (r == 0)
            song = "cd1\\Rittersleut.mp3";
        else if (r == 1)
            song = "cd1\\MauerUndTor.mp3";
        else if (r == 2)
            song = "cd2\\KraeuterUndPhiolen.mp3";
        else
            goto lskip;
        U_I((unsigned)&full_data_6f2418) = msx_Play(song, 1);
    }
lskip:
    d2_ReleaseFade(fade);
    d2_StartFade(0, 0, (int)*(short *)(unsigned)&d2_ScreenW, (int)*(short *)(unsigned)&d2_ScreenH, "BLACK", 0x5a, 10);
    *(char *)(unsigned)&kb_Last = 0;
    msx_SetFadeTime(2000);
#ifdef REA_START
    /* REA_START=1 (tools/build.py, `uv run play|dbg --start NAME`): take one main-menu entry without a mouse
       click, so a start-up path is reachable without the GUI automation (build/dbg_tutorial.ps1). The entry
       then runs the same code a click runs (the loop below); the names are the buttons' own. A click cannot
       happen before the menu has run once: on the zero-th pass the menu callback is not registered yet, so
       the pop request the entry sets (sv_PopRequest) would not be consumed and would leak into the game
       loop, which then exits after its first frame. Wait one pass, as a real click does. */
    rea_entry = -1;
    rea_delay = 1;
    rea_want = getenv("REA_START");
    if (rea_want != 0 && *rea_want != 0) {
        if (strcmp(rea_want, "new") == 0)
            rea_entry = bnew;
        else if (strcmp(rea_want, "load") == 0)
            rea_entry = bload;
        else if (strcmp(rea_want, "tutorial") == 0)
            rea_entry = btut;
        else if (strcmp(rea_want, "network") == 0)
            rea_entry = bnet;
        else if (strcmp(rea_want, "gameoptions") == 0)
            rea_entry = bopt3;
        else if (strcmp(rea_want, "gfxoptions") == 0)
            rea_entry = bopt1;
        else if (strcmp(rea_want, "sfxoptions") == 0)
            rea_entry = bopt2;
        else if (strcmp(rea_want, "credits") == 0)
            rea_entry = bcred;
        else if (strcmp(rea_want, "quit") == 0)
            rea_entry = bquit;
        else if (strcmp(rea_want, "ini") == 0)
            rea_entry = bini;
        else if (strcmp(rea_want, "netsave") == 0)
            rea_entry = bnetsave;
        else if (strcmp(rea_want, "checktext") == 0)
            rea_entry = bcheck;
    }
#endif
    do {
        d2_MenuNavigate(grp);
#ifdef REA_START
        if (rea_entry != -1 && rea_delay-- <= 0) {
            ai_Unk5fe710 = rea_entry;      /* what a click on that button leaves behind */
            dm_L.lclick = 1;
            rea_entry = -1;                /* one shot: the options screens return to this loop */
        }
#endif
        if (again != 0) {
            d2_ShowPicture(0x797);
            again = 0;
        }
        if (U_I(((unsigned)&dm_L + 0x18)) != 0 && ai_Unk5fe710 != -1) {
            if (ai_Unk5fe710 == bnew) {
                d2_EnableObject(f, 0);
                d3io_NoopCallback();
                hmt_Reset();
                if (main_Unk537930() == 0)
                    goto lback;
                U_I((unsigned)&ai_GameFlags) |= 1;
                U_I((unsigned)&sv_PopRequest) = 1;
            } else if (ai_Unk5fe710 == btut) {
                d2_EnableObject(f, 0);
                if (ai_Unk57bce0() == 0) {
lback:
                    d2_EnableObject(f, 1);
                } else {
                    ai_Unk57bc30();
                    *(unsigned char *)(unsigned)&ai_NumPlayers = 1;
                    strcpy((char *)((unsigned)&ai_DataPath + 0x220), "Tutorial");
                    U_I((unsigned)&ai_GameFlags) = 0x89;
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            } else {
                if (ai_Unk5fe710 == bini) {
                    d2_EnableObject(f, 0);
                    if (ai_Unk5d34b0(0x24087, 1, "$Z$[Load INI$]", "gamedata/cities", ".INI", c3) != 0) {
                        sprintf(c1, (char *)(unsigned)"%s%s", "\\project\\gfx\\", c3);
                        GetPrivateProfileStringA("A - ALLGEMEIN", "Stadtname", "Unknown", name1, 0x60, c1);
                        main_StartCityGame((int)name1, 0);
                    }
                    goto lback;
                }
                if (ai_Unk5fe710 == bnetsave) {
                    d2_EnableObject(f, 0);
                    if (ai_Unk5d34b0(0x24087, 1, "$Z$[Load INI$]", "gamedata/cities", ".INI", c4) != 0) {
                        sprintf(c2, (char *)(unsigned)"%s%s", "\\project\\gfx\\", c4);
                        GetPrivateProfileStringA("A - ALLGEMEIN", "Stadtname", "Unknown", name2, 0x60, c2);
                        main_StartCityGame((int)name2, 1);
                    }
                    d2_EnableObject(f, 1);
                } else if (ai_Unk5fe710 == bload) {
                    d2_EnableObject(f, 0);
                    if (ai_LoadGameWindow() != 0)
                        U_I((unsigned)&sv_PopRequest) = 1;
                    d2_EnableObject(f, 1);
                } else if (ai_Unk5fe710 == bnet) {
                    d2_EnableObject(f, 0);
                    if (main_Unk5335b0() == 0) {
                        d2_EnableObject(f, 1);
                        U_I((unsigned)&ai_GameFlags) = 0;
                    } else
                        U_I((unsigned)&sv_PopRequest) = 1;
                } else if (ai_Unk5fe710 == bopt1) {
                    d2_EnableObject(f, 0);
                    ai_OptionsGfxScreen();
                    d2_EnableObject(f, 1);
                    again = 1;
                    if (main_ScreenRestart != 0)
                        U_I((unsigned)&sv_PopRequest) = 1;
                } else if (ai_Unk5fe710 == bopt2) {
                    d2_EnableObject(f, 0);
                    ai_OptionsSfxScreen();
                    d2_EnableObject(f, 1);
                } else if (ai_Unk5fe710 == bopt3) {
                    d2_EnableObject(f, 0);
                    ai_OptionsGameScreen();
                    d2_EnableObject(f, 1);
                } else if (ai_Unk5fe710 == bquit) {
                    U_I((unsigned)&ai_GameOver) = 1;
                    U_I((unsigned)&full_data_6f25b4) = 1;
                    U_I((unsigned)&sv_PopRequest) = 1;
                } else if (ai_Unk5fe710 == bcheck) {
                    main_Unk533740();
                } else if (ai_Unk5fe710 == bcred) {
                    d2_EnableObject(f, 0);
                    if (U_I((unsigned)&snd_MusicEnabled) != 0) {
                        msx_SetFadeTime(1000);
                        msx_Play("cd2\\ZumGutenEnd.mp3", 1);
                    }
                    ai_CreditsScreen();
                    d2_EnableObject(f, 1);
                    if (U_I((unsigned)&snd_MusicEnabled) != 0) {
                        msx_SetFadeTime(1000);
                        msx_Start(U_I((unsigned)&full_data_6f2418), 1);
                    }
                }
            }
        }
        if (*(char *)(unsigned)&kb_Last == 1) {
            U_I((unsigned)&full_data_6f25b4) = 1;
            U_I((unsigned)&sv_PopRequest) = 1;
        }
        r = sv_RunFrame((int)main_Unk533850, 0x24087);
        if (r == 0) {
            d2_RadioClear(grp);
            d2_CloseView(f);
            d2_ClearScreen();
            d3if_SelectScene(0, 0);
            d3if_Reset();
            d3if_SelectScene(1, 0);
            d3if_Reset();
            msx_SetFadeTime(1000);
            if (U_I((unsigned)&full_data_6f2418) != 0) {
                msx_Stop(U_I((unsigned)&full_data_6f2418), 1);
                U_I((unsigned)&d2_CutsceneActive) = 1;
                while (*(char *)(U_I((unsigned)&full_data_6f2418) + 0x104) != 0)
                    sv_RunFrame(0, U_I((unsigned)&sv_FrameMask));
                Sleep(1000);
            }
            msx_SetFadeTime(5000);
            return;
        }
    } while (1);
}
