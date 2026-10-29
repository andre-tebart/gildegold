/* 0x5cfb60, a window callback (24 B): the record param_1 (+4 the handle) is closed with 0x50cc50
   (the handle, 2); returns 1. */

int ai_Unk5cfb60(int param_1)
{
    gm_SendMoneyDelta(*(int *)(param_1 + 4), 2);
    return 1;
}
