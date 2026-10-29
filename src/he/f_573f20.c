/* 0x573f20, the alloc of the event 0x9e: state 0, the start and the due clock +0x44 / +0x52 = now, the due clock + 24 hours
   (0x4fe140), the hour +0x56 = 6 and the minute +0x58 = 5 + random (0x10) */

void he_Unk573f20(int ev)
{
    HM_I(ev, 0x70) = 0;
    *(HETIME *)(ev + 0x44) = ai_Time;
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0x18, 0, 0);
    *(unsigned short *)(ev + 0x56) = 6;
    HM_I(ev, 0x58) = (util_Random(0x10) & 0xffff) + 5;
}
