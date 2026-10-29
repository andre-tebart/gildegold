/* 0x545840, a Ghidra miss (disassembled; 2224 B): the run handler of the patrol / raid event (see 0x545670, "STREIFE_INIT"; the figures +0xc4 .. +0xd0 are its guards, +0xac the target, +0xb0 the start, +0xb4 .. +0xc0 the way, +0xd8 the way index, +0xdc the round): state 0 = go to the target (0x4c52e0 with "Streife" / "Razzia"; "GOTO_TARGET: %i"), 1 = go to the start (the guards get their pay from the guard ranks: a sum of random rolls, the news 0x1a6a and the money 0x4c47e0; "GOTO_START: %i"), 2 = wait until the guards are idle (+0x128 of their job record), 3 = walk the way point (+0xb4 + 4 * +0xd8: a gesture "gestik/kopf_kratzen" / "gestik/durch_haar_fahr" / "gestik/kinn_kratzen" and the next way point; "GOTO_NEXT: %i"), 0x400 = the same wait as 2, entered when +0xd4 is set; when no guard is left the event ends (ROUGH) */

int gm_MinutesBetween();
int gm_PlayerFactor(int a, unsigned char k);
void ai_Unk4c5230(int a);
void ai_Unk4c5410(int a, const char *s, unsigned char c);
void cm_SendMoveGoods(int a, int b, unsigned char c, int d);
unsigned char *gm_BuildQuery(int n, ...);
char *gm_Query(void *list, int n, ...);

