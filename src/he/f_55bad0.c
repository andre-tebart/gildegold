/* 0x55bad0, the run handler of a building event (4 states by the jump table 0x55bd10; ROUGH): state 0 orders (0x4c4880 (+0xac building, -1, +0xb0, 1, 0, 0)) and marks the building (0x5a / 2 / 0x80), state 1 takes the owner figure (0x4ffe90), tells the player the news 0x1c2b (text 0x9a8 + +0xb0 * 2, "_NACHRICHTEN_HS_53", 0x606 / 0x60b by the 0x50a810 flag), the last state sends the goods (0x4c4db0 (4, ..)) and frees */

void he_Unk55bad0(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b, o, tid = 0x606;
    char text[0x400];

    if (st < 0 || st > 3) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    if (b == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    switch (st) {
    case 0:
        cm_SendExSellObject(*(int *)(b + 1), -1, *(unsigned short *)(ev + 0xb0), 1, 0, 0);
        cm_SendSetField(*(int *)(b + 1), 0x5a, 2, 0x80, 0);
        *(int *)(ev + 0x70) = st + 1;
        return;
    case 1:
        o = gm_Query(*(int *)(b + 0x5d), 2, 6, 1, *(unsigned short *)(ev + 0xb0));
        if (o != 0) {
            int w = gm_FindCharacterByMaster(b);

            txt_Format(text, 0x1c2b, b, *(unsigned short *)(ev + 0xb0) + 0x9a8);
            if (w != 0 && *(unsigned char *)(w + 2) == 1 && (*(unsigned char *)(w + 0x1b4) & 1) != 0)
                tid = 0x60b;
            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(int *)(o + 2), text, 0, tid, *(int *)(b + 1), -1, "_NACHRICHTEN_HS_53", 0);
        }
        *(int *)(ev + 0x70) = st + 1;
        return;
    default:
        cm_SendExSysMessage(4, ev + 0xb0);
        he_Unk53e1f0((int *)ev);
        return;
    }
}
