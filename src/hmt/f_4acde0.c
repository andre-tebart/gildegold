/* 0x4acde0, frees hmt_File */

int hmt_Exit(void)
{
    if (hmt_File.k != 0) {
        mem_Free(hmt_File.k);
        hmt_File.k = 0;
    }
    if (hmt_File.p != 0) {
        mem_Free(hmt_File.p);
        hmt_File.p = 0;
    }
    hmt_File.np = 0;
    hmt_File.nk = 0;
    hmt_CurrentPerson = -1;
    hmt_CurrentEvent = -1;
    hmt_Person = -1;
    hmt_Event = -1;
    hmt_PendingElection = -1;
    return 0;
}
