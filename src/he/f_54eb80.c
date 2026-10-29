/* 0x54eb80, a Ghidra miss: the figure record test of the street simulation: alive, kind below 10, and (0x47e840 or 0x47e7f0 non zero) */

int he_Unk54eb80(int rec)
{
    if (*(unsigned short *)rec == 0xffff)
        return 0;
    if (*(unsigned char *)(rec + 8) == 0)
        return 0;
    if (*(unsigned char *)(rec + 2) >= 10)
        return 0;
    if (ai_IsLocalWorker(rec) != 0)
        return 1;
    return ai_CanAct(rec) != 0;
}
