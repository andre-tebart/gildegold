/* 0x56a170, a run handler (state 0 -> 1; state 1 with orders (+0x14 / +0x18): when due, the goods building +0xb0 gets an order of the good
   +0xac (the 0x4c4880 order, the amount = (a random number below 0x28 times the 0x36 field of the good + 0x28 times it) * the orders), the owner
   is told (news 0x1bb8, "_NACHRICHTEN_HS_08") and the record is freed; states -2 / -1 hand the characters on and free it) (ROUGH) */

void he_Unk56a170(int ev)
{
    char text[0x200];
    int b, rec, amount, bo[2];
    char *r91;
    unsigned short x;

    switch (*(int *)(ev + 0x70)) {
    case -2:
    case -1:
        for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_Chars + 0x65400); rec += 0x21c) {
            if (*(int *)(rec + 0x17c) == ev)
                sv_MoveToGroup(0, *(unsigned short *)rec, 0, 0);
        }
        he_Unk53e1f0((int *)ev);
        return;
    case 0:
        *(int *)(ev + 0x70) = 1;
        return;
    case 1:
        if (*(int *)(ev + 0x14) == 0 && *(int *)(ev + 0x18) == 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) >= 0)
            return;
        gm_FindById(bo, 0, 0, *(int *)(ev + 0xb0));
        b = bo[0];
        if (b == 0) {
            *(int *)(ev + 0x70) = -1;
            return;
        }
        r91 = (char *)U_I((unsigned)&ai_Goods) + *(unsigned short *)(ev + 0xac) * 0x41;
        x = *(unsigned short *)(r91 + 0x36) * 0x28;
        amount = ((unsigned short)util_Random(x) + *(unsigned short *)(r91 + 0x36) * 0x28) * (*(int *)(ev + 0x14) + *(int *)(ev + 0x18));
        cm_SendExSellObject(*(int *)(ev + 0xb4), -1, *(unsigned short *)(ev + 0xac), amount, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
        txt_Format(text, 0x1bb8, *(unsigned short *)(ev + 0xac) * 2 + 0x9a8, b, amount);
        he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(b + 0x27) * 0x21c), -1, text, 0, 0x5f1, *(int *)(b + 1), -1, 0, "_NACHRICHTEN_HS_08");
        for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_Chars + 0x65400); rec += 0x21c) {
            if (*(int *)(rec + 0x17c) == ev)
                sv_MoveToGroup(b, *(unsigned short *)rec, 0, 0);
        }
        he_Unk53e1f0((int *)ev);
        return;
    }
}
