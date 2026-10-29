/* udp.c -- original file name unknown: UDP broadcast to the local network and a scan for game
   servers that answer it. Extent in GildeGold.exe (inferred): 0x401d50..0x402140; its .data
   (the socket at 0x5f10f8, "WSAStartup failed ...") follows the "!bmp:" strings of the bmp loader.
   Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/udp.c); boundaries in decl/text.dcl, notes in src/udp/open.md */
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

