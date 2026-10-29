/* 0x510e50 @deep behaviour: 87 code hunks, all read against 0x510e50..0x5112d0. Meaning fixes done: the game.INI literal is \game.INI (10 bytes: the original copies dword+dword+word at 0x510f48..0x510f60; the old 9-byte 'game.INI' gave a byte store), the player profile is the INIT_PLAYER view (status 0x02, job 0x03, coat 0x04 as an int, sex 0x08, faith 0x09, names 0x0a/0x2a: the original loads the dword at 0x75e5a4 and the two bytes at 0x75e5a8/0x75e5a9 separately, so the old unsigned short + >>8 pair was the meaning error; 183 -> 87 code hunks). Left: (1) the frame 0x310 against the original's 0x384 - 0x74 bytes of locals are missing between the scalars and ini (esp+0x24 against esp+0x98), every local below ini and the two returns' add esp shift; no statement in 0x510e50..0x5112d0 reads the missing region, so no source form reaches it. (2) init_StatusFormat's sprintf: the original's strrchr args are cleaned only at 0x510fb9 (add esp,0x34), ours cleans them at the strcpy (add esp,0x8 then 0x2c) - scheduling of the stack cleanup. (3) the start-money select: the original branches (cmp eax,edi / je / push 0x7a120 / jmp / push 0x7d0) where 'main_MoneyCheat == 0 ? 2000 : 500000' compiles neg/sbb/and/add; the if/else-into-a-local spelling was tried too (worse: 165 hunks, reverted) - VC6 constant-select codegen. (4) register allocation of the ai_NewRandomMission / init_SetupNewGame / d3if_SelectScene sites (regs/order hunks). */


/* 0x75e5a0, the player profile of the character creation: main_Unk5361d0 reads it from game.INI ([Network]
   Name, Familienname, Wappen, Geschlecht, Glauben) and writes the result of its stage 5, main_Unk53b840
   takes it for the start couple, 0x57bc30 is the default profile of the demo. Layout from those writers and
   from the registration message 0xc (0x4c46d0: short + 0xc, byte + 0xe, int + 0xf, sex + 0x13, faith + 0x14) */

/* 0x510e50, the start of a network game for the joining or hosting player (strings "%s/AUGSBURG.NET", "scenes/*network.ed3", "\\game.INI", section "Network"): waits for the flags in 0x6f2594 (1 = host, 4 = ready), loads the city file `name` (default "gamedata/cities/AUGSBURG.NET"), writes the player names to game.INI, registers the player profile (0x4c46d0), sets up the network scene, gives each player slot its start objects (0x4fec10 / 0x4ff450). Returns 1 when the city could not be loaded, 2 when 0x520b80 fails, else 0.
   The remaining difference is the frame (0x310 against the original's 0x384): 0x74 bytes of locals are missing, every local below ini is shifted. @deep frame job */
extern HINSTANCE main_Instance;          /* 0x75e140 */
extern unsigned ai_MissionSeed;           /* 0x75e120 */
extern signed char ai_MissionMode;        /* 0x624580 */
extern void *d2_ScreenSurf;                /* 0x6ed924 */

