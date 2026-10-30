/* 0x56ce40, the run handler of the type 0x4c (a character action of the player below 22 h): the player byte 0x624580 (negative = no player: free the record), the record of the player *(unsigned short *)(ev + 8) in the 0x80-entry table 0xf82320 (0x24 bytes: +4 the character's +4, +0x20 the handled flag), the states (-2 / -1 free) 0 = wait for the flag, 1 = start the action 0x4c4db0 (8, &the record's +4) and wait for its handle +0xac, 2 = the handle result (1 = done, state 0 and wait 24; else state 1 and wait 30) */

int ai_Unk4b4f00();
void cm_SendExSysMessage(unsigned char id, void *msg);

void he_Unk56ce40(int ev)
{
    int i, h, arg;

    if (*(signed char *)(unsigned)&ai_MissionMode < 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    for (i = 0; i < 0x80; i++) {
        if (ai_Missions[i].type != 0 && ai_Missions[i].owner == *(int *)((unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c + 4))
            break;
    }
    if (i >= 0x80) {
        gm_AddTime(ev + 0x52, 0, 0x1e, 0);
        return;
    }
    switch (*(int *)(ev + 0x70)) {
    case -2:
    case -1:
        he_Unk53e1f0((int *)ev);
        break;
    case 0:
        if (ai_Missions[i].unk20 == 0 && *(unsigned short *)(unsigned)&ai_TimeHour < 0x16) {
            if (ai_Unk4b4f00(&ai_Missions[i]) != 0) {
                ai_Missions[i].unk20 = 1;
                gm_AddTime(ev + 0x52, 0, 1, 0);
                *(int *)(ev + 0x70) = 1;
                return;
            }
        }
        gm_AddTime(ev + 0x52, 0, 0xa, 0);
        break;
    case 1:
        gm_AddTime(ev + 0x52, 0, 1, 0);
        if (ai_Missions[i].unk20 == 1 && *(unsigned short *)(unsigned)&ai_TimeHour < 0x16 &&
            U_I((unsigned)&ai_MissionPartner) == -1) {
            arg = ai_Missions[i].owner;
            *(int *)(ev + 0xac) = ((int (*)(unsigned char, void *))cm_SendExSysMessage)(8, &arg);
            *(int *)(ev + 0x70) = 2;
            return;
        }
        gm_AddTime(ev + 0x52, 1, 0, 0);
        *(int *)(ev + 0x70) = 1;
        break;
    case 2:
        h = cm_GetSlotState(*(int *)(ev + 0xac));
        gm_AddTime(ev + 0x52, 0, 1, 0);
        if (h == 0)
            return;
        if (h == 1) {
            *(int *)(ev + 0x70) = 0;
            gm_AddTime(ev + 0x52, 0x18, 0, 0);
            return;
        }
        *(int *)(ev + 0x70) = 1;
        gm_AddTime(ev + 0x52, 0, 0x1e, 0);
        break;
    }
}
