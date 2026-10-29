/* 0x401d50 */
void udp_Init(u_short port)
{
    WSADATA wsa;
    HLOCAL msg;
    int on = 1;

    if (udp_Socket == INVALID_SOCKET) {
        WSAStartup(2, &wsa);
        udp_Socket = socket(AF_INET, SOCK_DGRAM, 0);
        if (udp_Socket == INVALID_SOCKET) {
            GetLastError();
            if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                               NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&msg, 0, NULL))
                LocalFree(msg);
        } else {
            setsockopt(udp_Socket, SOL_SOCKET, SO_BROADCAST, (char *)&on, sizeof(on));
            udp_Local.sin_family = AF_INET;
            udp_Local.sin_addr.s_addr = htonl(INADDR_ANY);
            udp_Local.sin_port = htons(0);
            bind(udp_Socket, (struct sockaddr *)&udp_Local, sizeof(udp_Local));
            udp_Bcast.sin_family = AF_INET;
            udp_Bcast.sin_addr.s_addr = htonl(INADDR_BROADCAST);
            udp_Bcast.sin_port = htons(port);
        }
    }
}
