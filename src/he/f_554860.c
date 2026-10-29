/* 0x554860, the run handler of the single-ware variant of 0x554320 (ROUGH): one counter +0xb0, +0xac the ware, +0xb4 the target building (0 = the first of the kind +0x29, -1 = kind 0xfd), +0xb8 the order handle, +0xbc the person's id. State 0 -> 1; state 1 orders the delivery (0x4c55d0 (building, 1, 0, the ware's price byte +0x223 of the kind record)), state 2 sends the person (0x4c51b0, news 0x1bb1 / "0x627228" to a player, payment 0x4c4830) and posts the next round 10 + random 10 later (state 1); states -2 / -1 free */

void he_Unk554860(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b, p, r, q, ch, kind, tgt, n;
    char text[0x400];

    if (st < -2 || st > 2)
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
        gm_Unk50cd40(ch, *(unsigned char *)b, *(unsigned char *)(ev + 0xac), -1);
        r = (int)gm_GetGoodsValue(ch);
        kind = U_I((unsigned)&ai_BTypes) + *(unsigned char *)b * 0x24d;
        if (r < 0) {
            txt_Format(text, 0x1a41, b);
            he_MessageBoxQuickjump(*(int *)(ch + 4), *(int *)(ev + 0x10), text, 0, 0x5f2, *(int *)(b + 1), *(int *)(ev + 0xb4), 0, 0);
            he_Unk53e1f0((int *)ev);
            return;
        }
        if (*(int *)(ev + 0xb0) <= 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        *(int *)(ev + 0xb8) = cm_SendMessage3d(b, 1, 0, *(unsigned char *)(kind + *(unsigned char *)(ev + 0xac) + 0x223));
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        return;
    case 2:
        if (cm_GetSlotState(*(int *)(ev + 0xb8)) == 0)
            return;
        p = cm_GetSlotValue(*(int *)(ev + 0xb8));
        if (p == 0) {
            he_ShowNews(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, (char *)((unsigned)&ai_Unk625820 + 0x1a1c), 0, 0x5f8, 0);
            he_Unk53e1f0((int *)ev);
            return;
        }
        U_I((unsigned)&ai_Unk6f2318) = 1;
        *(int *)(ev + 0xbc) = *(int *)(p + 4);
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
        if (b == 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        kind = U_I((unsigned)&ai_BTypes) + *(unsigned char *)b * 0x24d;
        p = cm_GetSlotValue(*(int *)(ev + 0xb8));
        *(unsigned char *)(p + 0x164) = *(unsigned char *)(kind + *(unsigned char *)(ev + 0xac) + 0x223);
        *(unsigned char *)(p + 0x165) = 0;
        if (*(int *)(ev + 0xb0) > 0) {
            ch = (unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c;
            gm_Unk50cd40(ch, *(unsigned char *)b, *(unsigned char *)(ev + 0xac), -1);
            n = (int)gm_GetGoodsValue(ch);
            *(int *)(ev + 0xb0) = *(int *)(ev + 0xb0) - 1;
            q = *(unsigned char *)(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(ev + 8) * 0x21c);
            if (q == 6 || q == 7) {
                txt_Format(text, 0x1bb1, n, *(unsigned short *)p, b);
                he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(int *)(b + 1), text, 0, 0x5f2, *(int *)(b + 1), *(int *)(ev + 0xb4), (char *)((unsigned)&ai_Unk625820 + 0x1a08), 0);
            }
            cm_SendMoveGoodsUnchecked(*(int *)(p + 4), *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(unsigned char *)(unsigned)&ai_OwnPlayer, n);
        }
        tgt = *(int *)(ev + 0xb4);
        if (tgt == 0) {
            q = gm_Query(*(int *)(b + 0x5d), 1, 0, *(unsigned short *)(b + 0x29));
            tgt = q != 0 ? *(int *)(q + 2) : -1;
        } else if (tgt == -1) {
            q = gm_Query(*(int *)(b + 0x5d), 1, 0, 0xfd);
            tgt = q != 0 ? *(int *)(q + 2) : -1;
        }
        cm_RequestChrMoveToUniverse(*(int *)(ev + 0xbc), *(int *)(ev + 0x10), tgt, (char *)(unsigned)"dummy_EINGANG");
        if (*(int *)(ev + 0xb0) <= 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        gm_AddTime(ev + 0x52, 0, (unsigned short)util_Random(10) + 10, 0);
        *(int *)(ev + 0x70) = 1;
        return;
    }
}
