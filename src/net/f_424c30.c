/* 0x424c30, closes the connection if one is open (shutdown both directions, closesocket) and marks it closed; then WSACleanup clears net_WSAStarted */

void net_Close(void)
{
    if (net_Conn.s != INVALID_SOCKET) {
        shutdown(net_Conn.s, SD_BOTH);
        closesocket(net_Conn.s);
        net_Conn.s = INVALID_SOCKET;
        net_Conn.closed = 1;
    }
    WSACleanup();
    net_WSAStarted = 0;
}
