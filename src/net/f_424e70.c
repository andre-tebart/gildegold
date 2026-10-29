/* 0x424e70 */

int net_Send(void)
{
    int n;

    if (net_Conn.s == INVALID_SOCKET)
        return 1;
    if (!net_Conn.tx)
        return 0;
    {
        n = send(net_Conn.s, (char *)net_Conn.tx + net_Conn.txlen, net_Conn.tx->len - net_Conn.txlen, 0);
        if (n == SOCKET_ERROR) {
            n = WSAGetLastError();
            if (n != WSAEWOULDBLOCK) {
                net_Error(n);
                net_Close();
                return 1;
            }
        } else {
            net_Conn.txlen += n;
            net_SentBytes += n;
            if (net_Conn.txlen == net_Conn.tx->len) {
                net_Count(net_Conn.tx, net_Conn.txlen, 0);
                net_Conn.tx = NULL;
                net_Conn.txlen = 0;
            }
        }
    }
    return 0;
}
