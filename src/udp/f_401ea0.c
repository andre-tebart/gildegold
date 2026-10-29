/* 0x401ea0, collects the servers that answer on port within timeout ms; returns their count */

int udp_FindServers(u_short port, DWORD timeout, UDPSERVER *list, int max)
{
    WSADATA wsa;
    UDPPACKET pkt;
    struct sockaddr_in from;
    struct sockaddr_in addr;
    DWORD start;
    int fromlen;
    u_long nonblock = 1;
    SOCKET s;
    int n, i;

    start = timeGetTime();
    n = 0;
    if (max <= 0)
        return -2;
    ZERO_AT(list, max * sizeof(UDPSERVER));
    if (WSAStartup(2, &wsa) == SOCKET_ERROR) {
        fprintf(stderr, "WSAStartup failed with error %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = 0;
    addr.sin_port = htons(port);
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s == INVALID_SOCKET) {
        WSACleanup();
        return -1;
    }
    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
        WSACleanup();
        return -1;
    }
    if (ioctlsocket(s, FIONBIO, &nonblock) == SOCKET_ERROR) {
        WSACleanup();
        return -1;
    }
    while (timeGetTime() - start < timeout && n < max) {
        fromlen = sizeof(from);
        if ((unsigned)recvfrom(s, (char *)&pkt, sizeof(pkt), 0, (struct sockaddr *)&from, &fromlen) >= 6
            && pkt.magic == 0x6a && (pkt.version == 1 || pkt.version == 4) && pkt.type == 1) {
            int found = 0;
            for (i = 0; i < n; i++) {
                if (!strcmp(list[i].ip, inet_ntoa(from.sin_addr))) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                strcpy(list[n].ip, inet_ntoa(from.sin_addr));
                if (pkt.version == 1) {
                    list[n].info.v1 = pkt.v1;
                    list[n].flags = 0x3f;
                } else {
                    list[n].info.v4 = pkt.v4;
                    list[n].flags = 0x7f;
                }
                n++;
            }
        }
    }
    closesocket(s);
    WSACleanup();
    return n;
}
