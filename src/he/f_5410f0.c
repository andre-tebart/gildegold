/* 0x5410f0, posts the event 0x73 for the player of 0x6f1bf8 (replacing a posted one) */

void he_Unk5410f0(void)
{
    char ev[0xf8];
    int r;

    r = (int)he_FindEvent(1, 0, 0x73);
    if (r != 0)
        he_Unk53e1f0((int *)r);
    HM_B(ev, 4) = 0x73;
    HM_I(ev, 8) = *(int *)(*(int *)(unsigned)&ai_PlayerChar + 4);
    HM_I(ev, 0x2c) = U_I((unsigned)&ai_TimeHour);
    HM_I(ev, 0x28) = U_I((unsigned)&ai_Time);
    HM_I(ev, 0xc) = -1;
    HM_B(ev, 0x36) = 1;
    HM_I(ev, 0x30) = U_I(((unsigned)&ai_Time + 0x8));
    *(short *)(ev + 0x34) = *(short *)((unsigned)&ai_Time + 0xc);
    cm_SendLongMessage(ev);
}
