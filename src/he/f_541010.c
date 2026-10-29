/* 0x541010, posts the event 0x3a for the current player (flag 1) */

void he_Unk541010(void)
{
    char ev[0xf8];

    HM_B(ev, 4) = 0x3a;
    HM_I(ev, 0xc) = -1;
    HM_B(ev, 0x36) = 1;
    HM_I(ev, 8) = U_I((unsigned)&ai_CharInts + (unsigned short)U_I((unsigned)&ai_PlayerId) * 0x21c);
    cm_SendLongMessage(ev);
}
