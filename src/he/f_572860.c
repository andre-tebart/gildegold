/* 0x572860, the alloc of the events 0x8b and 0x92: the due clock +0x52 = now; the minute word +0x56 >= 0x17 -> due + 0x18 hours and the minute word = 0xc, else due + 6 hours; state 0 */

void he_Unk572860(int ev)
{
    *(HETIME *)(ev + 0x52) = ai_Time;
    if (*(unsigned short *)(ev + 0x56) >= 0x17) {
        gm_AddTime(ev + 0x52, 0x18, 0, 0);
        *(unsigned short *)(ev + 0x56) = 0xc;
    } else
        gm_AddTime(ev + 0x52, 6, 0, 0);
    HM_I(ev, 0x70) = 0;
}
