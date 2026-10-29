/* 0x549120, a Ghidra miss (disassembled): the run handler of the feud between the player figure (word +8, record +0x5c = its partner) and the five figures of its slots (+0x68 ..): state 0 rolls (0x42ec20 + (1 - f * f) against f * f * +0xac (a float), f = (5 - the figures alive) * 0.2) and either raises +0xac by f * f * 0.1 or goes to the state 1; state 1 waits for +0xac >= 3.0 (+1.0 per step), posts a message record (type 7) for a player figure of kind 6, and sends 0x4c4650 (8, ..) between the two figures (ROUGH) */

void he_Unk549120(int ev)
{
    char msg[0x114];
    int x = -1, rec, o, i, n, st, g;
    float f, f2, thr;

    rec = (unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c;
    o = gm_FindChar(*(int *)(rec + 0x5c));
    st = *(int *)(ev + 0x70);
    if (o == 0 || *(unsigned char *)(o + 8) == 0 || *(unsigned char *)(rec + 8) == 0 || st == -2) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (st == 0) {
        n = 0;
        for (i = 0; i < 5; i++) {
            g = *(int *)(rec + 0x68 + i * 4);
            if (g != -1 && gm_FindChar(g) != 0)
                n++;
        }
        if (n == 5) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        f = (5.0f - (float)n) * 0.2f;
        f2 = f * f;
        thr = f2 * *(float *)(ev + 0xac);
        if (util_RandomF() + (1.0f - f2) < thr) {
            *(int *)(ev + 0x52) = *(int *)(ev + 0x52) + 1;
            *(int *)(ev + 0x70) = 1;
            *(int *)(ev + 0xac) = 0;
        } else {
            *(float *)(ev + 0xac) = (float)((double)f2 * 0.1 + (double)*(float *)(ev + 0xac));
        }
        return;
    }
    if (st != 1)
        return;
    if ((double)*(float *)(ev + 0xac) < 3.0) {
        *(int *)(ev + 0x52) = *(int *)(ev + 0x52) + 1;
        *(float *)(ev + 0xac) = (float)((double)*(float *)(ev + 0xac) + 1.0);
        return;
    }
    if (*(char *)(rec + 2) == 6) {
        memset(msg, 0, sizeof(msg));
        msg[8] = 7;
        *(int *)(msg + 0xc) = *(int *)(rec + 4);
        *(int *)(msg + 0x14) = -1;
        *(HETIME *)(msg + 0x18) = ai_Time;
        msg[0x26] = 0x20;
        msg[0x30] = 2;
        *(int *)(msg + 0x34) = *(int *)(o + 4);
        gm_AddTime(msg + 0x18, 0, 0x1e, 0);
        ai_Unk4c4fa0(msg);
    }
    *(int *)(ev + 0xac) = 0;
    if (*(int *)(rec + 0x170) != 0)
        x = *(int *)(*(int *)(rec + 0x170) + 1);
    if (*(char *)(rec + 9) == 0)
        cm_SendCreateCharacter(8, *(int *)(rec + 4), *(int *)(o + 4), 0, x, 0, 0, 2);
    else
        cm_SendCreateCharacter(8, *(int *)(o + 4), *(int *)(rec + 4), 0, x, 0, 0, 2);
    *(int *)(ev + 0x70) = 0;
    *(int *)(ev + 0x52) = *(int *)(ev + 0x52) + (unsigned short)util_Random(8) + 1;
}
