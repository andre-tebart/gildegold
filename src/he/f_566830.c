/* 0x566830, the run handler of the attack event with up to eight attackers (the player ids at +0x8c .. +0xab, -1 = free; the target building +0xac, the attacker building +0xb0; a Ghidra miss, 3568 bytes with the jump
   table 0x5675f4 (states -2 .. 5); ROUGH skeleton: states 0 .. 3 follow the disassembly, 4 / 5 (the fight result: loot 0x4c47e0 / 0x4c4880, the news 0x19e7 / 0x19e9 / 0x1a85 / 0x1a86 through 0x53dee0, the
   cases 0x4c3ba0 / 0x4c3d00 (4, 1, 0x65 / 0x69) and 0x4c49a0) are summarised): the end handler (-1 / -2) releases the attackers (0x4c5230, 0x4c52e0 (id, target, leader, 0, 0, 0x627d74), 0x524260 (0, 0, id, 0)) and
   frees; state 0 sends every attacker to the building (0x524260 (b, id, 0, ev), 0x4c5230, 0x4c52e0) and waits 4 units; state 1 waits until every attacker with a figure has arrived (0x524770 (p, b)); state 2 starts the
   fight scene (0x553ae0 on a record {b, 5000.0f, ev, 0}, 0x4f2e30 (0x17, 1, owner, +4, b + 1); +0xb4 / +0xb8 / +0xbc = the scene, the opponent, its value) or, without a scene, ends the attack with a random
   sound text (0x4c5410 from the three names of the table 0x62689c / 0x627d8c) and state 5; state 3 waits for the scene (0x4c3ae0) and the opponent event (0x53e400 (1, 1, +0xb8), day +0xd4 == +4) and then
   hands the scene result (0x4c3b10, 0x4c4ed0) over; state 4 (waits 6 units while the opponent event runs) and state 5 settle the result (ROUGH) */

