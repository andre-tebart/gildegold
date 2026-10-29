/* 0x574120, the alloc of the event 0xa0: nothing while paused (+0x78 & 4); the start and the due clock +0x44 / +0x52 = now; when
   0x6f2448 is set the due clock + 1 (0x4fe140 (.., 1, 0, 0)), else + 24 hours with the hour +0x56 = 0x13 + random (2); the
   minute +0x58 = 0 and the handler scheduled with 0 */

void he_Unk574120(int ev)
{
    if (HM_B(ev, 0x78) & 4)
        return;
    *(HETIME *)(ev + 0x44) = ai_Time;
    *(HETIME *)(ev + 0x52) = ai_Time;
    if (U_I((unsigned)&ai_Unk6f2448) != 0) {
        gm_AddTime(ev + 0x52, 1, 0, 0);
    } else {
        gm_AddTime(ev + 0x52, 0x18, 0, 0);
        *(unsigned short *)(ev + 0x56) = util_Random(2) + 0x13;
    }
    HM_I(ev, 0x58) = 0;
    HM_I(ev, 0x84) = cm_SendExSetHE(0, ev);
}
