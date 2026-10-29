/* 0x541060, posts the event 0x3b for the current player (replacing a posted one) */

void he_Unk541060(void)
{
    char ev[0xf8];
    int r;

    r = (int)he_FindEvent(1, 0, 0x3b);
    if (r != 0)
        he_Unk53e1f0((int *)r);
    HM_I(ev, 0x28) = U_I((unsigned)&ai_Time);
    HM_B(ev, 4) = 0x3b;
    HM_I(ev, 0xc) = -1;
    HM_B(ev, 0x36) = 1;
    *(short *)(ev + 0x34) = *(short *)((unsigned)&ai_Time + 0xc);
    HM_I(ev, 8) = U_I((unsigned)&ai_CharInts + (unsigned short)U_I((unsigned)&ai_PlayerId) * 0x21c);
    HM_I(ev, 0x2c) = U_I((unsigned)&ai_TimeHour);
    HM_I(ev, 0x30) = U_I(((unsigned)&ai_Time + 0x8));
    cm_SendLongMessage(ev);
}
