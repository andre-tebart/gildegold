/* 0x53ccf0, the program entry (WinMain: instance, previous, command line, show): reads game.INI (Network / Gfx / Sound / General), parses the command line (STADT=" BERUF=" IP=" PORT=" MO_MONEY FULLSCREEN DIRECTWINDOW), starts the engine
   (0x5324f0 / 0x531ba0 / 0x531c50 / 0x532160), then the game loop 0x53bb20 until the game ends (ROUGH) */

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE prev, char *cmd, int show)
{
    char mode;
    int net = 0;
    int sv = 0;
    int r, i, k;
    char *p, *q;
    char *s, *e;
    char *d;
    unsigned res;
    char c224[4];
    short s220;
    char c21c[8];
    char num[0x80];
    OSVERSIONINFOA vi;
    char bm[0x100];

    U_I((unsigned)&main_Instance) = (int)hInst;
    vi.dwOSVersionInfoSize = 0x94;
    GetVersionExA(&vi);
    U_I((unsigned)&main_PlatformNT) = (vi.dwPlatformId != 1);
    GetModuleFileNameA((HMODULE)U_I((unsigned)&main_Instance), (char *)(unsigned)&ai_OptionsFile, 0x104);
    i = 0;
    do {
        p = (char *)(unsigned)&ai_OptionsFile + i;
        *((char *)(unsigned)&main_AppDir + i) = *p;
        i++;
    } while (*p != 0);
    p = strrchr((char *)(unsigned)&main_AppDir, 0x5c);
    *p = 0;
    p = strrchr((char *)(unsigned)&ai_OptionsFile, 0x5c);
    strcpy(p + 1, "game.INI");
    he_Unk5755c0((char *)(unsigned)&ai_OptionsFile);
    GetPrivateProfileStringA("Network", "Server", "Server\\Server.dll", (char *)((unsigned)&ai_DataPath + 0x100), 0x104, (char *)(unsigned)&ai_OptionsFile);
    i = 0;
    do {
        p = (char *)((unsigned)&ai_DataPath + 0x100) + i;
        *((char *)((unsigned)&ls_SavePath + 0x160) + i) = *p;
        i++;
    } while (*p != 0);
    p = strrchr((char *)((unsigned)&ls_SavePath + 0x160), 0x5c);
    *p = 0;
    _strupr(cmd);
    U_I((unsigned)&main_TextureDivider) = GetPrivateProfileIntA("Gfx", "TextureDivider", 0, (char *)(unsigned)&ai_OptionsFile);
    U_I((unsigned)&snd_MusicEnabled) = GetPrivateProfileIntA("Sound", "msx", 0, (char *)(unsigned)&ai_OptionsFile);
    U_I((unsigned)&full_data_624584) = GetPrivateProfileIntA("Sound", "ambient", 0, (char *)(unsigned)&ai_OptionsFile);
    snd_SfxEnabled = GetPrivateProfileIntA("Sound", "sfx", 0, (char *)(unsigned)&ai_OptionsFile);
    U_I((unsigned)&full_data_6f2580) = GetPrivateProfileIntA("Sound", "weather", 1, (char *)(unsigned)&ai_OptionsFile);
    GetPrivateProfileStringA("General", "Bildmodus", "FULLSCREEN", bm, 0x100, (char *)(unsigned)&ai_OptionsFile);
    _strupr(bm);
    mode = (strcmp(bm, "DIRECTWINDOW") != 0) * 2 + 1;
    if (strstr(cmd, "DIRECTWINDOW") != 0)
        mode = 1;
    if (strstr(cmd, "FULLSCREEN") != 0)
        mode = 3;
    GetPrivateProfileStringA("General", "GfxPath", "\\project\\gfx\\", (char *)(unsigned)&main_GfxPath, 0x104, (char *)(unsigned)&ai_OptionsFile);
    GetPrivateProfileStringA("General", "GamePath", "\\project\\game\\", (char *)(unsigned)&main_GamePath, 0x104, (char *)(unsigned)&ai_OptionsFile);
    GetPrivateProfileStringA("General", "MoviePath", "\\project\\movie\\", (char *)(unsigned)&full_data_624798, 0x104, (char *)(unsigned)&ai_OptionsFile);
    GetPrivateProfileStringA("General", "Language", "german", (char *)(unsigned)"german", 0x40, (char *)(unsigned)&ai_OptionsFile);
    ls_LanguageOf((char *)(unsigned)"german");
    U_I((unsigned)&full_data_6f2574) = GetPrivateProfileIntA("General", "show_intro", 0, (char *)(unsigned)&ai_OptionsFile);
    res = GetPrivateProfileIntA("Gfx", "cur_res", 0, (char *)(unsigned)&ai_OptionsFile);
    k = (res & 0xff) * 8;
    U_I((unsigned)&ai_ScreenModeW) = U_I((unsigned)&ai_ScreenModes + k);
    U_I((unsigned)&ai_ScreenModeH) = U_I(((unsigned)&ai_ScreenModes + 0x4) + k);
    *(char *)(unsigned)&ai_GfxResolution = (char)res;
    if (strstr(cmd, "MO_MONEY") != 0)
        U_I((unsigned)&main_MoneyCheat) = 1;
    GetPrivateProfileStringA("General", "Stadt", "Augsburg", (char *)((unsigned)&ai_DataPath + 0x220), 0x40, (char *)(unsigned)&ai_OptionsFile);
    GetPrivateProfileStringA("Network", "Host", "128.0.0.1", (char *)(unsigned)&main_NetHost, 0x80, (char *)(unsigned)&ai_OptionsFile);
    U_I(((unsigned)&ls_SavePath + 0x84)) = GetPrivateProfileIntA("Network", "Port", 0x1d6b, (char *)(unsigned)&ai_OptionsFile);
    r = strstr(cmd, "STADT=\"");
    if (r != 0) {
        s = (char *)(r + 7);
        d = (char *)((unsigned)&ai_DataPath + 0x220);
        e = (char *)strchr(s, 0x22);
        if (e != 0)
            for (; s < e; s++)
                *d++ = *s;
        *d = 0;
    }
    r = strstr(cmd, "BERUF=\"");
    if (r != 0) {
        s = (char *)(r + 7);
        d = (char *)(unsigned)&full_data_6f2470;
        e = (char *)strchr(s, 0x22);
        if (e != 0)
            for (; s < e; s++)
                *d++ = *s;
        *d = 0;
    }
    r = strstr(cmd, "IP=\"");
    if (r == 0) {
        r = 0;
    } else {
        s = (char *)(r + 4);
        d = (char *)(unsigned)&main_NetHost;
        e = (char *)strchr(s, 0x22);
        if (e != 0)
            for (; s < e; s++)
                *d++ = *s;
        *d = 0;
        r = 1;
        net = 1;
    }
    i = strstr(cmd, "PORT=\"");
    if (i != 0) {
        s = (char *)(i + 6);
        d = num;
        e = (char *)strchr(s, 0x22);
        if (e != 0 && s < e) {
            k = e - s;
            memcpy(d, s, k);
            d += k;
        }
        *d = 0;
        U_I(((unsigned)&ls_SavePath + 0x84)) = atoi(num);
    }
    i = main_CreateMutex(c224);
    if (i != 0)
        exit(1);
    i = main_CreateWindow(hInst, show, mode);
    if (i == 0) {
        main_Exit();
        return 0;
    }
    i = main_Init();
    if (i == 0) {
        main_Exit();
        return 0;
    }
    if (U_I((unsigned)&full_data_6f2574) != 0)
        main_Unk53c9b0();
    i = main_OpenScreen(mode);
    if (i == 0) {
        main_Exit();
        return 0;
    }
    i = main_OpenEngine();
    if (i == 0) {
L_fail:
        main_Exit();
        return 0;
    }
    do {
        if (U_I((unsigned)&full_data_6f2434) == 0) {
            if (r == 0) {
                main_Unk533850();
                if (main_ScreenRestart != 0) {
                    main_ShutdownScreen();
                    sv_PumpMessages();
                    i = main_OpenScreen(mode);
                    if (i == 0)
                        goto L_fail;
                    main_ScreenRestart = r;
                    sv_PumpMessages();
                }
            } else {
                U_I((unsigned)&ai_GameFlags) = 5;
            }
        } else {
            r = 0x38;
            if (*(char *)(unsigned)&full_data_6f2470 != 0) {
                i = 0;
                do {
                    k = _strcmpi((char *)(unsigned)&full_data_6f2470, ((char **)(unsigned)&ai_Unk143a680)[i]);
                    r = i;
                    if (k == 0)
                        break;
                    i++;
                    r = 0x38;
                } while (i < 0x5f);
            }
            U_I((unsigned)&ai_GameFlags) = 9;
            *(char *)(unsigned)&init_PlayerProfile = (char)util_Random(0x5f) + 1;
            *(char *)((unsigned)&init_PlayerProfile + 0x1) = (char)util_Random(0x5f) + 1;
            *(char *)((unsigned)&init_PlayerProfile + 0x3) = (char)r;
            *(char *)((unsigned)&init_PlayerProfile + 0x2) = 0x12;
            *(short *)((unsigned)&init_PlayerProfile + 0x4) = 0x59a;
            *(char *)((unsigned)&init_PlayerProfile + 0x8) = 0;
            *(char *)((unsigned)&init_PlayerProfile + 0x9) = 1;
            i = unk_Unk4fb770("patrizier_MANN2");
            U_I((unsigned)&full_data_75e628) = i + 0x639;
            strcpy((char *)((unsigned)&init_PlayerProfile + 0xa), "Tobias");
            strcpy((char *)((unsigned)&init_PlayerProfile + 0x2a), "Severin");
            q = (char *)gm_Unk503ee0(c21c, *(char *)((unsigned)&init_PlayerProfile + 0x3));
            s220 = *(short *)(q + 4);
            *(char *)(unsigned)&full_data_75e5f4 = (char)s220;
            *(int *)(unsigned)&full_data_75e5f0 = (*(int *)q & ((unsigned)&txt_Keys + 0x181bf)) | 0xa8000000;
            *(int *)(unsigned)&full_data_75e5f0 &= 0xffff0000;
            ai_Unk4b1f90(1);
            r = net;
        }
        U_I((unsigned)&ai_LeaveGame) = 1;
        if (U_I((unsigned)&ai_GameFlags) != 0 && U_I((unsigned)&full_data_6f25b4) == 0) {
            do {
                main_GameLoop(0);
            } while (U_I((unsigned)&ai_LeaveGame) != 0);
        }
        if (U_I((unsigned)&full_data_6f2434) != 0 || r != 0)
            U_I((unsigned)&full_data_6f25b4) = 1;
        U_I((unsigned)&sv_PopRequest) = 0;
        U_I((unsigned)&sv_FrameDepth) = 0;
        if (U_I((unsigned)&full_data_6f25b4) != 0) {
            main_Exit();
            if (U_I((unsigned)&main_ScreenSaverOff) != 0)
                main_SetScreenSaver(1);
            return 1;
        }
    } while (1);
}
