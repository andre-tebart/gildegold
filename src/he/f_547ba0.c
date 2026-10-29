/* 0x547ba0, a Ghidra miss (disassembled): the run handler of the quarrel flag event (the figures +0xac a / +0xb0 b, flag 0x400 of +0x1c8): state 2 posts a message record of 0x114 bytes (type 4, +0x18 in 0x1e units, 0x4c4fa0) for a and b, the states 1 / -1 / -2 clear the flags again and end the event (ROUGH) */

void he_Unk547ba0(int ev)
{
    char msg[0x114];
    int a, b;

    if (*(int *)(ev + 0x84) != -1 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
        return;
    switch (*(int *)(ev + 0x70)) {
    case 2:
        *(int *)(ev + 0x84) = -1;
        if ((*(unsigned char *)(ev + 0x78) & 2) == 0)
            return;
        a = gm_FindChar(*(int *)(ev + 0xac));
        b = gm_FindChar(*(int *)(ev + 0xb0));
        if (a != 0) {
            if (b != 0) {
                memset(msg, 0, sizeof(msg));
                *(int *)(msg + 0x34) = *(int *)(a + 4);
                *(HETIME *)(msg + 0x18) = ai_Time;
                msg[8] = 4;
                msg[0x26] = (char)0xa2;
                *(int *)(msg + 0x14) = -1;
                msg[0x30] = 2;
                *(int *)(msg + 0xc) = U_I((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c);
                *(int *)(msg + 0x38) = *(int *)(b + 4);
                msg[0x7c] = 0;
                gm_AddTime(msg + 0x18, 0, 0x1e, 0);
                ai_Unk4c4fa0(msg);
            }
            cm_SendSetField(*(int *)(a + 4), 0x1c8, 4, 0, 0x400);
        }
        if (b != 0)
            cm_SendSetField(*(int *)(b + 4), 0x1c8, 4, 0, 0x400);
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 2, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    case 1:
        *(int *)(ev + 0x84) = -1;
        if ((*(unsigned char *)(ev + 0x78) & 2) == 0)
            return;
        a = gm_FindChar(*(int *)(ev + 0xac));
        b = gm_FindChar(*(int *)(ev + 0xb0));
        if (a != 0)
            cm_SendSetField(*(int *)(a + 4), 0x1c8, 4, 0, 0x400);
        if (b != 0)
            cm_SendSetField(*(int *)(b + 4), 0x1c8, 4, 0, 0x400);
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 2, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    case -2:
    case -1:
        *(int *)(ev + 0x84) = -1;
        if ((*(unsigned char *)(ev + 0x78) & 2) != 0) {
            a = gm_FindChar(*(int *)(ev + 0xac));
            b = gm_FindChar(*(int *)(ev + 0xb0));
            if (a != 0)
                cm_SendSetField(*(int *)(a + 4), 0x1c8, 4, 0, 0x400);
            if (b != 0)
                cm_SendSetField(*(int *)(b + 4), 0x1c8, 4, 0, 0x400);
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 2, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        }
        he_Unk53e1f0((int *)ev);
        return;
    default:
        *(int *)(ev + 0x84) = -1;
        return;
    }
}
