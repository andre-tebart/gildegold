/* net
   unit of rea 8cef7d5b (src/game/net.c); boundaries in decl/text.dcl */
#line 1 "D:\\develop\\units\\net\\net.c"
/* net.c -- D:\develop\units\net\net.c (leaked assert path).
   TCP connection to the game server over WinSock: non-blocking connect, packet receive/send state
   machine, per packet type statistics. Extent in GildeGold.exe: 0x424510..0x424f30.
   Functions are kept in their original order. Names are ours.
   @named_by leaked
   @boundary plausible */
#include <stdio.h>
#include <winsock2.h>
#include "err.h"
#include "net.h"
#include "cm.h"

extern NETCONN net_Conn;

/* 0x424510, logs the Winsock error err as a fatal error (err_Log level 1) with the error's English explanation, or "Unknown winsock error <n>" for a code the switch does not know */
void net_Error(int err)
{
    char buf[256];

    switch (err) {
    case WSANOTINITIALISED:
#line 49
        err_Log(__FILE__, __LINE__, "WSANOTINITIALISED", 1);
        break;
    case WSAENETDOWN:
#line 51
        err_Log(__FILE__, __LINE__, "WSAENETDOWN: The network subsystem has failed!", 1);
        break;
    case WSAEFAULT:
#line 53
        err_Log(__FILE__, __LINE__, "WSAEFAULT: The buf parameter is not completely contained in a valid part of the user address space", 1);
        break;
    case WSAENOTCONN:
#line 55
        err_Log(__FILE__, __LINE__, "WSAENOTCONN: The socket is not connected.", 1);
        break;
    case WSAEINTR:
#line 57
        err_Log(__FILE__, __LINE__, "WSAEINTR: The (blocking) call was canceled through WSACancelBlockingCall.", 1);
        break;
    case WSAEINPROGRESS:
#line 59
        err_Log(__FILE__, __LINE__, "WSAEINPROGRESS: A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.", 1);
        break;
    case WSAENETRESET:
#line 61
        err_Log(__FILE__, __LINE__, "WSAENETRESET: The connection has been broken due to the keep-alive activity detecting a failure while the operation was in progress.", 1);
        break;
    case WSAENOTSOCK:
#line 63
        err_Log(__FILE__, __LINE__, "WSAENOTSOCK: The descriptor is not a socket.", 1);
        break;
    case WSAEOPNOTSUPP:
#line 65
        err_Log(__FILE__, __LINE__, "WSAEOPNOTSUPP: MSG_OOB was specified, but the socket is not stream-style such as type SOCK_STREAM, OOB data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only send operations.", 1);
        break;
    case WSAESHUTDOWN:
#line 67
        err_Log(__FILE__, __LINE__, "WSAESHUTDOWN: The socket has been shut down; it is not possible to receive on a socket after shutdown has been invoked with how set to SD_RECEIVE or SD_BOTH.", 1);
        break;
    case WSAEWOULDBLOCK:
#line 69
        err_Log(__FILE__, __LINE__, "WSAEWOULDBLOCK: The socket is marked as nonblocking and the receive operation would block.", 1);
        break;
    case WSAEMSGSIZE:
#line 71
        err_Log(__FILE__, __LINE__, "WSAEMSGSIZE: The message was too large to fit into the specified buffer and was truncated.", 1);
        break;
    case WSAEINVAL:
#line 73
        err_Log(__FILE__, __LINE__, "WSAEINVAL: The socket has not been bound with bind, or an unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled or (for byte stream sockets only) len was zero or negative.", 1);
        break;
    case WSAECONNABORTED:
#line 75
        err_Log(__FILE__, __LINE__, "WSAECONNABORTED: The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable.", 1);
        break;
    case WSAETIMEDOUT:
#line 77
        err_Log(__FILE__, __LINE__, "WSAETIMEDOUT: The connection has been dropped because of a network failure or because the peer system failed to respond.", 1);
        break;
    case WSAECONNRESET:
#line 79
        err_Log(__FILE__, __LINE__, "WSAECONNRESET: The virtual circuit was reset by the remote side executing a hard or abortive close. The application should close the socket as it is no longer usable. On a UPD-datagram socket this error would indicate that a previous send operation resulted in an ICMP >Port Unreachable< message.", 1);
        break;
    default:
        sprintf(buf, "Unknown winsock error %u!!!", err);
#line 81
        err_Log(__FILE__, __LINE__, buf, 1);
        break;
    }
}
/* 0x4247e0, clears the network statistics: the per-packet-type table net_Stats and the received / sent packet and byte totals */

void net_ClearStats(void)
{
    memset(net_Stats, 0, sizeof(net_Stats));
    net_RecvPackets = 0;
    net_RecvBytes = 0;
    net_SentPackets = 0;
    net_SentBytes = 0;
}
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
                sprintf(buf, "nt_Recv(): Received a Command with Count == cm_LastSyncCount, cmd->Count is %li, LastRequCnt is %li", net_Conn.rx->seq, cm_LastSyncCount);
            else if (net_Conn.rx->type == ' ' && net_Conn.rx->cmd == 14)
                sprintf(buf, "nt_Recv(): Received Sync, cmd->Count is %li, LastRequCnt is %li", net_Conn.rx->seq, net_Seq);
            else {
                sprintf(buf, "nt_Recv(): Lost a Command, cmd->Count is %li, nt_LastRequCount is %li", net_Conn.rx->seq, net_Seq);
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

extern int net_Seq;
extern NETSTAT net_Stats[NET_TYPES];
int net_WSAStarted = 0;  /* 0x633160 */
unsigned int net_RecvBytes = 0;  /* 0x633174 */
unsigned int net_RecvPackets = 0;  /* 0x633178 */
unsigned int net_SentBytes = 0;  /* 0x63317c */
unsigned int net_SentPackets = 0;  /* 0x633180 */
int net_Seq = 0;  /* 0x1460ac4, last received sequence number */
/* 0x1460ae0, the network connection (net.h's NETCONN, 0x14 bytes); the game code tests net_Conn.s != -1 for "a
   network game is running" */
NETCONN net_Conn = {0};  /* 0x1460ae0, the network connection of a running network game */
NETSTAT net_Stats[NET_TYPES] = {0};  /* 0x1460b00 */
