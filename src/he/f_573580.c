/* 0x573580, the run of the event 0x9d: -2 / -1 free; nothing while paused or while the pending handle +0x84 is not done
   (0x4c3ae0); then +0x84 = -1, the due clock +0x52 = now, the figure +0xb0 (0x505df0) and the player record (0x770b80) must
   exist (else scheduled with -1): state 0 calls 0x5736d0 (player, figure, ev +0xac) and retries in 24 hours with the hour 6
   and the minute 15 * random (4) + 5; state 1 calls 0x573e30 (player, ev +0xac) and schedules -1 */

void he_Unk5736d0(char *pl, char *per, int a);  /* 0x5736d0 */
void he_Unk573e30(char *pl, int a);             /* 0x573e30 */

void he_Unk573580(int ev)
{
    char *per, *pl;

    if (HM_I(ev, 0x70) == -1 || HM_I(ev, 0x70) == -2) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (HM_B(ev, 0x78) & 4)
        return;
    if (HM_I(ev, 0x84) != -1 && cm_GetSlotState(HM_I(ev, 0x84)) == 0)
        return;
    HM_I(ev, 0x84) = -1;
    *(HETIME *)(ev + 0x52) = ai_Time;
    per = (char *)gm_FindChar(HM_I(ev, 0xb0));
    if (per == 0 || *(unsigned short *)((unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c) == 0xffff) {
        HM_I(ev, 0x84) = cm_SendExSetHE(-1, ev);
        return;
    }
    pl = (char *)((unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c);
    switch (HM_I(ev, 0x70)) {
    case 0:
        he_Unk5736d0(pl, per, HM_I(ev, 0xac));
        gm_AddTime(ev + 0x52, 0x18, 0, 0);
        *(unsigned short *)(ev + 0x56) = 6;
        HM_I(ev, 0x58) = (util_Random(4) & 0xffff) * 0xf + 5;
        HM_I(ev, 0x84) = cm_SendExSetHE(1, ev);
        return;
    case 1:
        he_Unk573e30(pl, HM_I(ev, 0xac));
        HM_I(ev, 0x84) = cm_SendExSetHE(-1, ev);
        return;
    }
}
