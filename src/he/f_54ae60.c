/* 0x54ae60, a Ghidra miss (disassembled): the protection money question ("Berufe\Messagebox_SCHUTZGELD", the running event +0xac (0x53e400 kind 1) of the pickpocket gang 0x54a7b0 and its target figure +0xb0 of 0x501190): state 0 opens the window and the news 0x1a8c (the amount +0xb4 and the player, in the order of 0x5190a0), state 1 waits for the due time or the answer (0xf8a668: 0 = pays: the gang event goes on in the state 3 with +0xb8 = 1, the target figure gets the flag 0x100 (0x4c4a60 0x5a), the amount goes from the player to the gang leader (0x4c4830) and to the building +0x4d; 1 = refuses: +0xb8 = 0); a gone event or figure ends it (ROUGH) */

void he_Unk54ae60(int ev)
{
    int ebx = 0, esi, h, st, o;

    esi = (int)he_FindEvent(1, 1, *(int *)(ev + 0xac));
    if (esi != 0)
        ebx = gm_BuildQuery(1, 1, *(int *)(esi + 0xb0));
    st = *(int *)(ev + 0x70);
    if (st == -2 || st == -1 || esi == 0 || ebx == 0) {
        if (*(int *)(ev + 0x74) != 0)
            he_Unk53d800(ev);
        he_Unk53e1f0((int *)ev);
    }
    if (*(int *)(ev + 0x70) == 0) {
        he_Unk53d6f0(ev, 0, 0, (int)"Berufe\\Messagebox_SCHUTZGELD", 0x5f3);
        if (*(int *)(ev + 0x74) == 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 0);
        h = ls_GetLanguage();
        if (h == 0)
            io_PrintText(0x1a8c, *(unsigned short *)(esi + 8), *(int *)(esi + 0xb4), ebx);
        else if (h == 1)
            io_PrintText(0x1a8c, *(unsigned short *)(esi + 8), ebx, *(int *)(esi + 0xb4));
        *(HETIME *)(ev + 0x44) = ai_Time;
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 3, 0, 0);
        snd_QueueVoice(-7, 0, "_NACHRICHTEN_HS_34", 0, 0);
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        return;
    }
    if (*(int *)(ev + 0x70) != 1)
        return;
    if (gm_CompareTime((unsigned)&ai_Time, ev + 0x52) > 0) {
        he_Unk53d800(ev);
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (U_I((unsigned)&d2_Unkf8aac4) != *(int *)(*(int *)(ev + 0x74) + 8) || U_I((unsigned)&d2_ClickedId) == -1)
        return;
    h = U_I((unsigned)&d2_ClickedId);
    if (h == 0) {
        *(int *)(ev + 0x70) = 3;
        *(int *)(esi + 0xb8) = 1;
        *(HETIME *)(esi + 0x52) = ai_Time;
        cm_SendExSetHE(3, esi);
        cm_SendSetField(*(int *)(ebx + 1), 0x5a, 2, 0x100, 0);
        cm_SendMoveGoodsUnchecked(U_I((unsigned)&ai_CharInts + *(unsigned short *)(esi + 8) * 0x21c), U_I((unsigned)&ai_CharInts + *(unsigned short *)(ebx + 0x27) * 0x21c), 0, *(int *)(esi + 0xb4));
        if (*(int *)(esi + 0xbc) != -1) {
            o = gm_BuildQuery(1, 1, *(int *)(esi + 0xbc));
            *(int *)(o + 0x4d) = *(int *)(o + 0x4d) + *(int *)(esi + 0xb4);
        }
    } else if (h == 1) {
        *(int *)(ev + 0x70) = 3;
        *(int *)(esi + 0xb8) = 0;
        *(HETIME *)(esi + 0x52) = ai_Time;
        cm_SendExSetHE(3, esi);
    }
    he_Unk53d800(ev);
    he_Unk53e1f0((int *)ev);
}
