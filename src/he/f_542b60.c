/* 0x542b60, a Ghidra miss (disassembled): the run handler of the offer event (see 0x542960): state 0 waits for the offer (+0xd4) to be answered (0x4c3ae0, result 1 = accepted, 2 = refused): accepted tells every member (news 0x1bc0 / 0x1bbf / 0x1bc1 / 0x1bc2, messages 0x5f5, "_NACHRICHTEN_HS_16 / 15 / 11 / 12") and re-posts the event in state 1, which sends the 0x4c4fa0 message 0xa2 (kind 5 / 6 by +0xc0) with all the members and the clocks, state 2 (refused) tells the sender (news 0x1bbe) and counts the retry (+0x58 += 2). The message layout and the sprintf arguments are only approximated (ROUGH) */

void ai_Unk4c4fa0(void *p);

void he_Unk542b60(int ev)
{
    int q, p, a3, a4, a5, a6;
    int ok, st;
    int m[0x45];
    char buf[0x200];

    if (*(int *)(ev + 0x70) == -1 || *(int *)(ev + 0x70) == -2) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    if (*(int *)(ev + 0x84) != -1 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
        return;
    st = *(int *)(ev + 0x70);
    *(int *)(ev + 0x84) = -1;
    if (st == 0) {
        ok = (char)cm_GetSlotState(*(int *)(ev + 0xd4));
        if (ok == 0)
            return;
        q = gm_FindChar(*(int *)(ev + 0xb0));
        p = gm_FindChar(*(int *)(ev + 0xac));
        a3 = gm_FindChar(*(int *)(ev + 0xb4));
        a4 = gm_FindChar(*(int *)(ev + 0xb8));
        a5 = gm_FindChar(*(int *)(ev + 0xbc));
        a6 = gm_FindChar(*(int *)(ev + 0xc0));
        if ((char)ok == 1) {
            *(HETIME *)(ev + 0x52) = *(HETIME *)(ev + 0xc5);
            if (*(char *)(q + 2) == 6 || *(char *)(q + 2) == 7) {
                txt_Format(buf, 0x1bc0, *(unsigned short *)p, ev + 0x52, *(unsigned short *)(ev + 0x56), *(unsigned short *)a3, *(unsigned short *)a4, *(unsigned short *)a5);
                he_ShowNews(*(int *)(q + 4), *(int *)(q + 4), buf, 0, 0x5f5, "_NACHRICHTEN_HS_16");
            }
            if (*(char *)(p + 2) == 6 || *(char *)(q + 2) == 7) {
                txt_Format(buf, 0x1bbf, *(unsigned short *)q, ev + 0x52, *(unsigned short *)(ev + 0x56), *(unsigned short *)a3, *(unsigned short *)a4, *(unsigned short *)a5);
                he_ShowNews(*(int *)(p + 4), *(int *)(p + 4), buf, 1, 0x5f5, "_NACHRICHTEN_HS_15");
            }
            txt_Format(buf, 0x1bc1, *(unsigned short *)p, *(unsigned short *)q, ev + 0x52, *(unsigned short *)(ev + 0x56));
            if (*(char *)(a3 + 2) == 6 || *(char *)(a3 + 2) == 7)
                he_ShowNews(*(int *)(a3 + 4), *(int *)(a3 + 4), buf, 0, 0x5f5, "_NACHRICHTEN_HS_11");
            if (*(char *)(a4 + 2) == 6 || *(char *)(a4 + 2) == 7)
                he_ShowNews(*(int *)(a4 + 4), *(int *)(a4 + 4), buf, 0, 0x5f5, "_NACHRICHTEN_HS_11");
            txt_Format(buf, 0x1bc2, *(unsigned short *)p, *(unsigned short *)q, ev + 0x52, *(unsigned short *)(ev + 0x56));
            if (*(char *)(a5 + 2) == 6 || *(char *)(a5 + 2) == 7)
                he_ShowNews(*(int *)(a5 + 4), *(int *)(a5 + 4), buf, 0, 0x5f5, "_NACHRICHTEN_HS_12");
            if (a6 != 0 && (*(char *)(a6 + 2) == 6 || *(char *)(a6 + 2) == 7))
                he_ShowNews(*(int *)(a6 + 4), *(int *)(a6 + 4), buf, 0, 0x5f5, "_NACHRICHTEN_HS_12");
            cm_SendExSetHE(1, ev);
            return;
        }
        if ((char)ok == 2) {
            if (*(char *)(p + 2) == 6 || *(char *)(q + 2) == 7) {
                txt_Format(buf, 0x1bbe, *(unsigned short *)q);
                he_ShowNews(*(int *)(p + 4), *(int *)(p + 4), buf, 0, 0x5f5, (char *)0);
            }
            *(HETIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x58) += 2;
            cm_SendExSetHE(-1, ev);
        }
        return;
    }
    if (st == 1) {
        memset(m, 0, sizeof(m));
        *(unsigned char *)((char *)m + 0x74) = *(unsigned char *)(ev + 0xc4);
        m[0x1f] = *(int *)(ev + 4);
        m[0x0d] = *(int *)(ev + 0xac);
        m[0x0e] = *(int *)(ev + 0xb0);
        m[0x0f] = *(int *)(ev + 0xb4);
        m[0x10] = *(int *)(ev + 0xb8);
        m[0x11] = *(int *)(ev + 0xbc);
        m[0x12] = *(int *)(ev + 0xc0);
        *(unsigned char *)((char *)m + 0x30) = (*(int *)(ev + 0xc0) != -1) + 5;
        *(unsigned char *)((char *)m + 0x14) = 2;
        *(unsigned char *)((char *)m + 0x32) = 0xa2;
        m[0x0a] = -1;
        m[0x0e - 4] = U_I((unsigned)&ai_CharInts + (unsigned short)U_I((unsigned)&ai_PlayerId) * 0x21c);
        *(HETIME *)((char *)m + 0x24) = ai_Time;
        gm_AddTime((char *)m + 0x24, 0, 0x2d, 0);
        ai_Unk4c4fa0(m);
        gm_AddTime(ev + 0x52, 0, 2, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
    }
}
