/* 0x4ad3b0, the election step message: sends sub type 0x14 with v (the election index, -1 for none) */

void hmt_SendElectionMessage(int v)
{
    int msg[31];

    msg[0] = v;
    cm_SendExSysMessage(0x14, msg);
}
