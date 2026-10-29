/* 0x571f60, the alloc of the event 0x90: paused -> nothing; state 0; the due clock +0x52 = now; 0x53e400 (2, 0, 0x90, 3, +0x10) finds another running event 0x90 of the building -> state -1 (0x4c4c60); else the message 0x91 (+0x28 the clock, +0x36 0xa, +0x58 +4, +0x5c -1, +0x60 +0xb0) through 0x4c4b70, +0xb4 / +0xb0 capped at 2, +0xb8 = 0, 0x50cc50 (pid, -(+0xb4 + 2) * (+0xb0 + 2)), due now + 5 hours, state 0 */

void he_Unk571f60(int ev)
{
    char msg[0xf8];
    char *r;

    if (*(unsigned char *)(ev + 0x78) & 4)
        return;
    HM_I(ev, 0x70) = 0;
    *(HETIME *)(ev + 0x52) = ai_Time;
    for (r = he_FindEvent(2, 0, 0x90, 3, HM_I(ev, 0x10)); r != 0; r = he_NextEvent()) {
        if ((int)r != ev) {
            HM_I(ev, 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
    }
    memset(msg, 0, 0xf8);
    HM_B(msg, 4) = 0x91;
    HM_I(msg, 8) = HM_I(ev, 0xc);
    HM_I(msg, 0xc) = HM_I(ev, 0xac);
    *(HETIME *)(msg + 0x28) = ai_Time;
    HM_B(msg, 0x36) = 0xa;
    HM_I(msg, 0x58) = HM_I(ev, 4);
    HM_I(msg, 0x5c) = -1;
    HM_I(msg, 0x60) = HM_I(ev, 0xb0);
    cm_SendLongMessage(msg);
    if (HM_I(ev, 0xb4) > 2)
        HM_I(ev, 0xb4) = 2;
    if (HM_I(ev, 0xb0) > 2)
        HM_I(ev, 0xb0) = 2;
    HM_I(ev, 0xb8) = 0;
    gm_Unk50cc50(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -((HM_I(ev, 0xb4) + 2) * (HM_I(ev, 0xb0) + 2)));
    gm_AddTime(ev + 0x52, 0, 5, 0);
    HM_I(ev, 0x84) = cm_SendExSetHE(0, ev);
}
