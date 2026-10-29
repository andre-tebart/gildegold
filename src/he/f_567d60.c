/* 0x567d60, a Ghidra miss: the run handler of the tutorial event 0x85 (night walk of the player's characters; only with the tutorial flag 0x6f2438, else -1): state 0 (sound on 0x6f25ac) sets the kind byte of
   the player record (+2) to 5; state 1 before 22:00 collects the characters of the player (0x501190 (1, 4, id) / 0x501030, at most 0x40, plus 0x6f1bc8) and shows the speech bubble 0x521960 (one of them, with 1 of 4
   the name 0x501b50 / 0x501ce0 + 2), next run +random(2) hours, random(0x3c) minutes; from 22:00 the next state; state 2 sets the kind byte to 6 and frees (ROUGH) */

void he_Unk567d60(int ev)
{
    int n, k, o, r, pick;
    int list[0x41];

    n = 0;
    if (U_I((unsigned)&ai_Unk6f2438) == 0)
        *(int *)(ev + 0x70) = -1;
    switch (*(int *)(ev + 0x70)) {
    case 0:
        if (*(char *)(unsigned)&ai_GameState == 0)
            return;
        *(char *)(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(ev + 8) * 0x21c) = 5;
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        return;
    case 1:
        if (*(unsigned short *)(unsigned)&ai_TimeHour >= 0x16) {
            *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
            return;
        }
        o = gm_BuildQuery(1, 4, *(unsigned short *)(ev + 8));
        while (o != 0 && n < 0x40) {
            list[n++] = o;
            o = gm_BuildNext();
        }
        list[n++] = U_I((unsigned)&ai_MarketBuilding);
        pick = list[(unsigned short)util_Random(n)];
        if ((unsigned short)util_Random(4) != 0 && pick != U_I((unsigned)&ai_MarketBuilding)) {
            r = gm_BuildingMainObject(pick);
            if (r == 0)
                r = ((void *(*)(unsigned char *))gm_GetGebaeudeHauptraum)(pick);
            if (r != 0)
                sv_SelectPanelObject(pick, *(int *)(r + 2), (unsigned)&ch_EmptyName);
            else
                sv_SelectPanelObject(pick, -1, (unsigned)&ch_EmptyName);
        } else {
            sv_SelectPanelObject(pick, -1, (unsigned)&ch_EmptyName);
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        *(unsigned short *)(ev + 0x56) += (unsigned short)util_Random(2);
        *(int *)(ev + 0x58) = (unsigned short)util_Random(0x3c);
        return;
    case 2:
        *(char *)(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(ev + 8) * 0x21c) = 6;
        he_Unk53e1f0((int *)ev);
        return;
    case -2:
    case -1:
        he_Unk53e1f0((int *)ev);
        return;
    default:
        return;
    }
}
