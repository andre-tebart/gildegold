/* 0x556ed0, the run handler of the delivery event (a Ghidra function with a jump table 0x557150 for 4 states; ROUGH): state 0 waits for the duration clock (0x4fe280 against +0xc0), asks the buildings of the person (0x4ffe90, none: free), state 1 / 2 hands the goods over and tells the owner (0x50ca50, the news 0x1c49 and 0x6273bc text, 0x53dee0 with 0x5ec, the person gets 0x4c4a60 / 0x4c3ba0 / 0x4c3c00 / 0x4c49a0 and the record is freed), the last state frees the person's flag 0x13 / 0x20 and the record */

void he_Unk556ed0(int ev)
{
    int st = *(int *)(ev + 0x70);
    int o = 0, b, q;
    char text[0x100];

    if (st < 0 || st > 3)
        return;
    switch (st) {
    case 0:
        if (gm_MinutesBetween((unsigned)&ai_Time, ev + 0xc0) < 0)
            return;
        b = gm_Query(0, 1, 0, *(int *)(ev + 0xac));
        if (b == 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        *(int *)(ev + 0x70) = 1;
        return;
    case 1:
    case 2:
        gm_FindById(&o, 0, 0, *(int *)(ev + 0xac));
        if (o == 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        txt_Format(text, 0x1c49);
        gm_FindBuildingById(*(int *)(o + 0x1c));
        q = gm_BuildingMainObject(o);
        if (q != 0)
            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, *(int *)(o + 1), *(int *)(q + 2), "_NACHRICHTEN_HS_55", 0);
        else
            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, *(int *)(o + 1), -1, 0, 0);
        cm_SendSetField(*(int *)(o + 2), 0x13, 1, 0, 0x20);
        he_Unk53e1f0((int *)ev);
        return;
    case 3:
        gm_FindById(&o, 0, 0, *(int *)(ev + 0xac));
        if (o != 0)
            cm_SendSetField(*(int *)(o + 2), 0x13, 1, 0, 0x20);
        he_Unk53e1f0((int *)ev);
        return;
    }
}
