/* 0x5732d0, the run of the event 0xa2 (the daily Hanseatic League politics): nothing while paused (+0x78 & 4); the state
   +0x70 0 .. 4 picks the step, each schedules the next state with 0x4c4c60: 0 hmt_Update and +2 hours, 1 hmt_Begin and
   +2 hours, 2 hmt_Election and the time of day 0x16 (0x4fe1e0), 3 hmt_ClearOffices and the time of day 6:05 with the day +1,
   4 hmt_Unk4ad580 and the time of day 0xa */

void hmt_Update();                 /* 0x4ace40, hmt_Update */
void hmt_Begin();                 /* 0x4ad0a0, hmt_Begin */
void hmt_Election();                 /* 0x4ad1c0, hmt_Election */
void hmt_ClearOffices(void); /* 0x4ad510 */
void hmt_Unk4ad580();                 /* 0x4ad580 */

void he_Unk5732d0(int ev)
{
    int *c;

    if (HM_B(ev, 0x78) & 4)
        return;
    switch (HM_I(ev, 0x70)) {
    case 0:
        hmt_Update();
        gm_AddTime(ev + 0x52, 2, 0, 0);
        cm_SendExSetHE(1, ev);
        return;
    case 1:
        hmt_Begin();
        gm_AddTime(ev + 0x52, 2, 0, 0);
        cm_SendExSetHE(2, ev);
        return;
    case 2:
        hmt_Election();
        gm_SetTime(ev + 0x52, 0x16, 0, 0);
        cm_SendExSetHE(3, ev);
        return;
    case 3:
        hmt_ClearOffices();
        gm_SetTime(ev + 0x52, 6, 5, 0);
        c = (int *)(ev + 0x52);
        *c = *c + 1;
        cm_SendExSetHE(4, ev);
        return;
    case 4:
        hmt_Unk4ad580();
        gm_SetTime(ev + 0x52, 0xa, 0, 0);
        cm_SendExSetHE(0, ev);
        return;
    }
}
