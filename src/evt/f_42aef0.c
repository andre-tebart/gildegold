/* 0x42aef0 */

void evt_StepAll(void)
{
    int i;

    for (i = 0; i < 128; i++) {
        if (evt_Scripts[i].flags & 1) {
            evt_Cur = &evt_Scripts[i];
            evt_Step(evt_Cur);
        }
    }
    evt_Cur = 0;
}
