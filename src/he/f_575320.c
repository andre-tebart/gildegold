/* 0x575320, the alloc of the event 0xa4: state 0, the clocks +0x44 / +0x52 = now, the due clock + random (0x20) hours (0x4fe140) */

void he_Unk575320(int ev)
{
    *(HETIME *)(ev + 0x44) = ai_Time;
    *(HETIME *)(ev + 0x52) = ai_Time;
    HM_I(ev, 0x70) = 0;
    gm_AddTime(ev + 0x52, util_Random(0x20) & 0xffff, 0, 0);
}
