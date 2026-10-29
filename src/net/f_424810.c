/* 0x424810, counts one received (recv != 0) or sent packet in net_Stats[pkt->type]: its packet count and byte count; a null packet, a type >= NET_TYPES or a size not above the type is ignored */

void net_Count(NETPKT *pkt, unsigned int size, char recv)
{
    if (pkt) {
        if (size > pkt->type && pkt->type < NET_TYPES) {
            int k = recv ? 0 : 2;
            ((unsigned int *)&net_Stats[pkt->type])[k]++;
            ((unsigned int *)&net_Stats[pkt->type])[k + 1] += size;
        }
    }
}
