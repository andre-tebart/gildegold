/* 0x575150, the alloc of the event 0xa3: state 0, the start and the due clock +0x44 / +0x52 = now, the due clock plus random (2)
   hours and random (0x1e) minutes (0x4fe140) */

void he_Unk575150(int ev)
{
    *(HETIME *)(ev + 0x44) = ai_Time;
    *(HETIME *)(ev + 0x52) = ai_Time;
    HM_I(ev, 0x70) = 0;
    gm_AddTime(ev + 0x52, util_Random(2) & 0xffff, util_Random(0x1e) & 0xffff, 0);
}
