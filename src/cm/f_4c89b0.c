/* 0x4c89b0, handler of type 0x20 (system message): data[0] is the kind (0..0x14) */

int ai_Unk4c89b0(G17MSG *m, G17STAT *s)
{
    G17MSG msg = *m;
    char buf[0x80];
    int ret = 1;
    int i, k;
    char *p, *q;
    int t[4];
    short t4;
    int save;

    switch (msg.data[0]) {
    case 0:
        t[0] = 0;
        t[1] = G17_I(msg, 1);
        t[2] = 0;
        t[3] = 0;
        t4 = 0;
        util_Seed(G17_I(msg, 5));
        sv_Unk525510(t, 1);
        break;
    case 2:
        ai_Unkf31144 = 1;
        if (G17_I(msg, 1) != 0)
            cm_NetFlags = (cm_NetFlags & ~0x10) | 8;
        else
            cm_NetFlags = (cm_NetFlags & ~8) | 0x10;
        break;
    case 3:
        p = tb_IsFunctionOff(sv_UpdateClock);
        if (p == 0)
            tb_SetFunctionOff(sv_UpdateClock, 1);
        *(int *)ai_Time = G17_I(msg, 1);
        *(short *)&ai_Time[4] = G17_S(msg, 5);
        *(int *)&ai_Time[6] = G17_I(msg, 7);
        *(int *)&ai_Time[10] = G17_I(msg, 0xb);
        *(G17T14 *)ai_Unk75e130 = *(G17T14 *)ai_Time;
        if (p == 0)
            tb_SetFunctionOff(sv_UpdateClock, 0);
        break;
    case 4:
        p = gm_BuildQuery(1, 1, G17_I(msg, 1));
        if (p != 0) {
            q = gm_Query(*(int *)(p + 0x5d), 1, 1, G17_I(msg, 5));
            if (q != 0)
                init_ShowBuildingScene(p, *(unsigned short *)q);
            else
                init_ShowBuildingScene(p, 0xfd);
        }
        break;
    case 5:
        util_Seed(G17_I(msg, 1));
        ai_Unkf83650 = 0;
        break;
    case 6:
        ai_NumPlayers = msg.data[1];
        break;
    case 7:
        ai_NetPlayer = G17_I(msg, 1);
        break;
    case 8:
        if (ai_MissionPartner == -1) {
            k = G17_I(msg, 1);
            if (gm_FindChar(k) != 0) {
                ai_MissionPartner = k;
                break;
            }
        }
        ret = 2;
        break;
    case 9:
        ai_MissionModeB = msg.data[1];
        ai_MissionSeed = G17_I(msg, 5);
        break;
    case 10:
        if (ai_MissionPartner == G17_I(msg, 1)) {
            ai_MissionPartner = -1;
            if (G17_I(msg, 0x11) == 1)
                ai_GameOver = 1;
            if (ai_MissionModeB == 5)
                ai_Unk4b3bd0(ai_CharInts[ai_PlayerId][0], G17_I(msg, 0x21));
        } else
            ret = 2;
        break;
    case 12:
        save = d3_CurrentScene;
        d3if_SelectScene(0, 1);
        d3sm_Build(*(int *)(ai_Unk5fb5b0 + 0xb0), ai_Unk633738I);
        d3if_SelectScene(save, 1);
        ai_Unk631084 = 0;
        break;
    case 13:
        sv_Unk528390(G17_I(msg, 1));
        break;
    case 15:
        cm_RequestedSaveSlot = G17_I(msg, 1);
        i = 0;
        do {
            cm_RequestedSaveName[i] = msg.data[5 + i];
            i++;
        } while (msg.data[4 + i] != 0);
        break;
    case 16:
        for (i = 0; i < 8; i++) {
            if (cm_SaveAcks[i][0] == -1) {
                cm_SaveAcks[i][0] = G17_I(msg, 1);
                cm_SaveAcks[i][1] = G17_I(msg, 5);
                break;
            }
        }
        break;
    case 17:
        if (ai_PlayerId != 0xffff) {
            if (ai_CharInts[ai_PlayerId][0x82] != G17_I(msg, 1) || G17_I(msg, 1) == -1)
                ai_Unk6f0cd4++;
        }
        break;
    case 18:
        k = G17_I(*m, 1);
        if (k >= 4)
            k = 4;
        cm_SpeedLevel = k;
        ai_Unk60bd80 = ai_Unk5edc10[k];
        sv_SetStatusText(ai_Unk14413c8[k]);
        ai_ApplyOptions();
        break;
    case 19:
        ai_Unk60bd94 = G17_I(msg, 1);
        break;
    case 20:
        hmt_SetPendingElection(G17_I(msg, 1));
        break;
    }
    sprintf(buf, "cm_ExSysMessage() type: %i", msg.data[0]);
    if (s != 0) {
        s->unk01 = 0;
        s->state = ret;
        s->value = 0;
    }
    return 0;
}
