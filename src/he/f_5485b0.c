/* 0x5485b0, the run handler of the pay event for a master exam of the player (state 0 opens the window "Berufe\Messagebox_Meisterpruefung", the news 0x10b1; state 1 = the answer yes: pays 0x4c47e0 (the amount 0x50a640 of the player) and 0x50ca10 (8), no: 0x50ca10 (-6)) (ROUGH) */

void he_Unk5485b0(int ev)
{
    int h, v;

    if (*(int *)(ev + 0x70) == -1 || *(int *)(ev + 0x70) == -2) {
        if (*(int *)(ev + 0x74) != 0)
            he_Unk53d800(ev);
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (gm_CompareTime((unsigned)&ai_Time, ev + 0x44) == -1)
        return;
    if (*(int *)(ev + 0x70) == 0) {
        v = (int)gm_GetWorth(*(unsigned short *)(ev + 8));
        if (*(int *)(ev + 0x74) == 0)
            he_Unk53d6f0(ev, 0, 0, (int)"Berufe\\Messagebox_Meisterpruefung", 0x5ec);
        d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 0);
        io_PrintText((unsigned)"$C");
        io_PrintText(0x10b1, *(unsigned short *)(ev + 8), v);
        *(int *)(ev + 0x70) = 1;
        return;
    }
    if (*(int *)(ev + 0x70) == 1) {
        if (*(int *)(ev + 0x74) == 0) {
            *(int *)(ev + 0x70) = 0;
            return;
        }
        h = *(int *)(*(int *)(ev + 0x74) + 8);
        if (U_I((unsigned)&d2_Unkf8aac4) == h && U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&d2_ClickedId) == 0) {
                v = gm_GetWorth(*(unsigned short *)(ev + 8));
                cm_SendMoveGoods(*(int *)(U_I((unsigned)&ai_PlayerChar) + 4), U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), U_I((unsigned)&ai_OwnPlayer), v);
                gm_Unk50ca10(*(unsigned short *)(ev + 8), 8);
                he_Unk53d800(ev);
                he_Unk53e1f0((int *)ev);
                return;
            }
            if (U_I((unsigned)&d2_ClickedId) == 1) {
                gm_Unk50ca10(*(unsigned short *)(ev + 8), -6);
                he_Unk53d800(ev);
                he_Unk53e1f0((int *)ev);
                return;
            }
        }
    }
}
