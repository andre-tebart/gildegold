/* 0x4ad190, the object type of election hmt_PendingElection (0..2), 0 otherwise */

int hmt_GetElectionObjType(void)
{
    if ((unsigned int)hmt_PendingElection >= 3)
        return 0;
    return hmt_ElectionObjTypes[hmt_PendingElection];
}
