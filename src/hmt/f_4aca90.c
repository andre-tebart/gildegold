/* 0x4aca90, loads historical.dfn into hmt_File; the current person / event become the first ones @deep code hunks 12: the original keeps hmt_File.p / .k in ecx / edx after the call, ours in eax / edx (same statements and order); register allocation (match-deep; ret local and !x tried before) */

void hmt_Init(void)
{
    hmt_CurrentPerson = -1;
    hmt_CurrentEvent = -1;
    hmt_Person = -1;
    hmt_Event = -1;
    hmt_PendingElection = -1;
    if (hmt_ReadBinaryFile(&hmt_File) == 0) {
        hmt_Person = hmt_File.p->idx;
        hmt_Event = hmt_File.k->idx;
    }
}
