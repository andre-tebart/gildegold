/* 0x5461c0, a Ghidra miss (disassembled; 2646 B): the run handler of the raid ("Razzia") event: the target +0xac (a figure), the start +0xb0, the raiders are the figures of the slots +0x8c .. +0xa8; state 0 = the raiders go to the target (0x4c52e0 "Razzia"), 1 = they go to the start, 2 = wait until they are idle (+0x128 of the job record), then 3 = the outcome is rolled: a random number (0..0x4a) plus 100 * a chance (the 0x4ffe90 record 0xca of the start: 1.0 + byte +0x37 / 100, or 5.0 when 0x6f2448 is set) against 125 * (1 + 0.01 * 0x5092e0 (t, 5, 1)): a failed raid posts a message record of 0x114 bytes (type 3, the guards and the raiders as lists, 0x4c4fa0; state 4), a successful one reports the news 0x114b / 0x1c54 / 0x1c55 / 0x1c56 with the result of 0x4d3d10 (state 1); 4 = wait for the posted record (+0xb4), 5 = wait for 0x4e50a0 of its result (+0xb8); a free (state -1) sends the raiders to the start (ROUGH) */

int ai_Unk4d3d10();
int ai_Unk4d4050();
int cm_PartyById();
int cm_GetSlotValue(int h);
void gm_GetGebaeudeHauptraum(unsigned char *b);
unsigned short *gm_QueryNext(void);
int gm_BuildingSkill();

#define HE_REC(id) ((unsigned)&ai_Chars + (id) * 0x21c)

