/* 0x54dec0, a Ghidra miss (disassembled; 1680 B): the run handler of the street event of the town (+0xb0 / +0xb4 floats = the chance and its step, +0xac the number of cases done, +0xb8 the job handle of the 0x4c4650 (0xff, .., 0xc) task): state 0 clears the counters (+0xb0 *= 0) and posts state 1; state 1 reads the time (0x4f9e00), derives the chance (+0xb0 = 1 - the fraction of the day passed, +0xb4 = the day step) and goes on to state 2 / 6 (a day over: the sign of the chances is dropped), state 2 and 7 wait (+0x58 counts, the minute word +0x56 = 0xf), state 3 / 6 (random against +0xb0): a random player figure (0x300 ring, kind 0, no boss) gets a 0x4fca40 (-2000) or a task is posted (0x4c4650), state 4 waits for the task (0x4c3ae0 / 0x4c3b10) and gives a random figure an inn visit (0x4f8890 and 0x4c5540), 0x4c4830 pays 0x33 + random(50) * 0x20 (ROUGH) */

void he_Unk54dec0(int ev)
{
    int st, i, n, rec, id, found, h;
    float f;
    int tm[8];

    st = *(int *)(ev + 0x70);
    if (st == -1 || st == -2) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    switch (st) {
    case 0:
        *(int *)(ev + 0xb4) = 0;
        *(int *)(ev + 0xac) = 0;
        *(float *)(ev + 0xb0) = *(float *)(ev + 0xb0) * 0.0f;
        gm_AddTime(ev + 0x52, 0, 5, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
        return;
    case 1:
        unk_Unk4f9e00(tm);
        f = (float)(tm[6] - tm[3]) / (float)(*(int *)(ev + 0xac) + 2);
        *(float *)(ev + 0xb0) = 1.0f - f;
        *(float *)(ev + 0xb4) = f;
        gm_AddTime(ev + 0x52, 0, 1, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(2, ev);
        return;
    case 2:
        *(float *)(ev + 0xb0) = (2.0f - 0.0f) * *(float *)(ev + 0xb0);
        *(float *)(ev + 0xb0) = (*(float *)(ev + 0xb0) + 1.0f) * *(float *)(ev + 0xb0);
        *(float *)(ev + 0xb0) = (*(float *)(ev + 0xb0) < 0.0f ? -*(float *)(ev + 0xb0) : *(float *)(ev + 0xb0));
        *(float *)(ev + 0xb4) = (*(float *)(ev + 0xb4) < 0.0f ? -*(float *)(ev + 0xb4) : *(float *)(ev + 0xb4));
        *(int *)(ev + 0x84) = cm_SendExSetHE(6, ev);
        return;
    case 3:
        if (cm_GetSlotState(*(int *)(ev + 0xb8)) == 0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 1, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(3, ev);
            return;
        }
        if (cm_GetSlotState(*(int *)(ev + 0xb8)) == 2) {
            gm_AddTime(ev + 0x52, 0, 1, 0);
            *(int *)(ev + 0xb8) = -1;
            *(int *)(ev + 0x84) = cm_SendExSetHE(6, ev);
            return;
        }
        h = cm_GetSlotValue(*(int *)(ev + 0xb8));
        id = (unsigned short)util_Random(2);
        found = 0;
        if (h == 0) {
            gm_AddTime(ev + 0x52, 0, 1, 0);
            *(int *)(ev + 0xb8) = -1;
            *(int *)(ev + 0x84) = cm_SendExSetHE(6, ev);
            return;
        }
        n = 0x100;
        i = (unsigned short)util_Random(0x100);
        while (found == 0 && n-- > 0) {
            int b = U_I((unsigned)&ai_Blds) + i * 0xa9;

            if (*(char *)(U_I((unsigned)&ai_BTypes) + *(unsigned char *)b * 0x24d) == 1 && ord_GetKindRecord(*(unsigned char *)b, tm) != 0 && *(unsigned char *)(b + 0x65) < *(unsigned short *)tm)
                found = b;
            i = (i + 1) & 0xff;
        }
        if (found != 0)
            cm_SendBoss(*(int *)(h + 4), *(int *)(found + 1));
        cm_SendMoveGoodsUnchecked(-1, *(int *)(h + 4), *(unsigned char *)(unsigned)&ai_OwnPlayer, ((unsigned short)util_Random(0x32) + 0x33) << 5);
        cm_BeginFieldBlock(h, *(int *)(h + 4));
        cm_AddFieldDelta(1, 1, 2, &id);
        cm_AddFieldDelta(1, 1, 0xc, &id);
        cm_SendFieldDeltas();
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 1, 0);
        *(int *)(ev + 0xac) = *(int *)(ev + 0xac) + 1;
        *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
        return;
    case 4:
    case 5:
        return;
    case 6:
        if (util_RandomF() < *(float *)(ev + 0xb0)) {
            n = 0x300;
            i = (unsigned short)util_Random(0x300);
            while (n-- > 0) {
                rec = (unsigned)&ai_Chars + i * 0x21c;
                if (*(unsigned short *)rec != 0xffff && *(unsigned char *)(rec + 8) != 0 && *(unsigned char *)(rec + 2) == 0 && *(int *)(rec + 0x16c) == 0) {
                    gm_AddHealth(i, -2000);
                    *(int *)(ev + 0xac) = *(int *)(ev + 0xac) + 1;
                    break;
                }
                i = (i + 1) % 0x300;
            }
            *(int *)(ev + 0xb8) = -1;
            gm_AddTime(ev + 0x52, 0, 0x1e, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
            return;
        }
        *(float *)(ev + 0xb0) = *(float *)(ev + 0xb0) - *(float *)(ev + 0xb4);
        gm_AddTime(ev + 0x52, 0, 1, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(6, ev);
        return;
    case 7:
        gm_AddTime(ev + 0x52, 0, 10, 0);
        if (*(unsigned short *)(ev + 0x56) >= 0x16) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x52) = *(int *)(ev + 0x52) + 1;
            *(int *)(ev + 0x58) = 0;
            *(short *)(ev + 0x56) = 6;
        } else if (*(unsigned short *)(ev + 0x56) < 0xf) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x58) = *(int *)(ev + 0x58) + 1;
        }
        *(int *)(ev + 0x84) = cm_SendExSetHE(7, ev);
        return;
    default:
        return;
    }
}
