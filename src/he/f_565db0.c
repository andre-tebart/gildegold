/* 0x565db0, the run handler of the kill-player event (ROUGH, 1472 bytes with the jump table 0x5662a8; the Ghidra function 0x565d10 stops before it: a Ghidra miss; the code at 0x566030 is a case of it, not a
   function): -1 / -2 free; while the player (0x505df0 (+0xac)) has +0x20c set (and the state is not 6) the next run is 5 units ahead; state 0: removes the figure object (+0x184: 0x429e90 (0x4284a0 (+0x28))), the lock
   0x524260 (0, 0, id, 0), with the flag +0x78 & 2 the news 0x42f4 for the opponent figure (+0x16c: 0x53dee0, 9 parameters), +0x16c = 0; state 1 waits until the figure is gone; state 2 lays the figure down (0x405610,
   0x40b2f0 (fig, 0x36, 1), the animation "bewegung/zu_boden_sinken") and waits 0x1e units (frees at once with +0x78 & 4); state 3: figure kinds 6 / 7 build the message 0x114 bytes (type 8, the clock, +0x36 = 0xa0,
   the player +4) for 0x4c4fa0 (+0xb4) and go to state 6, the others wait 2 and go to 4; state 4: 0x493ea0 finds the successor h (none: state 5), 0x506890 (p, h), kinds 5 / 6 / 7 whose +0xd level exceeds h's raise
   h (0x4c3ba0 / 0x4c3c00 (1, 1, 0xd, &level) / 0x4c49a0), 0x4c4e00 (+4, 0x5075c0 (p)), free; state 5: 0x506890 (p, 0x6f1bf8), 0x4c4e00, free; state 6 polls 0x4c3ae0 (+0xb4) (0: wait, 2: back to state 4, else +0xb0
   = *0x4c3b10 and state 7); state 7 waits for 0x4e50a0 (+0xb0) and the dead flag +8, frees or goes to state 4 */

void he_Unk565db0(int ev)
{
    int p, f, o, h, q, a, st, idx;
    unsigned char m[0x114];
    char text[0x200];
    char kind;

    st = *(int *)(ev + 0x70);
    if (st == -1) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    p = gm_FindChar(*(int *)(ev + 0xac));
    if (p == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (*(int *)(p + 0x20c) != -1 && st != 6) {
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 5, 0);
        return;
    }
    switch (st) {
    case 0:
        f = *(int *)(p + 0x184);
        if (f == 0) {
            if ((*(unsigned char *)(ev + 0x78) & 2) == 0) {
                he_Unk53e1f0((int *)ev);
                return;
            }
            *(int *)(ev + 0x70) = 3;
            return;
        }
        o = *(int *)(f + 0x28);
        if (o != -1)
            evt_Stop(evt_FindById(o));
        sv_MoveToGroup(0, 0, *(unsigned short *)p, 0);
        if ((*(unsigned char *)(ev + 0x78) & 2) != 0 && (q = *(int *)(p + 0x16c)) != 0) {
            idx = *(unsigned short *)(q + 0x27);
            kind = *(char *)(((unsigned)&ai_Chars + 0x2) + idx * 0x21c);
            if (kind == 6 || kind == 7) {
                txt_Format(text, 0x42f4, *(unsigned short *)p, q);
                he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + idx * 0x21c), *(int *)(q + 1), text, 0, 0x60a, *(int *)(q + 1), -1, (unsigned)&ch_EmptyName, (unsigned)&ch_EmptyName);
            }
        }
        *(int *)(p + 0x16c) = 0;
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        return;
    case 1:
        f = *(int *)(p + 0x184);
        if (f != 0 && *(int *)(f + 0x28) != -1)
            return;
        *(int *)(ev + 0x70) = st + 1;
        return;
    case 2:
        f = *(int *)(p + 0x184);
        if (f != 0) {
            ch_StopActions(f);
            a = ch_AddAction(*(int *)(p + 0x184), 0x36, 1);
            if (a != 0)
                strcpy((char *)(a + 0xf0), "bewegung/zu_boden_sinken");
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 0x1e, 0);
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
            he_Unk53e1f0((int *)ev);
        return;
    case 3:
        if (*(char *)(p + 2) != 6 && *(char *)(p + 2) != 7) {
            gm_AddTime(ev + 0x52, 0, 2, 0);
            *(int *)(ev + 0x70) = 4;
            return;
        }
        memset(m, 0, sizeof(m));
        m[0x18] = 8;
        m[0x36] = 0xa0;
        *(HETIME *)(m + 0x1c) = ai_Time;
        gm_AddTime(m + 0x1c, 0, 0x1e, 0);
        h = *(int *)(p + 4);
        *(int *)(m + 0x2c) = h;
        *(int *)(m + 0x28) = -1;
        m[0x44] = 1;
        *(int *)(m + 0x48) = h;
        *(int *)(ev + 0xb4) = ((int (*)(void *))ai_Unk4c4fa0)(m + 0x10);
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 1, 0);
        *(int *)(ev + 0x70) = 6;
        return;
    case 4:
        h = ai_Unk493ea0(p);
        if (h == 0) {
            *(int *)(ev + 0x70) = 5;
            return;
        }
        gm_Unk506890(p, h);
        kind = *(char *)(p + 2);
        if ((kind == 6 || kind == 7 || kind == 5) && *(unsigned char *)(p + 0xd) > *(unsigned char *)(h + 0xd)) {
            cm_BeginFieldBlock(h, *(int *)(h + 4));
            cm_AddFieldDelta(1, 1, 0xd, p + 0xd);
            cm_SendFieldDeltas();
        }
        cm_SendKillCharacter(*(int *)(p + 4), gm_Unk5075c0(p));
        he_Unk53e1f0((int *)ev);
        return;
    case 5:
        gm_Unk506890(p, U_I((unsigned)&ai_PlayerChar));
        cm_SendKillCharacter(*(int *)(p + 4), gm_Unk5075c0(p));
        he_Unk53e1f0((int *)ev);
        return;
    case 6:
        a = cm_GetSlotState(*(int *)(ev + 0xb4));
        if (a == 0)
            return;
        if (a == 2) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x70) = 4;
            return;
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        *(int *)(ev + 0xb0) = *(int *)cm_GetSlotValue(*(int *)(ev + 0xb4));
        gm_AddTime(ev + 0x52, 2, 0, 0);
        *(int *)(ev + 0x70) = 7;
        return;
    case 7:
        if (*(unsigned char *)(p + 8) != 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        if (cm_PartyById(*(int *)(ev + 0xb0)) == 0) {
            *(int *)(ev + 0xb4) = -1;
            *(int *)(ev + 0xb0) = -1;
            *(HETIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x70) = 4;
            return;
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 2, 0, 0);
        return;
    case -2:
        he_Unk53e1f0((int *)ev);
        return;
    default:
        return;
    }
}