void he_Unk566830(int ev)
{
    int st, b, p, i, id, lead, e, r, idx, owner;
    int *slot;
    int rec[4];
    char names[3][0x40];
    char text[0x200];

    memset(names, 0, sizeof(names));
    lead = 0;
    st = *(int *)(ev + 0x70);
    switch (st) {
    case -2:
    case -1:
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0xb0));
        if (b != 0)
            lead = ((void *(*)(unsigned char *))gm_GetGebaeudeHauptraum)(b);
        for (i = 0, slot = (int *)(ev + 0x8c); i < 8; i++, slot++) {
            if (*slot == -1)
                continue;
            p = gm_FindChar(*slot);
            if (p == 0)
                continue;
            ai_Unk4c5230(*slot);
            if (b != 0 && lead != 0)
                ai_Unk4c52e0(*slot, *(int *)(b + 1), *(int *)(lead + 2), 0, 0, (unsigned)&full_data_627d74);
            else
                ai_Unk4c52e0(*slot, -1, -1, 0, 0, (unsigned)&full_data_627d74);
            sv_MoveToGroup(0, 0, *(unsigned short *)p, 0);
        }
        he_Unk53e1f0((int *)ev);
        return;
    case 0:
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
        if (b == 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        for (i = 0, slot = (int *)(ev + 0x8c); i < 8; i++, slot++) {
            if (*slot == -1)
                continue;
            p = gm_FindChar(*slot);
            if (p == 0)
                continue;
            sv_MoveToGroup(b, *(unsigned short *)p, 0, ev);
            ai_Unk4c5230(*slot);
            ai_Unk4c52e0(*slot, *(int *)(b + 1), -1, 0, 1, (unsigned)&full_data_627d74);
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 4, 0);
        *(int *)(ev + 0x70) = 1;
        return;
    case 1:
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
        for (i = 0, slot = (int *)(ev + 0x8c); i < 8; i++, slot++) {
            if (*slot == -1)
                continue;
            p = gm_FindChar(*slot);
            if (p != 0 && *(int *)(p + 0x184) != 0 && sv_Unk524770(p, b) == 0) {
                *(HETIME *)(ev + 0x52) = ai_Time;
                gm_AddTime(ev + 0x52, 0, 4, 0);
                return;
            }
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 0, 1);
        *(int *)(ev + 0x70) = 2;
        return;
    case 2:
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
        if (b != 0) {
            idx = *(unsigned short *)(b + 0x27);
            rec[0] = b;
            rec[1] = 0x451c4000;
            rec[2] = ev;
            rec[3] = 0;
            if (he_Unk553ae0(rec) != 0) {
                *(int *)(ev + 0xb4) = unk_Unk4f2e30(0x17, 1, *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(int *)((unsigned)&ai_Chars + idx * 0x21c + 4), *(int *)(b + 1));
                *(int *)(ev + 0xb8) = *(int *)(rec[3] + 4);
                *(int *)(ev + 0xbc) = *(int *)(rec[3] + 0xb0);
                *(int *)(ev + 0x70) = 3;
                *(HETIME *)(ev + 0x52) = ai_Time;
                return;
            }
        }
        for (i = 0, slot = (int *)(ev + 0x8c); i < 8; i++, slot++) {
            if (*slot == -1)
                continue;
            p = gm_FindChar(*slot);
            if (p == 0)
                continue;
            sv_MoveToGroup(b, *(unsigned short *)p, 0, ev);
            ai_Unk4c5230(*slot);
            ai_Unk4c52e0(*slot, *(int *)(b + 1), -1, 0, 0, (unsigned)&full_data_627d74);
            ((void (*)(int, const char *))ai_Unk4c5410)(*slot, names[(unsigned short)((int (*)(int, int))util_Random)(3, 1)]);
        }
        *(int *)(ev + 0x70) = 5;
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 0xf, 0);
        return;
    case 3:
        if (*(int *)(ev + 0xb4) != -1 && cm_GetSlotState(*(int *)(ev + 0xb4)) == 0)
            return;
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
        e = (int)he_FindEvent(1, 1, *(int *)(ev + 0xb8));
        if (b != 0 && e != 0 && *(int *)(e + 0xd4) == *(int *)(ev + 4) && *(int *)(ev + 0xb4) != -1) {
            p = gm_FindChar(*(int *)(ev + 0xb8));
            r = cm_GetSlotValue(*(int *)(ev + 0xb4));
            if (p != 0 && r != 0)
                cm_SendProofExecuted(*(int *)r, *(int *)(p + 4));
            *(int *)(ev + 0x70) = 4;
            gm_AddTime(ev + 0x52, 0, 10, 0);
            return;
        }
        *(int *)(ev + 0x70) = -1;
        return;
    case 4:
        e = (int)he_FindEvent(1, 1, *(int *)(ev + 0xb8));
        if (e == 0) {
            *(int *)(ev + 0x70) = -1;
            return;
        }
        if (*(int *)(e + 0xd4) == *(int *)(ev + 4)) {
            gm_AddTime(ev + 0x52, 0, 6, 0);
            return;
        }
        /* the fight result: loot from the opponent building to the attackers, the news of the owner / attacker, the cases
           of the attackers released (ROUGH, see the header) */
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
        owner = gm_FindChar(*(unsigned short *)(ev + 8) * 0 + *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c));
        if (b != 0 && owner != 0) {
            ((void (*)(int))gm_PlayerFactor)(0x226 + (unsigned short)((int (*)(int, int))util_Random)(0xc8, U_I((unsigned)&ai_OwnPlayer)));
            txt_Format(text, 0x19ea, b, 0, 0);
            he_ShowNews(*(int *)(owner + 4), *(int *)(owner + 4), text, 1, 0x5f0, (unsigned)&full_data_627a10);
        }
        for (i = 0, slot = (int *)(ev + 0x8c); i < 8; i++, slot++) {
            if (*slot == -1)
                continue;
            p = gm_FindChar(*slot);
            if (p != 0)
                gm_AddHealth(*(unsigned short *)p, -100 - (unsigned short)util_Random(100));
        }
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0xb0));
        if (b == 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        lead = ((void *(*)(unsigned char *))gm_GetGebaeudeHauptraum)(b);
        for (i = 0, slot = (int *)(ev + 0x8c); i < 8; i++, slot++) {
            if (*slot != -1) {
                ai_Unk4c5230(*slot);
                ai_Unk4c52e0(*slot, *(int *)(b + 1), *(int *)(lead + 2), 0, 0, (unsigned)&full_data_6279f0);
            }
        }
        he_Unk53e1f0((int *)ev);
        return;
    case 5:
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
        owner = gm_FindChar(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c));
        if (b != 0 && owner != 0) {
            txt_Format(text, 0x19e7, *(unsigned short *)((unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c), 0x19c1, b);
            he_MessageBoxQuickjump(*(int *)(owner + 4), *(int *)(owner + 4), text, 1, 0x5f0, *(int *)(b + 1), -1, 0, (unsigned)&ch_EmptyName);
        }
        *(int *)(ev + 0x70) = -2;
        return;
    default:
        he_Unk53e1f0((int *)ev);
        return;
    }
}
