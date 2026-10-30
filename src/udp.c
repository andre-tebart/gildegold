/* udp.c -- original file name unknown: UDP broadcast to the local network and a scan for game
   servers that answer it. Extent in GildeGold.exe (inferred): 0x401d50..0x402140; its .data
   (the socket at 0x5f10f8, "WSAStartup failed ...") follows the "!bmp:" strings of the bmp loader.
   Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/udp.c); boundaries in decl/text.dcl, notes in notes/units/udp.md */
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include "udp.h"

/* the engine's inline-asm clear (D3_ZERO_AT in d3.h, which needs the DirectX headers) */
#define ZERO_CAT(a, b) a##b
#define ZERO_LABEL(n) ZERO_CAT(zero_tail, n)
#define ZERO_AT(var, size) {                                        \
    int _zn = (size);                                               \
    __asm mov edi, var                                              \
    __asm mov ebx, _zn                                              \
    __asm xor eax, eax                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle ZERO_LABEL(__LINE__)                                  \
    __asm rep stosb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep stosd                                                 \
    __asm ZERO_LABEL(__LINE__): add ecx, ebx                        \
    __asm rep stosb                                                 \
}


static SOCKET udp_Socket = INVALID_SOCKET;  /* 0x5f10f8 */
struct sockaddr_in udp_Bcast = {0};        /* 0x14eaae0 */
struct sockaddr_in udp_Local = {0};        /* 0x14eaaf0 */

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
/* 0x401e50, closes udp_Socket if it is open and marks it closed again (INVALID_SOCKET) */

void udp_Exit(void)
{
    if (udp_Socket != INVALID_SOCKET) {
        closesocket(udp_Socket);
        udp_Socket = INVALID_SOCKET;
    }
}
/* 0x401e70, sends len bytes of buf to the broadcast address on the port udp_Init configured; does nothing while no socket is open */

void udp_Send(char *buf, int len)
{
    if (udp_Socket != INVALID_SOCKET)
        sendto(udp_Socket, buf, len, 0, (struct sockaddr *)&udp_Bcast, sizeof(udp_Bcast));
}
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
