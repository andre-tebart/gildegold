/* 0x54c8e0, a Ghidra miss (disassembled; 640 B): the run handler of the ranking event (+0xac = a text id of 0x501190 kind 1, +0xb0 = the minimum 0x509e60 value): state 0 collects the best 8 player figures (kind 6 / 7, alive, no flag at +0x1af, value >= +0xb0) and the best 8 of the kinds 3 / 4 / 5 by 0x509e60 (the sorted lists +0x14 / +0x34 / +0x88) and posts a message record of 0x114 bytes (type 0xa, 0x22, the number of entries) with them; state 1 (the announcement) shows the news 0x11be to every player figure with a rank (0x53dda0, 0x601) and goes to state 1 again 1 unit later (+0x56 = 0xd, +0x58 = 0x1e) (ROUGH) */

void he_Unk54c8e0(int ev)
{
    char text[0x100];
    char msg[0x114];
    int best[8];
    int val[8];
    int top[16];
    int n = 0, rec, v, i, j, t;

    if (*(int *)(ev + 0x70) == -1) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    if (*(int *)(ev + 0x70) == 0) {
        t = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
        if (t == 0) {
            cm_SendExSetHE(-1, ev);
            return;
        }
        txt_Format(text, 0x11be, t, *(int *)(ev + 0xb0));
        for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_Chars + 0x65400); rec += 0x21c) {
            if ((*(char *)(rec + 2) == 6 || *(char *)(rec + 2) == 7) && *(unsigned char *)(rec + 8) > 0)
                he_ShowNews(*(int *)(rec + 4), *(int *)(rec + 4), text, 0, 0x601, (char *)0);
        }
        *(int *)(ev + 0x52) = *(int *)(ev + 0x52) + 1;
        *(short *)(ev + 0x56) = 0xd;
        *(int *)(ev + 0x58) = 0x1e;
        *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
        return;
    }
    if (*(int *)(ev + 0x70) != 1)
        return;
    for (i = 0; i < 8; i++) {
        best[i] = -1;
        val[i] = 0;
    }
    memset(msg, 0, sizeof(msg));
    for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_Chars + 0x65400) && n < 8; rec += 0x21c) {
        if ((*(char *)(rec + 2) == 6 || *(char *)(rec + 2) == 7) && *(unsigned char *)(rec + 8) > 0 && *(char *)(rec + 0x1b1) == 0
            && gm_GetGoodsValue(rec) >= *(int *)(ev + 0xb0)) {
            top[n] = *(int *)(rec + 4);
            n++;
        }
    }
    for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_Chars + 0x65400); rec += 0x21c) {
        if (*(unsigned char *)(rec + 8) > 0 && *(char *)(rec + 0x1b1) == 0
            && (*(char *)(rec + 2) == 5 || *(char *)(rec + 2) == 3 || *(char *)(rec + 2) == 4)) {
            v = gm_GetGoodsValue(rec);
            if (v < *(int *)(ev + 0xb0))
                continue;
            for (i = 0; i < 8; i++) {
                if (best[i] == -1 || v > val[i])
                    break;
            }
            if (i >= 8)
                continue;
            for (j = 7; j > i; j--) {
                best[j] = best[j - 1];
                val[j] = val[j - 1];
            }
            val[i] = v;
            best[i] = *(int *)(rec + 4);
        }
    }
    for (i = 0; i < 8; i++) {
        if (best[i] != -1 && n < 8)
            top[n++] = best[i];
    }
    if (n != 0) {
        msg[8] = 0xa;
        msg[0x22 + 4] = 0;
        *(int *)(msg + 0x1c) = -1;
        *(int *)(msg + 0xc) = U_I((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c);
        *(int *)(msg + 0x80) = *(int *)(ev + 0xac);
        *(int *)(msg + 0xb0) = n;
        *(int *)(msg + 0x70) = *(int *)(ev + 0xb0);
        *(HETIME *)(msg + 0x14) = ai_Time;
        gm_AddTime(msg + 0x14, 0, 0x1e, 0);
        ai_Unk4c4fa0(msg);
    }
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0x18, 0, 0);
    cm_SendExSetHE(-1, ev);
}
