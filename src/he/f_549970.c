/* 0x549970, a Ghidra miss (disassembled): the init of the training event: the ability +0xae of the player must be at least 0x5eceb4 (else the event is freed), a running event of the type 0x60 of the player is not doubled (0x53e400 (2, 2, player, 0, 0x60)); +0xac = 0x15, +0xad = 2 * (2 + the ability * 0.0xx + 1), state 0, the clock now, posted 5 (or 1 when 0x6f2448) units ahead, +0x44 = now */

void he_Unk549970(int ev)
{
    int e, a;

    a = *(unsigned char *)(((unsigned)&ai_Unk770b89 + 0x77) + *(unsigned short *)(ev + 8) * 0x21c + *(unsigned char *)(ev + 0xae));
    if (!((float)a < (252.0f))) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    e = (int)he_FindEvent(2, 2, *(unsigned short *)(ev + 8), 0, 0x60);
    if (e != 0) {
        if (e != ev) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        while ((e = (int)he_NextEvent()) != 0) {
            if (e != ev) {
                he_Unk53e1f0((int *)ev);
                return;
            }
        }
    }
    a = *(unsigned char *)(((unsigned)&ai_Unk770b89 + 0x77) + *(unsigned short *)(ev + 8) * 0x21c + *(unsigned char *)(ev + 0xae));
    *(unsigned char *)(ev + 0xac) = 0x15;
    *(unsigned char *)(ev + 0xad) = (unsigned char)(((int)((float)a * 0.5f + 1.0f) + 2) * 2);
    *(int *)(ev + 0x70) = 0;
    *(HETIME *)(ev + 0x52) = ai_Time;
    if (U_I((unsigned)&ai_Unk6f2448) != 0)
        gm_AddTime(ev + 0x52, 0, 0, 1);
    else
        gm_AddTime(ev + 0x52, 5, 0, 0);
    *(HETIME *)(ev + 0x44) = ai_Time;
}
