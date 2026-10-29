/* 0x548790, a Ghidra miss: the run handler of the pay event (state 0: the window "Berufe\Messagebox_Meisterpruefung" and the news 0x10b4 with the amount 0x50a640 * 0.05 (+0xb0); state 1: the answer yes (0xf8a668 = 0) pays 0x4c4830 and posts a message record of type 0x56, no only ends it) (ROUGH) */

void he_Unk548790(int ev)
{
    char msg[0xf8];
    int st, amt, h;

    st = *(int *)(ev + 0x70);
    if (st == -1 || st == -2) {
        if (*(int *)(ev + 0x74) != 0)
            he_Unk53d800(ev);
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (gm_CompareTime((unsigned)&ai_Time, ev + 0x44) == -1)
        return;
    if (*(int *)(ev + 0x70) == 0) {
        amt = (int)((float)gm_GetWorth(*(unsigned short *)(ev + 8)) * 0.05f);
        *(int *)(ev + 0xb0) = amt;
        if (*(int *)(ev + 0x74) == 0)
            he_Unk53d6f0(ev, 0, 0, (int)"Berufe\\Messagebox_Meisterpruefung", 0x5ec);
        if (*(int *)(ev + 0x74) != 0) {
            d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 0);
            io_PrintText((unsigned)"$C");
            io_PrintText(0x10b4, *(unsigned short *)(ev + 8), amt);
            *(int *)(ev + 0x70) = 1;
            return;
        }
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (*(int *)(ev + 0x70) == 1) {
        if (gm_CompareTime((unsigned)&ai_Time, ev + 0x52) > 0) {
            he_Unk53d800(ev);
            he_Unk53e1f0((int *)ev);
            return;
        }
        if (*(int *)(ev + 0x74) == 0) {
            *(int *)(ev + 0x70) = 0;
            return;
        }
        if (U_I((unsigned)&d2_Unkf8aac4) == *(int *)(*(int *)(ev + 0x74) + 8) && U_I((unsigned)&d2_ClickedId) != -1) {
            h = U_I((unsigned)&d2_ClickedId);
            if (h == 0) {
                memset(msg, 0, sizeof(msg));
                amt = *(int *)(ev + 0xb0);
                cm_SendMoveGoodsUnchecked(*(int *)(U_I((unsigned)&ai_PlayerChar) + 4), U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(unsigned char *)(unsigned)&ai_OwnPlayer, amt);
                msg[0] = 0x56;
                *(int *)(msg + 0xc) = -1;
                *(int *)(msg + 8) = U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c);
                msg[0x4a] = 0x11;
                msg[0x6c] = 4;
                *(int *)(msg + 0x70) = amt;
                cm_SendLongMessage(msg);
                he_Unk53d800(ev);
                he_Unk53e1f0((int *)ev);
                return;
            }
            if (h == 1) {
                he_Unk53d800(ev);
                he_Unk53e1f0((int *)ev);
            }
        }
    }
}
