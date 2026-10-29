/* 0x5496f0, the init of the event 0x5f (like 0x5493a0, without the posting) */

void he_Unk5496f0(int ev)
{
    int e = (int)he_FindEvent(2, 0, 0x5f, 2, *(unsigned short *)(ev + 8));

    while (1) {
        if (e == 0) {
            *(int *)(ev + 0x70) = 0;
            *(char *)(ev + 0xba) = 0;
            return;
        }
        if (e != ev)
            break;
        e = (int)he_NextEvent();
    }
    *(HETIME *)(ev + 0x52) = ai_Time;
    *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
}
