/* 0x5112d0, the start of a network game: the file names "gamedata/network/%s.SAV" / ".SRV" of the game n, the waiting loop for the flags in 0x6f2594, loading the save (0x51c950), the local data (0x511ad0). Returns 1 when the server file could not be read, else 0. ROUGH */


int unk_Unk5112d0(int n)
{
    int r;
    char a[0x104], b[0x104], c[0x200];

    sprintf(a, "gamedata/network/%s.SAV", (char *)n);
    sprintf(c, (char *)init_StatusFormat, ai_NumPlayers);
    init_ShowStatus((int)c);
    cm_NetFlags = 0;
    do {
        sv_PumpMessages();
        sv_PumpNet();
    } while (cm_NetFlags == 0);
    if ((cm_NetFlags & 1) != 0) {
        sprintf(b, "gamedata/network/%s.SRV", (char *)n);
        init_ShowStatus(init_StatusSendGame);
        r = ls_SendGame(b);
        if (r == 0)
            return 1;
        if ((cm_NetFlags & 1) != 0) {
            int args[31];

            args[0] = ai_NumPlayers;
            cm_SendExSysMessage(6, args);
        }
    }
    while ((cm_NetFlags & 4) == 0) {
        sv_PumpMessages();
        sv_PumpNet();
    }
    unk_Unk51c950(a, 5);
    sv_NetworkSyncWait(0x24087);
    init_ShowStatus(init_StatusLocalData);
    init_LocalData();
    d2_CutsceneActive = 0;
    return 0;
}
