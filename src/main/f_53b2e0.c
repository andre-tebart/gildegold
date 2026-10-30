/* 0x53b2e0, starts the player's round: resets the per-player flags, the round intro screen, the world set-up calls (each followed by Sleep(100)), the fanfare dialog (ROUGH) */

void ord_InitMessageRecord(int a, float b, float c, float d);
int sv_MessageBox();

void main_Unk53b2e0(void)
{
    char sv = *(char *)(unsigned)&snd_AmbientOn;
    int cur = -1;
    int i, n, t;
    unsigned *pf;
    unsigned char *pb;
    char *pc;
    int *pi;
    unsigned short r;
    int w;
    char s130[12];
    int a124[9];
    char buf[256];

    ord_CountGridObjects();
    i = 0;
    pf = (unsigned *)((unsigned)&ai_Selected + 0x40);
    do {
        ai_Unk4e5bf0(pf - 0x72);
        pf[0] &= 0xe0874703;
        pf[1] &= 0x3d31ffc7;
        if (*((char *)pf - 0x1c6) == 6)
            cur = i;
        pf += 0x87;
        i++;
    } while ((int)pf < ((unsigned)((unsigned)ai_Selected + 0x6527a) + 0x1c6));
    *(short *)(unsigned)&ai_PlayerId = (short)cur;
    he_Unk53ee40();
    if ((U_I((unsigned)&ai_GameFlags) & 4) == 0 && (r = util_RandomInt(10)) > 6) {
        n = main_Unk53b830();
        while (n != 1) {
            U_I((unsigned)&d2_CutsceneActive) = 0;
            U_I((unsigned)&sv_FrameMask) = 0x24087;
            txt_Format(buf, 0x79);
            n = sv_MessageBox(buf, 1);
            if (n == 0) {
                U_I((unsigned)&ai_GameOver) = 1;
                sv_PopTo(main_GameLoop);
                return;
            }
            d2_DrawAll();
            Sleep(1000);
            n = main_Unk53b830();
        }
    }
    U_I((unsigned)&sv_FrameMask) = 0x24087;
    if ((U_I((unsigned)&ai_GameFlags) & 4) != 0)
        sv_NetworkSyncWait(0x24087);
    ai_Unk4e36a0(0);
    if ((char)ai_Unk4e4f80(cur) != 0 && U_I((unsigned)&full_data_624560) != 0) {
        U_I((unsigned)&ai_Unkfb9060) = (int)*(short *)(unsigned)&d2_ScreenW;
        U_I((unsigned)&ai_Unkfb906c) = (int)*(short *)(unsigned)&d2_ScreenH;
        U_I((unsigned)&ai_Unkfb9068) = 0;
        U_I((unsigned)&ai_Unkfb9064) = 0;
        d2_ClearSurface(U_I((unsigned)&d2_ScreenSurf));
        d2_ShowPicture(0x79d);
    }
    he_Unk53eea0();
    he_Unk53ef50();
    if (U_I((unsigned)&full_data_624560) != 0) {
        w = d2_OpenForm(0, 0, "Runden\Spielerrunde_Beginn_NEU");
        d2_PlaceMainWindow(w, 3);
        d2_SetForm(w, 1);
        io_PrintText(0x7d);
        gm_ClockToDate((unsigned)&ai_Time, s130);
        d2_SetForm(w, 0);
        d2_CloseView(w);
    }
    pb = (unsigned char *)gm_BuildQuery(1, 6);
    while (pb != 0) {
        if (*pb == 0x1e) {
            i = init_BuildingSceneUniverse(pb);
            t = U_I((unsigned)&d3_CurrentScene);
            if (i != -1) {
                d3if_SelectScene((int)*(char *)((unsigned)&init_BuildingUniverse + *(unsigned char *)(U_I((unsigned)&ai_BTypes) + *pb * 0x24d)), 1);
                d3_InflateScene(3);
                veg_GrowPlants(*(int *)(pb + 0x71));
                d3_DeflateScene(3);
                d3if_SelectScene(t, 1);
            }
        } else if (*(char *)(U_I((unsigned)&ai_BTypes) + *pb * 0x24d) == 7) {
            pb[0x69] = 0;
            pb[0x6a] = 0;
            pb[0x6b] = 0;
            pb[0x6c] = 0;
        }
        pb = (unsigned char *)gm_BuildNext();
    }
    i = 0;
    gm_UpdatePlayers();
    unk_Unk4f88c0();
    if ((U_I((unsigned)&cm_NetFlags) & 8) == 0 && U_I((unsigned)&ai_Unk1460ae0) != -1) {
        unk_Unk4fabe0(2);
    } else {
        unk_Unk4f3700();
        sv_PumpNet();
        Sleep(100);
        pc = (char *)((unsigned)&ai_CharInts + 0x4);
        do {
            if (*(short *)(pc - 8) != -1 && *pc != 0)
                main_Unk53a520(i);
            pc += 0x21c;
            i++;
        } while ((int)pc < ((unsigned)((unsigned)ai_CharInts + 0x653fe) + 0x6));
        sv_PumpNet();
        Sleep(100);
        unk_Unk4fc920();
        sv_PumpNet();
        Sleep(100);
        unk_Unk4faa00();
        sv_PumpNet();
        Sleep(100);
        unk_Unk4fabe0(3);
        unk_Unk4fa580();
        sv_PumpNet();
        Sleep(100);
        unk_Unk4fa9b0();
        amt_CheckOffices();
        sv_PumpNet();
        Sleep(100);
        he_Unk540fe0();
        ((void (*)(void))d3io_NoopCallback)();  /* called with no argument (no push at the call) */
        sv_PumpNet();
        Sleep(100);
        unk_Unk4f4d10();
        sv_PumpNet();
        Sleep(100);
        ai_Unk495ee0();
        sv_PumpNet();
        Sleep(100);
        ord_UpdateStatistics();
        ord_GetMessageRecord(a124);
        if (a124[0] == -1)
            ord_InitMessageRecord(1, 1.0f, 0.5f, 10.0f - (float)(4 - U_I((unsigned)&ai_Difficulty)) * 2.5f);
        else
            ord_UpdateMessageEvent();
        he_Unk53f6b0();
        sv_PumpNet();
        Sleep(100);
        pi = (int *)(unsigned)&ai_CharInts;
        do {
            cm_SendRelations(-1, *pi, 0, 3);
            pi += 0x87;
        } while ((int)pi < ((unsigned)((unsigned)ai_CharInts + 0x653fe) + 0x2));
        cm_SendRelations(-1, -1, 0, 4);
        hmt_Unk4ad7a0();
    }
    pi = (int *)((unsigned)&ai_Selected + 0x64);
    do {
        *pi = 0;
        pi += 0x87;
    } while ((int)pi < ((unsigned)((unsigned)ai_Selected + 0x6527a) + 0x1ea));
    if (U_I((unsigned)&full_data_624560) != 0) {
        if (*(char *)((unsigned)&ai_GameState + 0x1) != 0 && (U_I((unsigned)&ai_GameFlags) & 4) == 0) {
            w = sv_Unk52fa10();
            d2_SetForm(w, 2);
            io_PrintText(0x7d);
            d2_SetForm(w, 1);
            io_PrintText(0x1ba0, U_I((unsigned)&ai_PlayerId) & 0xffff, U_I((unsigned)&ai_PlayerId) & 0xffff);
            snd_QueueVoice(-7, 0, "FanfarePositivHeldenhaftLang", -1, 0);
            i = sv_RunFrame((int)main_Unk53b2e0, 0x66087);
            while (i != 0) {
                if (U_I((unsigned)&d2_ClickedId) == 0)
                    U_I((unsigned)&sv_PopRequest) = 1;
                i = sv_RunFrame((int)main_Unk53b2e0, 0x66087);
            }
            d2_CloseView(w);
            main_Unk53cb40();
            U_I((unsigned)&ai_GameOver) = 1;
        }
        if (U_I((unsigned)&full_data_624560) != 0 && U_I((unsigned)&ai_GameOver) == 0) {
            if ((U_I((unsigned)&cm_NetFlags) & 8) == 0 && U_I((unsigned)&ai_Unk1460ae0) != -1)
                w = 0;
            else
                w = 1;
            ai_Unk4b3960(w);
        }
    }
    if ((U_I((unsigned)&cm_NetFlags) & 8) != 0 || U_I((unsigned)&ai_Unk1460ae0) == -1)
        main_Unk53a450();
    *(char *)(unsigned)&snd_AmbientOn = sv;
}
