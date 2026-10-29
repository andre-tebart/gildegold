/* 0x548bb0, a Ghidra miss: the run handler of the tip event for the player (word +8): in the state 0 the player's figures (0x501190 kind 4, up to 0x10 with the kind byte 1 or 9 of the building record 0x24d bytes from 0x7e3f30) are collected, one is picked at random and for each of its 0x4ffe90 records (kind 2/6, 0x2a or 0x116) the pay 0x4c4880 (the amount 0.xx * 0x50aaf0 ..) is sent with the text "%s .." (0x5ff710); a player of kind 6 hears the news 0x10b7 with the total (ROUGH) */

void he_Unk548bb0(int ev)
{
    char text[0x400];
    char text2[0x400];
    int sel[16];
    int n = 0, e, ebp, edi, esi, v, total = 0;

    if (*(int *)(ev + 0x70) == -2) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (*(int *)(ev + 0x70) != 0)
        return;
    esi = gm_BuildQuery(1, 4, *(unsigned short *)(ev + 8));
    if (esi == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    while (n < 16) {
        e = U_I((unsigned)&ai_BTypes) + *(unsigned char *)esi + *(unsigned char *)esi * 0x24c;
        if (gm_BuildingClass(*(unsigned char *)esi) == 1 || *(char *)e == 9) {
            sel[n] = esi;
            n++;
        }
        esi = gm_BuildNext();
        if (esi == 0)
            break;
    }
    if (n == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    ebp = sel[(unsigned short)util_Random(n)];
    if (ebp != 0) {
        edi = gm_Query(*(int *)(ebp + 0x5d), 2, 6, 0x2a);
        if (edi == 0)
            edi = gm_Query(*(int *)(ebp + 0x5d), 2, 6, 0x116);
        if (edi != 0) {
            esi = gm_Query(*(int *)(edi + 0x14), 1, 5);
            while (esi != 0) {
                v = (int)((float)gm_ListAmount(edi, esi) * 0.05f);
                if (v < 1)
                    v = 1;
                sprintf(text2, (char *)(unsigned)"cm_RequestSellObjekt(%i, %i, %i, %i, %i, %i)", -1, *(int *)(edi + 2), *(unsigned short *)esi, v, U_I((unsigned)&ai_OwnPlayer), 0);
                cm_SendExSellObject(-1, *(int *)(edi + 2), *(unsigned short *)esi, v, U_I((unsigned)&ai_OwnPlayer), 0);
                total = (int)(gm_GoodsPrice(*(unsigned short *)esi, *(unsigned char *)(unsigned)&ai_OwnPlayer) * (float)v + (float)total);
                esi = gm_QueryNext();
            }
        }
    }
    if (*(char *)(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(ev + 8) * 0x21c) == 6 && total != 0) {
        txt_Format(text, 0x10b7, ebp, total);
        he_MessageBoxQuickjump(U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), text, 0, 0x5ec, *(int *)(ebp + 1), -1, 0, (char *)0);
    }
    he_Unk53e1f0((int *)ev);
}
