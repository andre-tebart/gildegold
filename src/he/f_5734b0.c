/* 0x5734b0, the alloc of the event 0x9d: state 0; nothing while paused (+0x78 & 4); both clocks +0x44 / +0x52 = now; the
   figure +0xb0 (0x505df0): without one the event is scheduled with -1; else the message 0x4f2e30 (0x1c, 1, the player id
   +0xc, figure +4, *(figure + 0x170) ? the word after it : -1) and the handler scheduled with 0 */

void he_Unk5734b0(int ev)
{
    char *per;
    int x;

    HM_I(ev, 0x70) = 0;
    if (HM_B(ev, 0x78) & 4)
        return;
    *(HETIME *)(ev + 0x44) = ai_Time;
    *(HETIME *)(ev + 0x52) = ai_Time;
    per = (char *)gm_FindChar(HM_I(ev, 0xb0));
    if (per == 0) {
        HM_I(ev, 0x84) = cm_SendExSetHE(-1, ev);
        return;
    }
    x = HM_I(per, 0x170);
    if (x != 0)
        x = HM_I(x, 1);
    else
        x = -1;
    unk_Unk4f2e30(0x1c, 1, HM_I(ev, 0xc), HM_I(per, 4), x);
    HM_I(ev, 0x84) = cm_SendExSetHE(0, ev);
}
