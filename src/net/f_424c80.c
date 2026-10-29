/* 0x424c80, reads the current packet from the connection (first the 3-byte header in pieces, then the rest), counts it, checks the sequence (a gap logs "Lost a Command" and resyncs net_Seq; the last sent Sync's count cm_LastSyncCount is tolerated) and hands the packet to net_Count; returns 1 when the connection is lost (EOF / error with Close), else 0 @deep behaviour: no meaning difference against the Ghidra reading (the seq bookkeeping sets net_Seq with the same values in every branch, the two recv calls, net_Count and the rxlen clear are the same); the code hunks are register allocation (ecx against ebx/eax around rxlen += n / net_RecvBytes += n), the block order of the three sprintf branches and the dword clear of rxlen (ours stores rxlen as a word, the original clears the low word of the rxlen/txlen dword), plus 7 tail nops; readability met */

int net_Recv(void)
{
    char buf[256];
    int n;

    if (net_Conn.s == INVALID_SOCKET)
        goto ret1;
    if (net_Conn.rx == NULL)
        return 0;
    if (net_Conn.rxlen < 3) {
        n = recv(net_Conn.s, (char *)net_Conn.rx + net_Conn.rxlen, 3 - net_Conn.rxlen, 0);
        if (n == 0)
            goto lost;
        if (n == SOCKET_ERROR)
            goto error;
        net_RecvBytes += n;
        net_Conn.rxlen += n;
        if (net_Conn.rxlen < 3)
            return 0;
    }
    n = recv(net_Conn.s, (char *)net_Conn.rx + net_Conn.rxlen, net_Conn.rx->len - net_Conn.rxlen, 0);
    if (n == 0) {
        net_Close();
        return 1;
    }
    if (n == SOCKET_ERROR) {
error:
        n = WSAGetLastError();
        if (n == WSAEWOULDBLOCK)
            return 0;
        net_Error(n);
lost:
        net_Close();
ret1:
        return 1;
    }
    net_Conn.rxlen += n;
    net_RecvBytes += n;
    if (net_Conn.rxlen != net_Conn.rx->len)
        return 0;
    if (net_Conn.rx->idx != -1) {
        if (net_Conn.rx->seq != net_Seq + 1) {
            if (net_Conn.rx->seq == cm_LastSyncCount)
                sprintf(buf, "nt_Recv(): Received a Command with ID %d, expected %d", net_Conn.rx->seq, cm_LastSyncCount);
            else if (net_Conn.rx->type == ' ' && net_Conn.rx->cmd == 14)
                sprintf(buf, "nt_Recv(): Received Sync (cmd->CmdID %d, LastCmdID %d)", net_Conn.rx->seq, net_Seq);
            else {
                sprintf(buf, "nt_Recv(): Lost a Command (cmd->CmdID %d, LastCmdID %d)", net_Conn.rx->seq, net_Seq);
                net_Seq = net_Conn.rx->seq;
            }
        } else
            net_Seq = net_Conn.rx->seq;
    }
    net_Count(net_Conn.rx, net_Conn.rxlen, 1);
    net_Conn.rx = NULL;
    net_Conn.rxlen = 0;
    return 0;
}
