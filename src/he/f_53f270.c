/* 0x53f270, posts the events 0x86 and 0xa2 (once each) for the player of 0x6f1bf8 */

void he_Unk53f270(void)
{
    char ev[0xf8];

    if (he_FindEvent(1, 0, 0x86) == 0) {
        memset(ev, 0, 0xf8);
        HM_B(ev, 4) = 0x86;
        HM_I(ev, 8) = *(int *)(*(int *)(unsigned)&ai_PlayerChar + 4);
        HM_I(ev, 0x2c) = U_I((unsigned)&ai_TimeHour);
        HM_I(ev, 0x28) = U_I((unsigned)&ai_Time);
        HM_I(ev, 0xc) = -1;
        HM_B(ev, 0x36) = 2;
        HM_I(ev, 0x30) = U_I(((unsigned)&ai_Time + 0x8));
        *(short *)(ev + 0x34) = *(short *)((unsigned)&ai_Time + 0xc);
        cm_SendLongMessage(ev);
    }
    if (he_FindEvent(1, 0, 0xa2) == 0) {
        memset(ev, 0, 0xf8);
        HM_B(ev, 4) = 0xa2;
        HM_I(ev, 8) = *(int *)(*(int *)(unsigned)&ai_PlayerChar + 4);
        HM_I(ev, 0x2c) = U_I((unsigned)&ai_TimeHour);
        HM_I(ev, 0x28) = U_I((unsigned)&ai_Time);
        HM_I(ev, 0xc) = -1;
        HM_B(ev, 0x36) = 2;
        HM_I(ev, 0x30) = U_I(((unsigned)&ai_Time + 0x8));
        *(short *)(ev + 0x34) = *(short *)((unsigned)&ai_Time + 0xc);
        cm_SendLongMessage(ev);
    }
}
