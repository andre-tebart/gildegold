/* 0x424990, opens a nonblocking TCP connection to host:port: clears the stats and the connection, WSAStartup, a socket with the buffer options and nonblocking mode, the address from the IP literal or gethostbyname, connect (WSAEWOULDBLOCK accepted), then waits up to 16 * 0.5 s for the socket to become writable; stores the connection and returns the socket on success, else INVALID_SOCKET (fail: shutdown, closesocket; cleanup: WSACleanup) */

SOCKET net_Connect(char *host, unsigned short port)
{
    u_long opt;
    int len;
    struct timeval tv;
    struct sockaddr_in addr;
    char buf[128];
    fd_set fds;
    WSADATA wsa;
    SOCKET s;
    struct hostent *he;
    int i, r;

    net_ClearStats();
    net_Conn.s = INVALID_SOCKET;
    net_Conn.rx = NULL;
    net_Conn.tx = NULL;
    net_Conn.rxlen = 0;
    net_Conn.txlen = 0;
    net_Conn.closed = 0;
    if (host == NULL)
        return INVALID_SOCKET;

    WSAStartup(2, &wsa);
    net_WSAStarted = 1;
    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET)
        goto cleanup;
    opt = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 4) == SOCKET_ERROR)
        goto fail;
    opt = 1;
    if (setsockopt(s, SOL_SOCKET, SO_DONTLINGER, (char *)&opt, 4) == SOCKET_ERROR)
        goto fail;
    opt = 0x40000;
    if (setsockopt(s, SOL_SOCKET, SO_RCVBUF, (char *)&opt, 4) == SOCKET_ERROR)
        goto fail;
    opt = 0x20000;
    if (setsockopt(s, SOL_SOCKET, SO_SNDBUF, (char *)&opt, 4) == SOCKET_ERROR)
        goto fail;
    opt = 1;
    if (ioctlsocket(s, FIONBIO, &opt) == SOCKET_ERROR)
        goto fail;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host);
    if (addr.sin_addr.s_addr == INADDR_NONE) {
        he = gethostbyname(host);
        if (he == NULL)
            goto fail;
        memcpy(&addr.sin_addr, he->h_addr_list[0], sizeof(addr.sin_addr));
    }
    addr.sin_port = htons(port);
    if (connect(s, (struct sockaddr *)&addr, sizeof(addr)) != 0 && WSAGetLastError() != WSAEWOULDBLOCK) {
        net_Error(WSAGetLastError());
        goto fail;
    }
    tv.tv_sec = 0;
    tv.tv_usec = 500000;
    for (i = 0; i < 16; i++) {
        fds.fd_array[0] = s;
        fds.fd_count = 1;
        r = select(1, NULL, &fds, NULL, &tv);
        if (r == SOCKET_ERROR)
            break;
        if (r != 0) {
            len = 4;
            if (getsockopt(s, SOL_SOCKET, SO_SNDBUF, (char *)&opt, &len) != SOCKET_ERROR)
                sprintf(buf, "getsockopt SO_SNDBUF: %d", opt);
            len = 4;
            if (getsockopt(s, SOL_SOCKET, SO_RCVBUF, (char *)&opt, &len) != SOCKET_ERROR)
                sprintf(buf, "getsockopt SO_RCVBUF: %d", opt);
            net_Conn.s = s;
            return s;
        }
    }
    net_Error(WSAGetLastError());
fail:
    shutdown(s, SD_BOTH);
    closesocket(s);
cleanup:
    net_WSAStarted = 0;
    WSACleanup();
    return INVALID_SOCKET;
}
