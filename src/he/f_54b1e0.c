/* 0x54b1e0, a Ghidra miss (disassembled; 1976 B): the run handler of the protection money event ("Schutzgeld": the collector +0xac (a figure), the victim +0xb0 (a building owner of 0x501190), +0xb4 the amount, +0xb8 the answer (-1 open, 0 refused, 1 paid, 2 the victim is a player: a message record type 0x63 was posted), +0xbc the building, +0xc0 ..): state 1 sends the collector to the victim ("Schutzgeld", 0x4c52e0), state 2 waits until it is idle, then sizes the amount (0x509b10 * 0.065), sets the due clock 2 .. 0x12 units ahead and decides (a player victim gets the message record, else a roll against 0.001 * the fame at +0x770c94 gives +0xb8 = random(2), 1 for flag 1 of +0x5b), state 3 reports the result (pay: the flag 0x100 (0x4c4a60 0x5a) at the victim, the amount to the collector 0x4c4830 and the building +0x4d, the news 0x1a8d / 0x1a8e / 0x1a8f / 0x1a90 with "_NACHRICHTEN_HS_84" / "_85"), then the collector goes home ("Schutzgeld") and the event ends (ROUGH) */

void he_Unk54b1e0(int ev)
{
    char text[0x100];
    char msg[0xf8];
    int ebp, edi, st, r, o, amt;

    ebp = gm_BuildQuery(1, 1, *(int *)(ev + 0xb0));
    edi = gm_FindChar(*(int *)(ev + 0xac));
    st = *(int *)(ev + 0x70);
    if (st == -1 || st == -2) {
        if (ebp != 0 && (*(unsigned char *)(ev + 0x78) & 2) != 0)
            cm_SendSetField(*(int *)(ebp + 1), 0x5a, 2, 0, 0x100);
        he_Unk53e1f0((int *)ev);
        return;
    }
    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    if (*(int *)(ev + 0x84) != -1 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
        return;
    *(int *)(ev + 0x84) = -1;
    if (edi == 0 || ebp == 0 || *(unsigned short *)(ebp + 0x27) == 0xffff) {
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 2, 0);
        *(int *)(ev + 0x70) = -1;
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    }
    switch (*(int *)(ev + 0x70)) {
    case 1:
        sv_MoveToGroup(ebp, 0, *(unsigned short *)edi, ev);
        ai_Unk4c5230(*(int *)(edi + 4));
        ai_Unk4c52e0(*(int *)(edi + 4), *(int *)(ebp + 1), -1, 0, 1, "Schutzgeld");
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 10, 0);
        *(int *)(ev + 0x70) = 2;
        *(int *)(ev + 0x84) = cm_SendExSetHE(2, ev);
        return;
    case 2:
        if (*(int *)(*(int *)(edi + 0x184) + 0x128) != 0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 4, 0);
            *(int *)(ev + 0x70) = 2;
            *(int *)(ev + 0x84) = cm_SendExSetHE(2, ev);
            return;
        }
        amt = gm_EquipmentValue(ebp, 100);
        *(int *)(ev + 0xb4) = (int)((float)amt * 0.065f);
        r = (unsigned)&ai_Chars + *(unsigned short *)(ebp + 0x27) * 0x21c;
        if (*(char *)(r + 2) == 6 || *(char *)(r + 2) == 7) {
            memset(msg, 0, sizeof(msg));
            *(int *)(msg + 0x1c) = *(int *)(ebp + 1);
            *(int *)(msg + 0x18) = U_I(r + 4);
            msg[4] = 0x63;
            msg[0x3a] = 9;
            *(int *)(msg + 0x34) = *(int *)(ev + 4);
            *(int *)(msg + 0x58) = *(int *)(ev + 4);
            *(HETIME *)(msg + 0x28) = ai_Time;
            cm_SendLongMessage(msg);
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 3, 0);
            *(int *)(ev + 0x70) = 3;
            *(int *)(ev + 0xb8) = 2;
            *(int *)(ev + 0x84) = cm_SendExSetHE(3, ev);
            return;
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, (unsigned short)util_Random(0x10) + 2, 0);
        *(int *)(ev + 0x70) = 3;
        if ((*(unsigned char *)(ebp + 0x5b) & 1) != 0) {
            *(int *)(ev + 0xb8) = 2;
        } else if (*(unsigned char *)(((unsigned)&ai_Unk770b89 + 0x127) + *(unsigned short *)(ebp + 0x27) * 0x14) != 0xb
                   && util_RandomF() < *(float *)(((unsigned)&ai_Unk770b89 + 0x10b) + *(unsigned short *)(ebp + 0x27) * 0x14) * 0.001f) {
            *(int *)(ev + 0xb8) = (unsigned short)util_Random(2);
        } else {
            *(int *)(ev + 0xb8) = 1;
        }
        *(int *)(ev + 0x84) = cm_SendExSetHE(3, ev);
        return;
    case 3:
        r = *(int *)(ev + 0xb8);
        if (r == 1) {
            cm_SendSetField(*(int *)(ebp + 1), 0x5a, 2, 0x100, 0);
            cm_SendMoveGoodsUnchecked(U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), U_I((unsigned)&ai_CharInts + *(unsigned short *)(ebp + 0x27) * 0x21c), 0, *(int *)(ev + 0xb4));
            if (*(int *)(ev + 0xbc) != -1) {
                o = gm_BuildQuery(1, 1, *(int *)(ev + 0xbc));
                *(int *)(o + 0x4d) = *(int *)(o + 0x4d) + *(int *)(ev + 0xb4);
            }
            *(HETIME *)(ev + 0x52) = ai_Time;
            *(HETIME *)(ev + 0x44) = ai_Time;
            gm_AddTime(ev + 0x52, 0x30, 0, 0);
            *(int *)(ev + 0x70) = 1;
            *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
            txt_Format(text, 0x1a8d, *(unsigned short *)(ebp + 0x27), ebp, *(int *)(ev + 0xb4));
            he_ShowNews(U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), text, 0, 0x5f3, "_NACHRICHTEN_HS_84");
            return;
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 5, 0);
        if (r == 0) {
            *(int *)(ev + 0x70) = 1;
            *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
            txt_Format(text, 0x1a8e, *(unsigned short *)(ebp + 0x27), ebp);
        } else {
            *(int *)(ev + 0x70) = 1;
            *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
            txt_Format(text, 0x1a8f, *(unsigned short *)(ebp + 0x27), ebp);
        }
        he_ShowNews(U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), text, 0, 0x5f3, "_NACHRICHTEN_HS_85");
        return;
    case 4:
        if (*(int *)(ev + 0xb8) == 1) {
            txt_Format(text, 0x1a90, ebp);
            he_ShowNews(U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), text, 0, 0x5f3, (char *)0);
            he_ShowNews(U_I((unsigned)&ai_CharInts + *(unsigned short *)(ebp + 0x27) * 0x21c), U_I((unsigned)&ai_CharInts + *(unsigned short *)(ebp + 0x27) * 0x21c), text, 0, 0x5f3, (char *)0);
        }
        sv_MoveToGroup(ebp, 0, *(unsigned short *)edi, 0);
        ai_Unk4c5230(*(int *)(edi + 4));
        ai_Unk4c52e0(*(int *)(edi + 4), *(int *)(ev + 0xbc), *(int *)(ev + 0xc0), 0, 0, "Schutzgeld");
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 2, 0);
        *(int *)(ev + 0x70) = -1;
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    default:
        return;
    }
}
