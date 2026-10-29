/* 0x553e10, sets the due clock of the event record to now and posts it: with the flag 0x6f2448 one unit later, else after the delay (+0x22) of its table record (0x91aaf0, 0x41 bytes each, index +0xac) */

void he_Unk553e10(int ev)
{
    char *rec = (char *)U_I((unsigned)&ai_Goods) + *(unsigned short *)(ev + 0xac) * 0x41;

    *(HETIME *)(ev + 0x52) = ai_Time;
    if (U_I((unsigned)&ai_Unk6f2448) != 0)
        gm_AddTime(ev + 0x52, 0, 0, 1);
    else
        gm_AddTime(ev + 0x52, 0, *(int *)(rec + 0x22), 0);
}
