/* 0x424860, formats the network statistics into buf: first the receive / send totals, then one line per packet type with its share in percent; every sprintf overwrites buf (nothing is printed here) */

void net_PrintStats(void)
{
    char buf[256];
    unsigned int i;

    sprintf(buf, "Recv: %u Packets w/ %u bytes, Send: %u Packets w/ %u bytes",
            net_RecvPackets, net_RecvBytes, net_SentPackets, net_SentBytes);
    for (i = 0; i < NET_TYPES; i++)
        sprintf(buf, "%u. Recv: %u Packets (%02f%%) w/ %u bytes (%02f%%), Send: %u Packets (%02f%%) w/ %u bytes (%02f%%)", i,
                net_Stats[i].recvpk, (net_Stats[i].recvpk * 100.0) / net_RecvPackets,
                net_Stats[i].recvbytes, (net_Stats[i].recvbytes * 100.0) / net_RecvBytes,
                net_Stats[i].sentpk, (net_Stats[i].sentpk * 100.0) / net_SentPackets,
                net_Stats[i].sentbytes, (net_Stats[i].sentbytes * 100.0) / net_SentBytes);
}
