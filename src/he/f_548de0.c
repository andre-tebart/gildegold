/* 0x548de0, a Ghidra miss: the run handler of the event of the player (word +8): in the state 0 one of the figures of the player (0x501190 kind 4) with 0x501d80 == 6 is picked at random and 0x50cad0 (-0x32) is called for it; a player of kind 6 hears the news 0x10b8 (ROUGH) */

void he_Unk548de0(int ev)
{
    char text[0x400];
    int sel[16];
    int n = 0, esi;

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
        if (gm_BuildingClass(*(unsigned char *)esi) == 6) {
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
    esi = sel[(unsigned short)util_Random(n)];
    if (esi != 0)
        gm_Unk50cad0(esi, -0x32);
    if (*(char *)(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(ev + 8) * 0x21c) == 6) {
        txt_Format(text, 0x10b8, esi);
        he_MessageBoxQuickjump(U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), text, 0, 0x5ec, *(int *)(esi + 1), -1, 0, (char *)0);
    }
    he_Unk53e1f0((int *)ev);
}
