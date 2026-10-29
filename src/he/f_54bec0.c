/* 0x54bec0, a Ghidra miss (disassembled; 1496 B): the run handler of the watch meeting ("Patrol": the six figures +0x8c .. walk to the place +0xac .. (0x4c52e0), +0xcc / +0xd0 the next place, +0xd4 the 0x4c3b10 result, +0xd8 the pending job): state 0 sends them, 1 waits until they are idle, 2 waits 0x18 units and then tries to pair the event with a running one of the type 0x48 of another player (0x53e400 (1, 0, 0x48), same +0xac text, other player): the pair gets a message record of 0x114 bytes (type 3, the two groups as lists, 0x4c4fa0 -> +0xd8) and state 3; else it is posted for the state 2 again (a position is sent to 0x541280); 3 waits for the job, 4 / 5 wait for 0x4e50a0 and send the figures to the next place (ROUGH) */

void he_Unk54bec0(int ev)
{
    char msg[0x114];
    int ebp = 0, i, p, st, o, e, h, r, found = 0;
    int ebx = -1;
    int tmpclock[4];
    int eax;

    st = *(int *)(ev + 0x70);
    if (st == -2) {
        if ((*(unsigned char *)(ev + 0x78) & 4) != 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        *(int *)(ev + 0x70) = 5;
        found = 1;
    }
    if (*(int *)(ev + 0x70) == -1) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    *(HETIME *)(ev + 0x52) = ai_Time;
    switch (*(int *)(ev + 0x70)) {
    case 0:
        for (i = 0; i < 6; i++) {
            p = gm_FindChar(*(int *)(ev + 0x8c + i * 4));
            if (p != 0 && *(int *)(p + 0x184) != 0) {
                sv_MoveToGroup(0, 0, *(unsigned short *)p, ev);
                ai_Unk4c5230(*(int *)(p + 4));
                ai_Unk4c52e0(*(int *)(p + 4), -1, -1, ev + 0xac, 1, "Patrol");
            }
        }
        gm_AddTime(ev + 0x52, 0, 5, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
        return;
    case 1:
        for (i = 0; i < 6; i++) {
            p = gm_FindChar(*(int *)(ev + 0x8c + i * 4));
            if (p != 0 && *(int *)(p + 0x184) != 0 && *(int *)(*(int *)(p + 0x184) + 0x128) != 0) {
                gm_AddTime(ev + 0x52, 0, 4, 0);
                *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
                return;
            }
        }
        *(HETIME *)(ev + 0x44) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 10, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(2, ev);
        return;
    case 2:
        *(HETIME *)(tmpclock) = *(HETIME *)(ev + 0x44);
        gm_AddTime((int)tmpclock, 0x18, 0, 0);
        if (gm_CompareTime((unsigned)&ai_Time, (int)tmpclock) > 0) {
            gm_AddTime(ev + 0x52, 0, 2, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(5, ev);
            return;
        }
        e = (int)he_FindEvent(1, 0, 0x48);
        while (e != 0) {
            if (strcmp((char *)(ev + 0xac), (char *)(e + 0xac)) == 0 && *(unsigned short *)(ev + 8) == *(unsigned short *)(e + 8)) {
                e = (int)he_NextEvent();
                continue;
            }
            break;
        }
        if (e != 0 && (short)util_Random(2) != 0 && *(unsigned short *)(e + 8) != *(unsigned short *)(ev + 8)) {
            int n1 = 0, n2 = 0;

            memset(msg, 0, sizeof(msg));
            msg[8] = 3;
            msg[0x26] = (char)0xa2;
            *(int *)(msg + 0x14) = ebx;
            msg[0x30] = 2;
            msg[0x7c] = 4;
            *(int *)(msg + 0xc) = U_I((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c);
            *(int *)(msg + 0x34) = U_I((unsigned)&ai_CharInts + *(unsigned short *)(e + 8) * 0x21c);
            *(int *)(msg + 0x38) = U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c);
            for (i = 0; i < 16; i++) {
                *(int *)(msg + 0xc0 + i * 4) = ebx;
                *(int *)(msg + 0x80 + i * 4) = ebx;
            }
            for (i = 0; i < 8 && n1 < 6; i++)
                if (*(int *)(e + 0x8c + i * 4) != ebx)
                    *(int *)(msg + 0x80 + n1++ * 4) = *(int *)(e + 0x8c + i * 4);
            for (i = 0; i < 6 && n2 < 6; i++)
                if (*(int *)(ev + 0x8c + i * 4) != ebx)
                    *(int *)(msg + 0xc0 + n2++ * 4) = *(int *)(ev + 0x8c + i * 4);
            *(int *)(msg + 0x100) = ebx;
            *(int *)(msg + 0x104) = ebx;
            *(int *)(msg + 0x108) = ebx;
            *(int *)(msg + 0x10c) = *(int *)(ev + 0xcc);
            *(HETIME *)(msg + 0x18) = ai_Time;
            gm_AddTime(msg + 0x18, 0, 0x1e, 0);
            *(int *)(ev + 0xd8) = ((int (*)(void *))ai_Unk4c4fa0)(msg);
            gm_AddTime(ev + 0x52, 0, 10, 0);
            *(int *)(ev + 0x70) = 3;
            *(int *)(ev + 0x84) = cm_SendExSetHE(3, ev);
            return;
        }
        gm_AddTime(ev + 0x52, 0, 10, 0);
        if (U_I((unsigned)&d3_CurrentScene) == 0 && (o = d3_FindObject(0, 0x140, 0, ev + 0xac)) != 0) {
            float pos[3];

            d3_PointToWorld(o, o + 0x4c, pos);
            he_Unk541280(ev, (int)pos);
        }
        *(int *)(ev + 0x84) = cm_SendExSetHE(2, ev);
        return;
    case 3:
        gm_AddTime(ev + 0x52, 0, 1, 0);
        if (cm_GetSlotState(*(int *)(ev + 0xd8)) == 0) {
            *(int *)(ev + 0x84) = cm_SendExSetHE(3, ev);
            return;
        }
        r = cm_GetSlotValue(*(int *)(ev + 0xd8));
        if (r == 0) {
            *(int *)(ev + 0x84) = cm_SendExSetHE(5, ev);
            return;
        }
        *(int *)(ev + 0xd4) = *(int *)r;
        *(int *)(ev + 0x84) = cm_SendExSetHE(4, ev);
        return;
    case 4:
        gm_AddTime(ev + 0x52, 0, 5, 0);
        h = cm_PartyById(*(int *)(ev + 0xd4));
        *(int *)(ev + 0x84) = cm_SendExSetHE(h == 0 ? 5 : 4, ev);
        return;
    case 5:
        for (i = 0; i < 6; i++) {
            p = gm_FindChar(*(int *)(ev + 0x8c + i * 4));
            if (p != 0 && *(int *)(p + 0x184) != 0) {
                sv_MoveToGroup(0, 0, *(unsigned short *)p, 0);
                ai_Unk4c5230(*(int *)(p + 4));
                ai_Unk4c52e0(*(int *)(p + 4), *(int *)(ev + 0xcc), *(int *)(ev + 0xd0), 0, 0, "Patrol");
            }
        }
        gm_AddTime(ev + 0x52, 0, 2, 0);
        if (found) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    default:
        return;
    }
}
