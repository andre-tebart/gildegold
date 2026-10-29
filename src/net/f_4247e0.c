/* 0x4247e0, clears the network statistics: the per-packet-type table net_Stats and the received / sent packet and byte totals */

void net_ClearStats(void)
{
    memset(net_Stats, 0, sizeof(net_Stats));
    net_RecvPackets = 0;
    net_RecvBytes = 0;
    net_SentPackets = 0;
    net_SentBytes = 0;
}
