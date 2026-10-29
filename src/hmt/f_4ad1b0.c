/* 0x4ad1b0, sets the election to decide next (0..2, -1 for none; message type 20, 0x4ad190 reads it) */

void hmt_SetPendingElection(int v)
{
    hmt_PendingElection = v;
}
