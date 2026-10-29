/* 0x4b5c20, the mission complete dialog of the player's running mission: a mission of the next kind (only for
   kinds 0..3; message 0x0a, then 0x4b5360), the end of the missions, or quitting (0x4b5950) */

void ai_MissionNextDialog(int id)
{
    int msg1[0x1f];
    int msg2[0x1f];
    AICHAR *c;
    G13MIS *nm;
    G13TYPE *t;
    int next, again, quit, o1, o2, o3, view, flags, h, i;

    o1 = -1;
    next = -1;
    again = 0;
    quit = 0;
    t = 0;
    c = &ai_Chars[ai_PlayerId];
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
    view = d2_OpenForm(0, 0, "special\\mission");
    d2_CentreViewWindows(view);
    d2_SetForm(view, 1);
    io_PrintText((char *)0x1b94, c->id);
    if (next > -1 && next < 4)
        o1 = d2_GetWinObj(view, 1, io_PrintText((char *)0x1b96, 0x1b98));
    o2 = d2_GetWinObj(view, 1, io_PrintText((char *)0x1b96, 0x1b99));
    o3 = d2_GetWinObj(view, 1, io_PrintText((char *)0x1b96, 0x1b9a));
    d2_ClickedId = -1;
    flags = id | 0x80;
    do {
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == o1) {
                again = 1;
                sv_PopRequest = 1;
            } else if (d2_PointerObj == o2) {
                ai_MissionMode = -1;
                msg2[0] = c->netId;
                msg2[4] = 0;
                h = ((int (*)(int, int))cm_SendExSysMessage)(0x0a, msg2);
                while (!cm_GetSlotState(h))
                    sv_PumpNet();
                sv_PopRequest = 1;
            } else if (d2_PointerObj == o3) {
                ai_MissionMode = -1;
                msg1[0] = c->netId;
                quit = 1;
                msg1[4] = 1;
                h = ((int (*)(int, int))cm_SendExSysMessage)(0x0a, msg1);
                while (!cm_GetSlotState(h))
                    sv_PumpNet();
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame((void (*)(void))ai_MissionNextDialog, flags));
    d2_CloseView(view);
    if (again) {
        nm = ai_NewRandomMission(c->netId, next, 1);
        msg1[0] = c->netId;
        msg1[4] = 0;
        h = ((int (*)(int, int))cm_SendExSysMessage)(0x0a, msg1);
        while (!cm_GetSlotState(h))
            sv_PumpNet();
        ai_MissionInfoDialog(nm->type, id);
        return;
    }
    if (quit)
        ai_QuitGame(id);
}
