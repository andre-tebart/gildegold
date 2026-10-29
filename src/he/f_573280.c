/* 0x573280, the alloc of the event 0xa2 (the daily Hanseatic League politics): nothing while paused (+0x78 & 4) or while the
   hour (ai_Time.b) is 0xa or less; else the due clock +0x52 = now + 24 hours (0x4fe140), its hour +0x56 = 0xa, the minute
   +0x58 and the second +0x5c = 0, state 0 */

void he_Unk573280(int ev)
{
    if (HM_B(ev, 0x78) & 4)
        return;
    if (ai_Time.b <= 0xa)
        return;
    gm_AddTime(ev + 0x52, 0x18, 0, 0);
    *(unsigned short *)(ev + 0x56) = 0xa;
    HM_I(ev, 0x58) = 0;
    HM_I(ev, 0x5c) = 0;
    HM_I(ev, 0x70) = 0;
}
