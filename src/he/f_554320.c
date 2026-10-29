/* 0x554320, the run handler of the sales-tour event (a Ghidra miss; ROUGH): +0xb0 .. +0xdc twelve counters of wares to deliver, +0xac the kind, +0xe4 the order handle, +0xe8 the person's id, +0xe0 the target. State 0 -> 1; state 1 waits for the due time and takes the building of the kind +0x10 (none: free), the price check 0x50cd40 / 0x509e60 (a roll above the building's budget: the news 0x1a40, 0x5f2, free), picks the first counter > 0 (none: free) and orders (0x4c55d0, handle +0xe4), state 2; state 2 waits for the order and the person (0x4c3ae0 / 0x4c3b10; gone: the message 0x62720c and free), takes its id, fixes the target (0x501b50) and sends it (0x4c51b0 .. 0x5f1890), state 3; state 3 hands one ware over (counter - 1, a player gets the news 0x1bab, 0x60c / 0x5f2 with "0x626b04", 0x4c4830 pays, 0x506430 raises a field +0x4c) and goes on to the next counter after a random 0x1e + 0x42ec00(10) units (state 1), or frees the record when all are done; states -2 / -1 free */

float gm_Unk50cd40();
int gm_GetGoodsValue();
int cm_SendMessage3d(char *p, unsigned char b, int c, int d);
int gm_BuildingMainObject(unsigned char *b);
void cm_RequestChrMoveToUniverse(int a, int b, int c, const char *s);
short *gm_PlayerRecord(char *c);

void he_Unk554320(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b, i, p, r, o, q, ch;
    char text[0x400];

    if (st < -2 || st > 3)
        return;
    switch (st) {
    case -2:
    case -1:
        he_Unk53e1f0((int *)ev);
        return;
    case 0:
        *(int *)(ev + 0x70) = st + 1;
        return;
    case 1:
        if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) >= 0)
            return;
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
        if (b == 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        ch = (unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c;
        gm_Unk50cd40(ch, *(unsigned char *)b, -1, 1);
        r = (int)gm_GetGoodsValue(ch);
        if (r < 0) {
            txt_Format(text, 0x1a40, b);
            he_MessageBoxQuickjump(*(int *)(ch + 4), *(int *)(ev + 0x10), text, 0, 0x5f2, *(int *)(b + 1), *(int *)(ev + 0xe0), 0, 0);
            he_Unk53e1f0((int *)ev);
            return;
        }
        for (i = 0; i < 12; i++) {
            if (*(int *)(ev + 0xb0 + i * 4) > 0)
                break;
        }
        if (i >= 12) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        *(int *)(ev + 0xe4) = cm_SendMessage3d(b, *(unsigned char *)(ev + 0xac), 0, 0);
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 0, 1);
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        return;
    case 2:
        if (*(int *)(ev + 0xe4) == -1 || cm_GetSlotState(*(int *)(ev + 0xe4)) == 0)
            return;
        p = cm_GetSlotValue(*(int *)(ev + 0xe4));
        if (p == 0) {
            he_ShowNews(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, (char *)((unsigned)&ai_Unk625820 + 0x19ec), 0, 0x5f2, 0);
            he_Unk53e1f0((int *)ev);
            return;
        }
        *(int *)(ev + 0xe8) = *(int *)(p + 4);
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
        if (b == 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        if (*(int *)(ev + 0xe0) == -1) {
            q = gm_BuildingMainObject(b);
            if (q != 0)
                *(int *)(ev + 0xe0) = *(int *)(q + 2);
        }
        cm_RequestChrMoveToUniverse(*(int *)(ev + 0xe8), *(int *)(ev + 0x10), *(int *)(ev + 0xe0), (char *)(unsigned)"dummy_EINGANG");
        *(int *)(ev + 0x70) = 3;
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 0, 1);
        return;
    case 3:
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
        if (b == 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        p = gm_FindChar(*(int *)(ev + 0xe8));
        for (i = 0; i < 12; i++) {
            if (p != 0 && *(int *)(ev + 0xb0 + i * 4) > 0)
                break;
        }
        if (i < 12) {
            ch = (unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c;
            gm_Unk50cd40(ch, *(unsigned char *)b, -1, 1);
            r = (int)gm_GetGoodsValue(ch);
            *(int *)(ev + 0xb0 + i * 4) = *(int *)(ev + 0xb0 + i * 4) - 1;
            o = *(unsigned char *)(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(ev + 8) * 0x21c);
            if (o == 6 || o == 7) {
                q = gm_FindCharacterByMaster(b);
                txt_Format(text, 0x1bab, r, *(unsigned short *)p, b);
                if (q != 0 && (*(unsigned char *)(q + 0x1b4) & 1) != 0)
                    he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(int *)(b + 1), text, 0, 0x60c, *(int *)(b + 1), *(int *)(ev + 0xe0), (char *)((unsigned)&ai_Unk625820 + 0x12e4), 0);
                else
                    he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(int *)(b + 1), text, 0, 0x5f2, *(int *)(b + 1), *(int *)(ev + 0xe0), (char *)((unsigned)&ai_Unk625820 + 0x12e4), 0);
            }
            cm_SendMoveGoodsUnchecked(*(int *)(p + 4), *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(unsigned char *)(unsigned)&ai_OwnPlayer, r);
            q = gm_PlayerRecord((unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c);
            if (q != 0)
                *(int *)(q + 0x4c) = *(int *)(q + 0x4c) + r;
        }
        for (i = 0; i < 12; i++) {
            if (*(int *)(ev + 0xb0 + i * 4) > 0)
                break;
        }
        if (i >= 12) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, (unsigned short)util_Random(10) + 0x1e, 0);
        *(int *)(ev + 0x70) = 1;
        return;
    }
}