int init_StartNetworkGame(char *name)
{
    char c[0x200];
    char ini[0x104];
    unsigned flags;
    unsigned args[2];
    int h, r, i;
    int saved;
    INIT_CHAR *ch;
    char *p;
    short s;

    sprintf(c, (char *)init_StatusFormat, ai_NumPlayers);
    init_ShowStatus((int)c);
    cm_NetFlags = 0;
    do {
        sv_PumpMessages();
        sv_PumpNet();
    } while (cm_NetFlags == 0);
    if ((cm_NetFlags & 1) != 0) {
        if (*name == 0)
            sprintf(name, "%s/AUGSBURG.NET", "gamedata/cities");
        init_ShowStatus(init_StatusSendGame);
        if (ls_SendGame(name) == 0)
            return 1;
    }
    flags = cm_NetFlags & 1;
    init_ShowStatus(init_StatusReceiveGame);
    if (ls_ReceiveGame() == 0)
        return 2;
    GetModuleFileNameA(main_Instance, ini, 0x104);
    p = strrchr(ini, 0x5c);
    strcpy(p, "\\game.INI");
    saved = d3_CurrentScene;
    d2_CutsceneActive = 0;
    d3if_SelectScene(d3t_FindFreeScene(), 0);
    d3_LoadScene("scenes/*network.ed3", 0, 0);
    d2_SetViewEnabled(init_StatusForm, 0);
    d2_CutsceneActive = 1;
    d3if_Reset();
    d3if_SelectScene(saved, 0);
    d2_SetViewEnabled(init_StatusForm, 1);
    WritePrivateProfileStringA("Network", "Name", init_PlayerProfile.firstName, ini);
    WritePrivateProfileStringA("Network", "Familienname", init_PlayerProfile.familyName, ini);
    d2_ClearSurface((struct D2SURF *)d2_ScreenSurf);
    s = util_Random(2);
    d2_ShowPicture((s != 1 ? 3 : 0) + 0x78e);
    d2_DrawAll();
    if ((cm_NetFlags & 1) != 0) {
        args[0] = ai_NumPlayers;
        cm_SendExSysMessage(6, args);
    }
    h = cm_SendMessage0c(0xffff, 0xffff, init_PlayerProfile.status, init_PlayerProfile.job, init_PlayerProfile.coat + 0x59a, init_PlayerProfile.sex, init_PlayerProfile.faith, init_PlayerProfile.firstName, init_PlayerProfile.familyName);
    while (h == -1) {
        sv_PumpMessages();
        sv_PumpNet();
        h = cm_SendMessage0c(0xffff, 0xffff, init_PlayerProfile.status, init_PlayerProfile.job, init_PlayerProfile.coat + 0x59a, init_PlayerProfile.sex, init_PlayerProfile.faith, init_PlayerProfile.firstName, init_PlayerProfile.familyName);
    }
    r = cm_GetSlotState(h);
    while (r == 0) {
        sv_PumpMessages();
        sv_PumpNet();
        r = cm_GetSlotState(h);
    }
    ch = (INIT_CHAR *)cm_GetSlotValue(h);
    if ((cm_NetFlags & 1) != 0) {
        ai_MissionMode = -1;
        args[0] = 0xffffffff;
        ai_MissionSeed = timeGetTime();
        args[1] = ai_MissionSeed;
        cm_SendExSysMessage(9, args);
    }
    while (ai_MissionMode == -2) {
        sv_PumpMessages();
        sv_PumpNet();
    }
    if (ai_MissionMode != -1) {
        if (ai_MissionMode != 5)
            ai_MissionSeed = timeGetTime();
        ai_NewRandomMission(ch->netId, (int)ai_MissionMode, 1);
    }
    i = gm_PlayerFactor(main_MoneyCheat == 0 ? 2000 : 500000, ai_OwnPlayer);
    cm_SendMoveGoods(ch->netId, -1, ai_OwnPlayer, i);
    while ((cm_NetFlags & 4) == 0) {
        sv_PumpMessages();
        sv_PumpNet();
    }
    if (flags != 0) {
        init_ShowStatus(init_StatusSetupGame);
        init_SetupNewGame(ai_OwnPlayer);
    }
    sv_NetworkSyncWait(0x24087);
    init_ShowStatus(init_StatusLocalData);
    init_LocalData();
    sv_NetworkSyncWait(0x24087);
    if (flags != 0) {
        init_FillMarkets(ai_OwnPlayer);
        i = 0;
        p = ai_PlayerSlots[0];
        do {
            if (*p == 0)
                break;
            gm_SyncMarket(i);
            gm_RandomMarketShake(i);
            p += 0x2f4;
            i++;
        } while ((int)p < INIT_PLAYER_SLOTS_END);
        cm_SetSpeedLevel(2);
    }
    saved = d3_CurrentScene;
    d2_CutsceneActive = 0;
    if (ai_MissionMode > -1 && ai_MissionMode <= 5) {
        d3if_SelectScene(d3t_FindFreeScene(), 0);
        d3_LoadScene("scenes/*network.ed3", 0, 0);
        ai_ShowMissions(ch->netId);
        d3if_Reset();
        d3if_SelectScene(saved, 0);
    }
    return 0;
}
