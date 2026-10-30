/* 0x4b6110, the single player's mission complete sequence: 0x4b5460, then the choice of the next mission
   (0x4b4280 from the next kind on): none (message 0x0a), cancel (message 0x0a with 1, then 0x4b5950) or the
   new mission (0x4b3ca0, message 0x0a) */

void main_Unk53cb40(void);                                                         /* 0x53cb40, defined elsewhere */

void ai_MissionCompleteSingle(int id)
{
    int msg[0x1f];
    AICHAR *c;
    G13TYPE *t;
    int next, r, h, i;

    t = 0;
    next = -1;
    c = &ai_CharsT[ai_PlayerId];
    for (i = 0; i < 0x80; i++) {
        if (ai_Missions[i].type != 0 && ai_Missions[i].owner == c->netId) {
            t = ai_GetMissionType(ai_Missions[i].type);
            next = t->kind + 1;
            memset(&ai_Missions[i], 0, sizeof(G13MIS));
            break;
        }
    }
    id = (id & ~9) | 0x100;
    ai_MissionSuccessDialog(t, id);
    main_Unk53cb40();
    r = ai_ChooseMissionFrom(next);
    if (r == 0) {
        ai_MissionMode = -1;
        msg[0] = c->netId;
        msg[4] = 0;
        h = ((int (*)(int, int))cm_SendExSysMessage)(0x0a, msg);
        while (!cm_GetSlotState(h))
            sv_PumpNet();
        return;
    }
    if (r == 0xff) {
        ai_MissionMode = -1;
        msg[0] = c->netId;
        msg[4] = 1;
        h = ((int (*)(int, int))cm_SendExSysMessage)(0x0a, msg);
        while (!cm_GetSlotState(h))
            sv_PumpNet();
        ai_QuitGame(id);
        return;
    }
    ai_NewMission(c->netId, r, 1);
    msg[0] = c->netId;
    msg[4] = 0;
    h = ((int (*)(int, int))cm_SendExSysMessage)(0x0a, msg);
    while (!cm_GetSlotState(h))
        sv_PumpNet();
}
