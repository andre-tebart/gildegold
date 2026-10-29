/* 0x574060, the alloc of the event 0x9f: state -1, the start and the due clock +0x44 / +0x52 = now, the due clock + 24 hours
   (0x4fe140) */

void he_Unk574060(int ev)
{
    HM_I(ev, 0x70) = -1;
    *(HETIME *)(ev + 0x44) = ai_Time;
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0x18, 0, 0);
}
