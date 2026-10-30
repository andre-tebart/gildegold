/* 0x56a3d0, the run handler of the sibling event of 0x56a170 (state 0 -> 1; state 1 with orders: when due, the first good object of the kind 0x17 with +0xe == 1 is looked up
   in the goods building +0xb0 (0x4ffe90 / 0x4ffcc0), ordered (0x4c4880), and every good of the table 0x889400 (0x76fdc4 entries) whose 4 ingredients (+0x2e) are all in stock
   (+0xe >= 2) is ordered 3 times; then the order of the amount as in 0x56a170, the news 0x1bb8 / "_NACHRICHTEN_HS_08"; the record is freed) (ROUGH) */

unsigned short *gm_QueryNext(void);
int gm_BuildKindObjects(char *b);

void he_Unk56a3d0(int ev)
{
    char text[0x200];
    int b, g, rec, amount, i, j, ok, id, o, bo[2], bo2[2];
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
        gm_FindById(bo, 0, 0, *(int *)(ev + 0xac));
        b = bo[0];
        if (b == 0) {
            *(int *)(ev + 0x70) = -1;
            return;
        }
        gm_FindById(0, bo2, 0, *(int *)(ev + 0xb0));
        g = gm_Query(*(int *)(bo2[0] + 0x14), 1, 4, 0x17);
        while (g != 0 && *(int *)(g + 0xe) != 1)
            g = gm_QueryNext();
        if (g != 0) {
            cm_SendExSellObject(*(int *)(b + 2), -1, *(unsigned short *)g, 1, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
            gm_BuildKindObjects(bo2[0]);
            for (i = 0; i < *(unsigned short *)(unsigned)&ai_Unk76fdc4; i++) {
                id = *(int *)((unsigned)&ai_Unk889400 + i * 4);
                r91 = (char *)U_I((unsigned)&ai_Goods) + id * 0x41;
                if (*r91 == 0x17)
                    continue;
                ok = 1;
                for (j = 0; j < 4 && ok; j++) {
                    if (*(unsigned short *)(r91 + 0x2e + j * 2) != 0 && *(unsigned short *)(r91 + 0x2e + j * 2) != *(unsigned short *)g) {
                        o = gm_Query(*(int *)(bo2[0] + 0x14), 1, 0, *(unsigned short *)(r91 + 0x2e + j * 2));
                        if (o == 0 || *(int *)(o + 0xe) < 2)
                            ok = 0;
                    }
                }
                if (ok)
                    cm_SendExSellObject(*(int *)(b + 2), -1, id, 3, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
            }
            r91 = (char *)U_I((unsigned)&ai_Goods) + *(unsigned short *)g * 0x41;
            x = *(unsigned short *)(r91 + 0x36) * 0x1e;
            amount = ((unsigned short)util_Random(x) + *(unsigned short *)(r91 + 0x36) * 0x1e) * (*(int *)(ev + 0x14) + *(int *)(ev + 0x18));
            cm_SendExSellObject(*(int *)(ev + 0xb4), -1, *(unsigned short *)g, amount, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
            txt_Format(text, 0x1bb8, *(unsigned short *)g * 2 + 0x9a8, b, amount);
            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(b + 0x27) * 0x21c), -1, text, 0, 0x5f1, *(int *)(b + 1), -1, 0, "_NACHRICHTEN_HS_08");
        }
        for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_Chars + 0x65400); rec += 0x21c) {
            if (*(int *)(rec + 0x17c) == ev)
                sv_MoveToGroup(bo[0], *(unsigned short *)rec, 0, 0);
        }
        he_Unk53e1f0((int *)ev);
        return;
    }
}
