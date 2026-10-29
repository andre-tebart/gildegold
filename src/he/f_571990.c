/* 0x571990, the alloc of the event 0x8e: the due clock +0x52 = now + 0x48 hours, state 0 */

void he_Unk571990(int ev)
{
    *(HETIME *)(ev + 0x52) = ai_Time;
    HM_I(ev, 0x70) = 0;
    gm_AddTime(ev + 0x52, 0x48, 0, 0);
}
