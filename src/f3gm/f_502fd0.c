/* 0x502fd0, the sale of the building with the id a to the player b for the price c (the sale record d, flag e): marks "buy building" in the message log (0x4c4540 .. 0x4c4580), pays the former owner (a message 0x17e6 with "_NACHRICHTEN_HS_30", 0x4c4830 the price) or the bank (0x4c47e0), hands the ownership over (0x4c54a0, and for the type 2 with e the 0x4c5540), stamps the sale record d one hour ahead (0x4fe140 +1 second, 0x4c4c60) and sets the building's owner field (0x4c4a60 (id, 0x5a, 2, 0, 2)). ROUGH */

void gm_Unk502fd0(int a, int b, int c, int d, int e)
{
    unsigned char *bl;
    short *ch;
    char *k;
    char buf[256];
    int u4, u5;

    bl = (unsigned char *)gm_BuildQuery(1, 1, a);
    ch = (short *)gm_FindChar(b);
    k = ai_BTypes + *bl * 0x24d;
    cm_BeginGroup("buy building");
    if (*(short *)(bl + 0x25) == -1) {
        cm_SendMoveGoods(-1, b, ai_OwnPlayer, c);
        cm_SendOwnership(*(int *)(bl + 1), *(int *)(ch + 2), *(int *)(ch + 2));
        if (e == 0 || *k != 2)
            goto L1;
        u4 = *(int *)(bl + 1);
        u5 = *(int *)(ch + 2);
    } else {
        txt_Format(buf, 0x17e6, bl, *ch, c);
        he_MessageBoxQuickjump(*(int *)(ai_Chars[*(unsigned short *)(bl + 0x25)] + 4), -1, buf, 0, 0x5ff, *(int *)(bl + 1), -1, 0, "_NACHRICHTEN_HS_30");
        cm_SendMoveGoodsUnchecked(*(int *)(ai_Chars[*(unsigned short *)(bl + 0x25)] + 4), b, ai_OwnPlayer, c);
        cm_SendOwnership(*(int *)(bl + 1), *(int *)(ch + 2), *(int *)(ch + 2));
        if (e == 0 || *k != 2)
            goto L1;
        u4 = *(int *)(bl + 1);
        u5 = *(int *)(ch + 2);
    }
    cm_SendBoss(u5, u4);
L1:
    if (d != 0) {
        *(int *)(d + 0x52) = *(int *)ai_Time;
        *(int *)(d + 0x56) = *(int *)(ai_Time + 4);
        *(int *)(d + 0x5a) = *(int *)(ai_Time + 8);
        *(short *)(d + 0x5e) = *(short *)(ai_Time + 0xc);
        gm_AddTime((GCLOCK *)(d + 0x52), 0, 0, 1);
        *(int *)(d + 0x84) = ((int (*)(int, int))cm_SendExSetHE)(-1, d);
    }
    cm_SendSetField(*(int *)(bl + 1), 0x5a, 2, 0, 2);
    cm_EndGroup("buy building");
}
