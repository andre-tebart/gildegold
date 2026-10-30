/* 0x53b840, creates the two figures of the start couple (records of 0x70 / 0xbf random names) and links them (ROUGH) */

void main_Unk53b840(void)
{
    int a, b, k;
    char *t, *s;
    unsigned short *p6, *p7;
    int r6, r7;
    char *d;
    unsigned u;

    ai_ClearMissions();
    if (*(char *)((unsigned)&init_PlayerProfile + 0x2) != 0) {
        cm_BeginSendGroup();
        r6 = cm_SendMessage0c(-1, -1, 0x10, *(char *)((unsigned)&init_PlayerProfile + 0x3), *(char *)((unsigned)&init_PlayerProfile + 0x4), *(char *)((unsigned)&init_PlayerProfile + 0x8), *(char *)((unsigned)&init_PlayerProfile + 0x9), ((unsigned)&init_PlayerProfile + 0xa), ((unsigned)&init_PlayerProfile + 0x2a));
        cm_EndSendGroup();
        a = cm_SendGroupState();
        while (a == 0) {
            sv_PumpNet();
            a = cm_SendGroupState();
        }
        a = cm_GetSlotValue(r6);
        cm_BeginSendGroup();
        r6 = cm_SendCreateCharacter(9, -1, -1, util_RandomInt(4) + 0x20, -1, *(char *)((unsigned)&init_PlayerProfile + 0x1), 0, 1);
        r7 = cm_SendCreateCharacter(9, -1, -1, util_RandomInt(4) + 0x20, -1, *(char *)(unsigned)&init_PlayerProfile, 0, 0);
        cm_EndSendGroup();
        b = cm_SendGroupState();
        while (b == 0) {
            sv_PumpNet();
            b = cm_SendGroupState();
        }
        p6 = (unsigned short *)cm_GetSlotValue(r6);
        p7 = (unsigned short *)cm_GetSlotValue(r7);
        strncpy((char *)(p7 + 0x20), (char *)((unsigned)&init_PlayerProfile + 0x2a), 0x10);
        p6[0x28] = *(unsigned short *)(a + 0x50);
        *(int *)(p6 + 0x2a) = *(int *)(a + 0x54);
        *(int *)(p6 + 0x34) = *(int *)(a + 4);
        u = util_RandomInt(0x70);
        d = (char *)(p6 + 0x18);
        s = *(char **)((unsigned)&ai_Unk143af70 + (u & 0xffff) * 4);
        do { *d++ = *s; } while (*s++ != 0);
        unk_Unk4fb390(*p6);
        p7[0x28] = *(unsigned short *)(a + 0x50);
        *(int *)(p7 + 0x2a) = *(int *)(a + 0x54);
        *(int *)(p7 + 0x34) = *(int *)(a + 4);
        u = util_RandomInt(0xbf);
        d = (char *)(p7 + 0x18);
        s = *(char **)((unsigned)&ai_Unk143ac5c + (u & 0xffff) * 4);
        do { *d++ = *s; } while (*s++ != 0);
        unk_Unk4fb390(*p7);
        *(int *)(p6 + 0x2e) = *(int *)(p7 + 2);
        *(int *)(p7 + 0x2e) = *(int *)(p6 + 2);
        *(int *)(a + 0x60) = *(int *)(p7 + 2);
        *(int *)(a + 100) = *(int *)(p6 + 2);
        *(int *)(p6 + 0x34) = *(int *)(a + 4);
        t = (char *)(unsigned)&full_data_75e5f5;
        *(int *)(p7 + 0x34) = *(int *)(a + 4);
        do { t[a - (unsigned)&full_data_75e401] = *t; } while (*t++ != 0);
        *(int *)(a + 0x18c) = U_I((unsigned)&full_data_75e628);
        cm_SendRelations(*(int *)(a + 4), *(int *)(p7 + 2), 0x7f, 0);
        cm_SendRelations(*(int *)(p7 + 2), *(int *)(a + 4), 0x7f, 0);
        cm_SendRelations(*(int *)(a + 4), *(int *)(p6 + 2), 0x7f, 0);
        cm_SendRelations(*(int *)(p6 + 2), *(int *)(a + 4), 0x7f, 0);
        cm_SendRelations(*(int *)(p7 + 2), *(int *)(p6 + 2), 0x7f, 0);
        cm_SendRelations(*(int *)(p6 + 2), *(int *)(p7 + 2), 0x7f, 0);
        u = util_RandomInt(0x200);
        cm_SendMoveGoods(*(int *)(p7 + 2), -1, *(unsigned char *)(unsigned)&ai_OwnPlayer, ((u & 0xffff) + 500) * 0x20);
        u = util_RandomInt(0x200);
        k = ((u & 0xffff) + 500) * 0x20;
        cm_SendMoveGoods(*(int *)(p6 + 2), -1, *(unsigned char *)(unsigned)&ai_OwnPlayer, k);
        if (U_I((unsigned)&ai_MissionMode) >= 0)
            ai_NewMission(*(int *)(a + 4), U_I((unsigned)&full_data_75e5ec), 0);
        k = 0;
        do {
            *(char *)(a + 0x80 + k) = *(char *)((unsigned)&full_data_75e5f0 + k);
            k++;
        } while (k < 5);
    }
}
