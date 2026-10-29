/* 0x5436a0, a Ghidra miss (disassembled): starts the group event: state 0, clock +0x52 in 0x18 units (state word 0x14); with +0xc0 == 1 every member figure (+0xac .. +0xb8) gets the message 0x36 carrying the event clock and id (flag 9 for the players, else 1), then posts the event (ROUGH) */

void he_Unk5436a0(int ev)
{
    char msg[0xf8];
    int *pp;
    int k, m;
    unsigned char fl;

    fl = *(unsigned char *)(ev + 0x78);
    *(int *)(ev + 0x70) = 0;
    if ((fl & 4) == 0) {
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0x18, 0, 0);
        *(short *)(ev + 0x56) = 0x14;
        *(int *)(ev + 0x58) = 0;
        if (*(int *)(ev + 0xc0) == 1) {
            pp = (int *)(ev + 0xac);
            for (k = 4; k != 0; k--, pp++) {
                m = gm_FindChar(*pp);
                if (m != 0) {
                    HM_B(msg, 4) = 0x36;
                    HM_I(msg, 8) = *(int *)(m + 4);
                    HM_I(msg, 0xc) = -1;
                    HM_I(msg, 0x68) = 1;
                    *(HETIME *)(msg + 0x58) = *(HETIME *)(ev + 0x52);
                    HM_I(msg, 0x6c) = *(int *)(ev + 4);
                    if (*(char *)(m + 2) == 6 || *(char *)(m + 2) == 7)
                        HM_B(msg, 0x36) = 9;
                    else
                        HM_B(msg, 0x36) = 1;
                    cm_SendLongMessage(msg);
                }
            }
        }
        cm_SendExSetHE(0, ev);
    }
}
