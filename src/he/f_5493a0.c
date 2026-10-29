/* 0x5493a0, the init of the event 0x5e: looks for a running event of the type 0x5e of the player (0x53e400 (2, 0, 0x5e, 2, player)); none: state 0, +0xb8 = 0 and the event is posted, else the clock is set and it is posted as failed */

void he_Unk5493a0(int ev)
{
    int e = (int)he_FindEvent(2, 0, 0x5e, 2, *(unsigned short *)(ev + 8));

    while (1) {
        if (e == 0) {
            *(int *)(ev + 0x70) = 0;
            *(char *)(ev + 0xb8) = 0;
            *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
            return;
        }
        if (e != ev)
            break;
        e = (int)he_NextEvent();
    }
    *(HETIME *)(ev + 0x52) = ai_Time;
    *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
}
