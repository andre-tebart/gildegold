/* 0x401e50, closes udp_Socket if it is open and marks it closed again (INVALID_SOCKET) */

void udp_Exit(void)
{
    if (udp_Socket != INVALID_SOCKET) {
        closesocket(udp_Socket);
        udp_Socket = INVALID_SOCKET;
    }
}
