/* 0x54b9b0, a Ghidra miss: the end handler of the audit event: the due clock now, posted 2 units ahead, and the victim figure (+0xb0) gets the flag 0x800 (0x4c4a60 0x5a) */

void he_Unk54b9b0(int ev)
{
    int s = gm_BuildQuery(1, 1, *(int *)(ev + 0xb0));

    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 2, 0);
    if (s != 0)
        cm_SendSetField(*(int *)(s + 1), 0x5a, 2, 0x800, 0);
}
