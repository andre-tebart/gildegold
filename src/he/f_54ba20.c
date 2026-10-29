/* 0x54ba20, a Ghidra miss (disassembled; 1000 B): the run handler of the audit event ("BetriebsPr\xfc" "fung": the inspector +0xac, the building owner +0xb0): state 0 sends the inspector there (0x4c52e0, +0xb4 / +0xb8), 1 waits until it is idle and rolls (0x508d70, 0x1d records of kind 7 / 4 ..): without a fine the news 0x1a76 / 0x1a78 ("_NACHRICHTEN_HS_88" / "_86") and the state 2, else the fine 0x509b10 * the roll * 0.07 (0x4c4830, the news 0x1a75 / 0x1a77); state 2 frees the event (ROUGH) */

void he_Unk54ba20(int ev)
{
    char text[0x400];
    int esi, ebx, st, v, amt, found = 0, o;
    float f, g;

    esi = gm_BuildQuery(1, 1, *(int *)(ev + 0xb0));
    ebx = gm_FindChar(*(int *)(ev + 0xac));
    st = *(int *)(ev + 0x70);
    if (st == -2 || ebx == 0 || esi == 0 || *(unsigned short *)(esi + 0x27) == 0xffff) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (st == 0) {
        sv_MoveToGroup(esi, *(unsigned short *)ebx, 0, ev);
        ai_Unk4c5230(*(int *)(ebx + 4));
        ai_Unk4c52e0(*(int *)(ebx + 4), *(int *)(esi + 1), -1, 0, 1, "BetriebsPr\xfc" "fung");
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 10, 0);
        *(int *)(ev + 0x70) = 1;
        return;
    }
    if (st == 1) {
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 4, 0);
        if (*(int *)(*(int *)(ebx + 0x184) + 0x128) != 0) {
            *(int *)(ev + 0x70) = 1;
            return;
        }
        f = (100.0f - (float)gm_BuildingLevel(esi)) * 0.5f;
        o = gm_Query(*(int *)(esi + 1) /* the building's records of kind 7 */, 2, 7, 4, 0, (int)f);
        while (o != 0) {
            if (*(int *)(o + 0x1c) == *(int *)(esi + 1) && *(unsigned char *)(o + 0x12) < 0x50) {
                found = 1;
                break;
            }
            o = gm_QueryNext();
        }
        if ((double)f > 0.2 && found == 0) {
            txt_Format(text, 0x1a76, *(unsigned short *)(esi + 0x27), esi);
            he_ShowNews(U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), text, found, 0x5f0, "_NACHRICHTEN_HS_88");
            txt_Format(text, 0x1a78, *(unsigned short *)(ev + 8), esi);
            he_MessageBoxQuickjump(U_I((unsigned)&ai_CharInts + *(unsigned short *)(esi + 0x27) * 0x21c), -1, text, found, 0x5f0, *(int *)(esi + 1), -1, 0, "_NACHRICHTEN_HS_86");
            *(int *)(ev + 0x70) = 2;
            return;
        }
        v = (int)((float)gm_EquipmentValue(esi, 100) * f * 0.07f);
        cm_SendMoveGoodsUnchecked(U_I((unsigned)&ai_CharInts + *(unsigned short *)(esi + 0x27) * 0x21c), 0, U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), v);
        if (*(char *)(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(ev + 8) * 0x21c) == 6 || *(char *)(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(ev + 8) * 0x21c) == 7) {
            txt_Format(text, 0x1a75, v, *(unsigned short *)(esi + 0x27), esi);
            he_ShowNews(U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), text, 0, 0x5f0, "_NACHRICHTEN_HS_87");
        }
        if (*(char *)(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(esi + 0x27) * 0x21c) == 6 || *(char *)(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(esi + 0x27) * 0x21c) == 7) {
            txt_Format(text, 0x1a77, *(unsigned short *)(ev + 8), esi, v);
            he_MessageBoxQuickjump(U_I((unsigned)&ai_CharInts + *(unsigned short *)(esi + 0x27) * 0x21c), -1, text, 0, 0x5f0, *(int *)(esi + 1), -1, 0, "_NACHRICHTEN_HS_86");
        }
        *(int *)(ev + 0x70) = 2;
        return;
    }
    if (st == 2) {
        sv_MoveToGroup(esi, 0, *(unsigned short *)ebx, ev);
        ai_Unk4c5230(*(int *)(ebx + 4));
        ai_Unk4c52e0(*(int *)(ebx + 4), *(int *)(ev + 0xb4), *(int *)(ev + 0xb8), 0, 0, "BetriebsPr\xfc" "fung");
        he_Unk53e1f0((int *)ev);
    }
}
