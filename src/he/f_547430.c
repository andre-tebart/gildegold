/* 0x547430, a Ghidra miss (disassembled): the run handler of the duel announcement ("Special\Beleidigung" window, the event +0xac is the duel (0x53e400 kind 1): its two figures +0xb0 / +0xac): state 0 opens the window with the sound "NeueMessage_s_01" (0x426fa0, when 0x6f25ac is not 2), shows the news 0x1d6c / 0x1d6b and a click target (0x5be1d0 / 0x5be720), state 1 waits for the due time (then 0x5472d0 settles it for the first) or the answer (0xf8a668: 0 = 0x5471e0, 1 = 0x5472d0); a gone figure ends both events (ROUGH) */

void he_Unk547430(int ev)
{
    int ebp = 0, ebx = 0, edi, st, h, r, id;
    int out[2];
    int arg;

    edi = (int)he_FindEvent(1, 1, *(int *)(ev + 0xac));
    if (edi != 0) {
        ebp = gm_FindChar(*(int *)(edi + 0xb0));
        ebx = gm_FindChar(*(int *)(edi + 0xac));
    }
    st = *(int *)(ev + 0x70);
    if (st == -1 || st == -2 || edi == 0 || ebp == 0 || ebx == 0) {
        if (*(int *)(ev + 0x74) != 0)
            he_Unk53d800(ev);
        if (edi != 0) {
            *(HETIME *)(edi + 0x52) = ai_Time;
            cm_SendExSetHE(-1, edi);
        }
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (st == 0) {
        he_Unk53d6f0(ev, 0, 0, (int)"Special\\Beleidigung", 0x5ec);
        if (*(int *)(ev + 0x74) == 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        if (*(unsigned char *)(unsigned)&ai_GameState != 2)
            sb_PlaySample(U_I((unsigned)&snd_SystemBank), "NeueMessage_s_01", 1, 0x3f, 0x7f, 0, 0);
        d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 1);
        io_PrintText(0x1d6c);
        d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 2);
        arg = ebx;
        if (ai_Unk5be1d0(&arg) != 0) {
            ai_Unk5be720(*(short *)(U_I((unsigned)&d2_LastWindowPtr) + 8) / 2 - 0x28, 0, U_I((unsigned)&d2_LastWindow), out);
            id = out[1];
            *(int *)(U_I((unsigned)&d2_Objs) + id * 0x2ec + 0x4c) = 1;
            *(int *)(U_I((unsigned)&d2_Objs) + id * 0x2ec + 0x1c0) = 3;
            *(int *)(U_I((unsigned)&d2_Objs) + id * 0x2ec + 0x2e8) = out[0];
        }
        d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 3);
        io_PrintText(0x1d6b, *(unsigned short *)(edi + 0xb4), *(unsigned short *)ebx, *(unsigned short *)(edi + 0xb6), *(unsigned short *)ebx);
        *(HETIME *)(ev + 0x44) = ai_Time;
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 2, 0, 0);
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        return;
    }
    if (st == 1) {
        if (gm_CompareTime((unsigned)&ai_Time, ev + 0x52) > 0) {
            he_Unk5472d0(edi, ebx, ebp);
            he_Unk53d800(ev);
            he_Unk53e1f0((int *)ev);
            return;
        }
        if (U_I((unsigned)&d2_Unkf8aac4) != *(int *)(*(int *)(ev + 0x74) + 8))
            return;
        h = U_I((unsigned)&d2_ClickedId);
        if (h == -1)
            return;
        if (h == 0)
            he_Unk5471e0(edi, (unsigned short *)ebx, (unsigned short *)ebp);
        else if (h == 1)
            he_Unk5472d0(edi, ebx, ebp);
        he_Unk53d800(ev);
        he_Unk53e1f0((int *)ev);
    }
}
