/* 0x56fda0, the run of the event 0x88 (a work at the building +0xb0 for the character +0xb4): -1 / -2 release the worker (0x524260) and free; paused or with the handle +0x84 still active nothing. State 0: the owner -1 (0x4c3ae0), the worker registered, the building of +0xac (0x501190) -> its +1 to 0x4c52e0 (state 2), else the state -1. State 2: +0xbc = -1; the job +0x128 of the character or the building missing -> wait 4 hours and state 2; else 0x4fe140 (+0x18) and 0x5a9910 (the record of the player +8, the worker, the due clock), state 3. State 3: the boss +0x16c -> 0x4c52e0 (state 4), else state -1. State 4: no boss -> state -1; work in progress -> wait 4 hours; else 0x524260, state 5 and the due clock += (max (+0xb8 / 2, 1) + 1). State 5: state -1 (0x4c4c60). ROUGH */

int ai_Unk5a9910();                 /* 0x5a9910 */

void he_Unk56fda0(int ev)
{
    char *per = (char *)gm_FindChar(HM_I(ev, 0xb4));
    int state, r;

    if (per == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    state = HM_I(ev, 0x70);
    if (state == -1 || state == -2) {
        sv_MoveToGroup(0, 0, *(unsigned short *)per, ev);
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (*(unsigned char *)(ev + 0x78) & 4)
        return;
    if (HM_I(ev, 0x84) != -1 && cm_GetSlotState(HM_I(ev, 0x84)) == 0)
        return;
    HM_I(ev, 0x84) = -1;
    switch (state) {
    case 0:
        if (cm_GetSlotState(-1) == 0)
            return;
        sv_MoveToGroup(0, 0, *(unsigned short *)per, ev);
        r = gm_BuildQuery(1, 1, HM_I(ev, 0xac));
        if (r == 0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            HM_I(ev, 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        HM_I(ev, 0xbc) = ai_Unk4c52e0(HM_I(per, 4), *(int *)(r + 1), -1, 0, 1, (char *)(unsigned)&full_data_628e98);
        HM_I(ev, 0x84) = cm_SendExSetHE(2, ev);
        return;
    case 1:
        return;
    case 2:
        HM_I(ev, 0xbc) = -1;
        if (HM_I(HM_I(per, 0x184), 0x128) != 0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 4, 0);
            HM_I(ev, 0x84) = cm_SendExSetHE(2, ev);
            return;
        }
        if (gm_BuildQuery(1, 1, HM_I(ev, 0xac)) != 0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            r = gm_FindChar(HM_I(ev, 0xb0));
            if (r != 0) {
                gm_AddTime(ev + 0x52, 0x18, 0, 0);
                ai_Unk5a9910((unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c, r, ev + 0x52);
            }
            HM_I(ev, 0x84) = cm_SendExSetHE(3, ev);
            return;
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        HM_I(ev, 0x84) = cm_SendExSetHE(-1, ev);
        return;
    case 3:
        if (HM_I(per, 0x16c) == 0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            HM_I(ev, 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        HM_I(ev, 0xbc) = ai_Unk4c52e0(HM_I(per, 4), *(int *)(HM_I(per, 0x16c) + 1), -1, 0, 0, (char *)(unsigned)&full_data_628e98);
        HM_I(ev, 0x84) = cm_SendExSetHE(4, ev);
        return;
    case 4:
        if (HM_I(per, 0x16c) == 0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            HM_I(ev, 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        if (HM_I(HM_I(per, 0x184), 0x128) != 0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 4, 0);
            return;
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        sv_MoveToGroup(0, 0, *(unsigned short *)per, ev);
        HM_I(ev, 0x84) = cm_SendExSetHE(5, ev);
        r = HM_I(ev, 0xb8) >> 1;
        if (r < 1)
            r = 1;
        HM_I(ev, 0x52) += r + 1;
        return;
    case 5:
        HM_I(ev, 0x84) = cm_SendExSetHE(-1, ev);
        return;
    }
}
