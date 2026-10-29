/* 0x5cb1e0, the counter-espionage message wrapper (64 B): 0x4e6ba0 (param_1, param_2, param_3) sends the
   request; -1 returns 0, other types return 1; type 6 waits 0x4c3ae0 / 0x52fe00 and returns whether the
   last status is not 2. */

int amt_SendOfficeSwap();

int ai_Unk5cb1e0(int param_1, int param_2, int param_3)
{
    int h;

    h = amt_SendOfficeSwap(param_1, param_2, param_3);
    if (h == -1)
        return 0;
    if (*(char *)(param_1 + 2) != 6)
        return 1;
    while (cm_GetSlotState(h) == 0)
        sv_PumpNet();
    return cm_GetSlotState(h) != 2;
}
