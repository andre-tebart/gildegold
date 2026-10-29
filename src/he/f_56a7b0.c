/* 0x56a7b0, the alloc of an event of the type 0xb: when another record of the type (+0x10) already works for the same good (+0xac) of another player (+4) the new one is freed,
   else both clocks (+0x52 / +0x60) = the start clock, due now + 1, the handles +0xe8 / +0xec = -1, and a message record (0xf8 bytes: kind 0xf, the player's 0x770b84 entry, the clock, the good,
   the buyer +0x10, the goods building +0xb0) is posted (0x4c4b70) unless +0xf1 is set (then due 1 + 0x1e later); +0xf0 = 4 when the character +0xb4 exists (ROUGH) */

int gm_FindObjectInst(int id);

void he_Unk56a7b0(int ev)
{
    char msg[0xf8];
    int e2;

    for (e2 = (int)he_FindEvent(2, 0, 0xb, 3, *(int *)(ev + 0x10)); e2 != 0; e2 = he_NextEvent()) {
        if (*(int *)(e2 + 0xac) == *(int *)(ev + 0xac) && *(int *)(e2 + 4) != *(int *)(ev + 4)) {
            he_Unk53e1f0((int *)ev);
            return;
        }
    }
    *(HETIME *)(ev + 0x52) = *(HETIME *)(ev + 0x44);
    *(HETIME *)(ev + 0x60) = *(HETIME *)(ev + 0x44);
    *(int *)(ev + 0xe8) = -1;
    *(int *)(ev + 0xec) = -1;
    gm_AddTime(ev + 0x52, 0, 1, 0);
    memset(msg, 0, sizeof(msg));
    *(HETIME *)(msg + 0x28) = *(HETIME *)(ev + 0x44);
    msg[4] = 0xf;
    *(int *)(msg + 0xc) = -1;
    *(int *)(msg + 8) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c);
    msg[0x36] = 2;
    *(int *)(msg + 0x60) = *(int *)(ev + 0xb0);
    *(int *)(msg + 0x5c) = *(int *)(ev + 0x10);
    *(int *)(msg + 0x58) = *(int *)(ev + 0xac);
    *(int *)(msg + 0xa0) = *(int *)(ev + 4);
    if (gm_FindObjectInst(*(int *)(ev + 0xac)) != 0) {
        if (*(char *)(ev + 0xf1) != 0)
            gm_AddTime(ev + 0x52, 1, 0x1e, 0);
        else
            *(int *)(ev + 0xec) = cm_SendLongMessage(msg);
    } else
        he_Unk53e1f0((int *)ev);
    *(unsigned char *)(ev + 0xf0) = 0;
    if (gm_FindChar(*(int *)(ev + 0xb4)) != 0)
        *(unsigned char *)(ev + 0xf0) = 4;
}