void he_Unk5461c0(int ev)
{
    char buf[0x100];
    char msg[0x114];
    int t, ta, r, i, g, p, o, ebx, edi, flag = 0, st, rec, mrec, n, busy;
    int rr;
    float prob = 1.0f, mult;

    st = *(int *)(ev + 0x70);
    if (st == -2) {
        if ((*(unsigned char *)(ev + 0x78) & 2) == 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        flag = 1;
        *(int *)(ev + 0x70) = 1;
    }
    if (*(int *)(ev + 0x84) != -1 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
        return;
    *(int *)(ev + 0x84) = -1;
    switch (*(int *)(ev + 0x70)) {
    case -1:
        if ((*(unsigned char *)(ev + 0x78) & 2) != 0) {
            t = gm_BuildQuery(1, 1, *(int *)(ev + 0xb0));
            o = ((void *(*)(unsigned char *))gm_GetGebaeudeHauptraum)(t);
            for (i = 0; i < 8; i++) {
                g = *(int *)(ev + 0x8c + i * 4);
                if (g != -1 && (p = gm_FindChar(g)) != 0) {
                    ai_Unk4c5230(g);
                    ai_Unk4c52e0(g, *(int *)(t + 1), *(int *)(o + 2), 0, 0, "Razzia");
                    sv_MoveToGroup(t, 0, *(unsigned short *)p, 0);
                }
            }
        }
        he_Unk53e1f0((int *)ev);
        return;
    case 0:
        if ((*(unsigned char *)(ev + 0x78) == 0) & 2)
            return;
        t = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
        if (t != 0 && *(unsigned short *)(t + 0x27) != 0xffff) {
            for (i = 0; i < 8; i++) {
                g = *(int *)(ev + 0x8c + i * 4);
                if (g != -1) {
                    p = gm_FindChar(g);
                    if (p != 0) {
                        sv_MoveToGroup(t, 0, *(unsigned short *)p, ev);
                        ai_Unk4c5230(g);
                        ai_Unk4c52e0(g, *(int *)(t + 1), -1, 0, 0, "Razzia");
                    } else {
                        *(int *)(ev + 0x8c + i * 4) = -1;
                    }
                }
            }
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 2, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(2, ev);
            return;
        }
        goto gone;
    case 1:
        if ((*(unsigned char *)(ev + 0x78) & 2) == 0)
            return;
        t = gm_BuildQuery(1, 1, *(int *)(ev + 0xb0));
        if (t == 0)
            goto gone;
        o = gm_Query(*(int *)(t + 0x5d), 1, 0, 0x146);
        for (i = 0; i < 8; i++) {
            g = *(int *)(ev + 0x8c + i * 4);
            if (g != -1) {
                p = gm_FindChar(g);
                if (p != 0 && *(unsigned char *)(p + 8) > 0) {
                    ai_Unk4c5230(g);
                    ai_Unk4c52e0(g, *(int *)(t + 1), *(int *)(o + 2), 0, 0, "Razzia");
                    sv_MoveToGroup(t, 0, *(unsigned short *)p, ev);
                } else {
                    *(int *)(ev + 0x8c + i * 4) = -1;
                }
            }
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 2, 0);
        if (flag) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    case 2:
        if ((*(unsigned char *)(ev + 0x78) == 0) & 2)
            return;
        busy = 1;
        for (i = 0; i < 8; i++) {
            g = *(int *)(ev + 0x8c + i * 4);
            if (g != -1 && (p = gm_FindChar(g)) != 0 && *(int *)(*(int *)(p + 0x184) + 0x128) != 0) {
                busy = 0;
                break;
            }
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 2, 0);
        if (busy)
            *(int *)(ev + 0x84) = cm_SendExSetHE(3, ev);
        else
            *(int *)(ev + 0x84) = cm_SendExSetHE(2, ev);
        return;
    case 3:
        if ((*(unsigned char *)(ev + 0x78) & 2) == 0)
            return;
        t = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
        rec = HE_REC(*(unsigned short *)(t + 0x27));
        mrec = HE_REC(*(unsigned short *)(ev + 8));
        r = gm_BuildingSkill(t, 5, 1);
        mult = (float)((double)r * 0.01 + 1.0);
        o = gm_Query(*(int *)(gm_BuildQuery(1, 1, *(int *)(ev + 0xb0)) + 0x5d), 1, 0, 0xca);
        if (o != 0) {
            while (*(int *)(o + 0x2a) != *(int *)(t + 1)) {
                o = gm_QueryNext();
                if (o == 0)
                    break;
            }
            if (o != 0)
                prob = (float)(((double)*(unsigned char *)(o + 0x37) + 100.0) * 0.01);
        }
        if (U_I((unsigned)&ai_Unk6f2448) != 0)
            prob = 5.0f;
        rr = (unsigned short)util_Random(0x4b);
        if ((float)rr + prob * 100.0f > mult * 125.0f) {
            memset(msg, 0, sizeof(msg));
            msg[8] = 3;
            msg[0x26] = (char)0xa2;
            *(int *)(msg + 0x14) = *(int *)(t + 1);
            *(int *)(msg + 0xc) = U_I((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c);
            msg[0x30] = 2;
            msg[0x7c] = 2;
            *(int *)(msg + 0x34) = *(int *)(mrec + 4);
            *(int *)(msg + 0x38) = *(int *)(rec + 4);
            for (i = 0; i < 16; i++) {
                *(int *)(msg + 0xc0 + i * 4) = *(int *)(rec + 4);
                *(int *)(msg + 0x80 + i * 4) = *(int *)(rec + 4);
            }
            ebx = 0;
            for (p = (unsigned)&ai_Chars; p < (unsigned)((unsigned)ai_CharsV + 0x65400) && ebx < 16 && ebx < 6; p += 0x21c) {
                if (*(char *)(p + 8) != 0 && *(int *)(p + 0x16c) == 1) {
                    *(int *)(msg + 0x80 + ebx * 4) = *(int *)(p + 4);
                    ebx++;
                }
            }
            *(int *)(msg + 0x100) = -1;
            *(int *)(msg + 0x104) = -1;
            *(int *)(msg + 0x108) = -1;
            *(int *)(msg + 0x10c) = *(int *)(gm_BuildQuery(1, 1, *(int *)(ev + 0xb0)) + 1);
            edi = 0;
            for (i = 0; i < 8 && edi < 6; i++) {
                g = *(int *)(ev + 0x8c + i * 4);
                if (g != -1) {
                    *(int *)(msg + 0xc0 + edi * 4) = g;
                    edi++;
                }
            }
            if (edi != 0 && ebx != 0) {
                *(HETIME *)(msg + 0x18) = ai_Time;
                gm_AddTime(msg + 0x18, 0, 0x1e, 0);
                *(int *)(ev + 0xb4) = ((int (*)(void *))ai_Unk4c4fa0)(msg);
                *(HETIME *)(ev + 0x52) = ai_Time;
                gm_AddTime(ev + 0x52, 0, 0, 1);
                *(int *)(ev + 0x70) = 4;
                return;
            }
        } else {
            n = ai_Unk4d4050(gm_BuildQuery(1, 1, *(int *)(ev + 0xb0))) + 0x1125;
            txt_Format(buf, 0x114b, n, *(unsigned short *)(t + 0x27), t, n);
            he_ShowNews(U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, buf, 0, 0x5f0, (char *)0);
            if (*(char *)(rec + 2) == 6 || *(char *)(rec + 2) == 7) {
                txt_Format(buf, 0x1c56, t);
                he_MessageBoxQuickjump(U_I((unsigned)&ai_CharInts + *(unsigned short *)(t + 0x27) * 0x21c), -1, buf, 0, 0x5f0, *(int *)(t + 1), -1, 0, (char *)0);
            }
            *(int *)(ev + 0x70) = -1;
            return;
        }
        t = gm_BuildQuery(1, 1, *(int *)(ev + 0xb0));
        ta = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
        rec = HE_REC(*(unsigned short *)(ta + 0x27));
        r = ai_Unk4d3d10(ta, t, HE_REC(*(unsigned short *)(ev + 8)), rec);
        if (*(char *)(rec + 2) == 6 || *(char *)(rec + 2) == 7) {
            txt_Format(buf, 0x1c54, ta, r);
            he_MessageBoxQuickjump(U_I((unsigned)&ai_CharInts + *(unsigned short *)(ta + 0x27) * 0x21c), -1, buf, 0, 0x5f0, *(int *)(ta + 1), -1, 0, (char *)0);
        }
        if (*(char *)(HE_REC(*(unsigned short *)(ev + 8)) + 2) == 6 || *(char *)(HE_REC(*(unsigned short *)(ev + 8)) + 2) == 7) {
            txt_Format(buf, 0x1c55, *(unsigned short *)(ta + 0x27), ta, r);
            he_ShowNews(U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, buf, 0, 0x5f0, (char *)0);
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 0, 1);
        *(int *)(ev + 0x70) = 1;
        return;
    case 4:
        if (cm_GetSlotState(*(int *)(ev + 0xb4)) == 0)
            return;
        *(int *)(ev + 0xb8) = *(int *)cm_GetSlotValue(*(int *)(ev + 0xb4));
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 1, 0);
        *(int *)(ev + 0x70) = 5;
        return;
    case 5:
        if (cm_PartyById(*(int *)(ev + 0xb8)) != 0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 5, 0);
            return;
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 0, 1);
        *(int *)(ev + 0x70) = 1;
        return;
    default:
        return;
    }
gone:
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 1, 0);
    *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
}