void he_Unk545840(int ev)
{
    char buf[0x78];
    char names[3][0x20] = { "gestik/kopf_kratzen", "gestik/durch_haar_fahr", "gestik/kinn_kratzen" };
    char text[0x100];
    int i, gone = 0, st, flag = 0, t = 0, obj = 0, g, p, n = 0, sum = 0, val, k, v;

    st = *(int *)(ev + 0x70);
    if (st == -1) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if ((*(unsigned char *)(ev + 0x78) & 4) != 0) {
        if (st == -2)
            he_Unk53e1f0((int *)ev);
        return;
    }
    for (i = 0; i < 4; i++) {
        if (gm_FindChar(*(int *)(ev + 0xc4 + i * 4)) != 0)
            break;
        gone++;
    }
    if (gone == 4) {
        gm_AddTime(ev + 0x52, 0, 1, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    }
    if (*(int *)(ev + 0x70) == -2) {
        flag = 1;
        *(int *)(ev + 0x70) = 1;
    }
    if (*(int *)(ev + 0xd4) != -1)
        *(int *)(ev + 0x70) = 0x400;
    switch (*(int *)(ev + 0x70)) {
    case 0:
        t = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
        if (t == 0) {
            gm_AddTime(ev + 0x52, 0, 1, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        for (i = 0; i < 4; i++) {
            g = *(int *)(ev + 0xc4 + i * 4);
            if (g != -1) {
                p = gm_FindChar(g);
                if (p != 0 && *(int *)(p + 0x184) != 0) {
                    sv_MoveToGroup(0, 0, *(unsigned short *)p, ev);
                    ai_Unk4c5230(g);
                    ai_Unk4c52e0(g, *(int *)(t + 1), -1, 0, 1, "Streife");
                } else {
                    *(int *)(ev + 0xc4 + i * 4) = -1;
                }
            }
        }
        sprintf(buf, "GOTO_TARGET: %i", *(int *)(t + 1));
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 2, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(2, ev);
        return;
    case 1:
        t = gm_BuildQuery(1, 1, *(int *)(ev + 0xb0));
        if (t == 0) {
            gm_AddTime(ev + 0x52, 0, 1, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        obj = gm_Query(*(int *)(t + 0x5d), 1, 0, 0x146);
        for (i = 0; i < 4; i++) {
            g = *(int *)(ev + 0xc4 + i * 4);
            if (g != -1) {
                p = gm_FindChar(g);
                if (p != 0 && *(int *)(p + 0x184) != 0) {
                    val = (int)((float)*(unsigned char *)(p + 0x82) * 0.023809524f + 1.0f);
                    k = (unsigned short)util_Random(4) + 3;
                    for (; k > 0; k--)
                        sum += (unsigned short)util_Random((val + 1) * val) + val * 2;
                    n++;
                    ai_Unk4c5230(g);
                    ai_Unk4c52e0(g, *(int *)(t + 1), *(int *)(obj + 2), 0, 0, "Streife");
                    sv_MoveToGroup(0, 0, *(unsigned short *)p, ev);
                } else {
                    *(int *)(ev + 0xc4 + i * 4) = -1;
                }
            }
        }
        if (flag) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        if (n != 0) {
            v = gm_PlayerFactor(sum, U_I((unsigned)&ai_OwnPlayer) & 0xff);
            txt_Format(text, 0x1a6a, v);
            he_MessageBoxQuickjump(U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, *(int *)(t + 1), -1, 0, (char *)0);
            cm_SendMoveGoods(U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, *(unsigned char *)(unsigned)&ai_OwnPlayer, v);
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 2, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        sprintf(buf, "GOTO_START: %i", *(int *)(t + 1));
        return;
    case 2:
        for (i = 0; i < 4; i++) {
            g = *(int *)(ev + 0xc4 + i * 4);
            if (g != -1 && (p = gm_FindChar(g)) != 0 && *(int *)(p + 0x184) != 0 && *(int *)(*(int *)(p + 0x184) + 0x128) != 0)
                return;
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 2, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(3, ev);
        return;
    case 0x400:
        for (i = 0; i < 4; i++) {
            g = *(int *)(ev + 0xc4 + i * 4);
            if (g != -1 && (p = gm_FindChar(g)) != 0 && *(int *)(p + 0x184) != 0 && *(int *)(*(int *)(p + 0x184) + 0x128) != 0)
                goto busy;
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        *(int *)(ev + 0xd4) = -1;
        gm_AddTime(ev + 0x52, 0, 2, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(3, ev);
        return;
    busy:
        gm_AddTime(ev + 0x52, 0, 4, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(0x400, ev);
        return;
    case 3:
        if (*(int *)(ev + 0xd8) <= 3 && *(int *)(ev + 0xb4 + *(int *)(ev + 0xd8) * 4) != -1) {
            if (gm_MinutesBetween(ev + 0x44, (unsigned)&ai_Time) > 0x1e0) {
                *(HETIME *)(ev + 0x52) = ai_Time;
                gm_AddTime(ev + 0x52, 0, 2, 0);
                *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
                return;
            }
            t = gm_BuildQuery(1, 1, *(int *)(ev + 0xb4 + *(int *)(ev + 0xd8) * 4));
            *(int *)(ev + 0xd8) = *(int *)(ev + 0xd8) + 1;
            if (t == 0) {
                *(HETIME *)(ev + 0x52) = ai_Time;
                gm_AddTime(ev + 0x52, 0, 2, 0);
                *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
                return;
            }
            for (i = 0; i < 4; i++) {
                g = *(int *)(ev + 0xc4 + i * 4);
                if (g != -1) {
                    p = gm_FindChar(g);
                    if (p != 0 && *(int *)(p + 0x184) != 0) {
                        sv_MoveToGroup(0, 0, *(unsigned short *)p, ev);
                        ai_Unk4c5230(g);
                        ai_Unk4c52e0(g, *(int *)(t + 1), -1, 0, 1, "Streife");
                        k = (unsigned short)util_Random(2) + 1;
                        ai_Unk4c5410(g, names[(unsigned short)util_Random(3)], k);
                    } else {
                        *(int *)(ev + 0xc4 + i * 4) = -1;
                    }
                }
            }
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 2, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(2, ev);
            sprintf(buf, "GOTO_NEXT: %i", *(int *)(t + 1));
            return;
        }
        if (gm_MinutesBetween(ev + 0x44, (unsigned)&ai_Time) > 0x1e0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 2, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
            return;
        }
        *(int *)(ev + 0xd8) = 0;
        *(char *)(ev + 0xdc) = *(char *)(ev + 0xdc) + 1;
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 2, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
        return;
    default:
        return;
    }
}
