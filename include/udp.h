/* udp.c -- UDP broadcast and LAN server scan, original file name unknown.
   Extent in GildeGold.exe (inferred): 0x401d50..0x402140. Names are ours. */
#ifndef UNK_401D50_H
#define UNK_401D50_H

#include "ls_types.h"                     /* NETHDR */

typedef struct UDPINFO1 {           /* 0x2a bytes, server answer version 1 */
    short type;                     /* 1 */
    short version;                  /* 1 */
    short magic;                    /* 0x6a */
    char data[0x24];
} UDPINFO1;

typedef struct UDPINFO4 {           /* 0x6a bytes, server answer version 4 */
    short type;
    short version;                  /* 4 */
    short magic;
    char data[0x64];
} UDPINFO4;

typedef union UDPPACKET {           /* 0x100 bytes, receive buffer */
    struct {
        short type;
        short version;
        short magic;
    };
    UDPINFO1 v1;
    UDPINFO4 v4;
    char raw[0x100];
} UDPPACKET;

typedef struct UDPSERVER {          /* 0x80 bytes */
    char ip[16];
    union {
        UDPINFO1 v1;
        UDPINFO4 v4;
        NETHDR game;                /* v4 is the host's network game record (main_SearchNetworkGame reads its
                                       name, players, save name and party table) */
    } info;                         /* 0x10 */
    int flags;                      /* 0x7c, 0x3f (version 1) or 0x7f (version 4) */
} UDPSERVER;

void udp_Init(u_short port);                                                            /* 0x401d50 */
void udp_Exit(void);                                                                    /* 0x401e50 */
void udp_Send(char *buf, int len);                                                      /* 0x401e70 */
int udp_FindServers(u_short port, DWORD timeout, struct UDPSERVER *list, int max);             /* 0x401ea0 */

#endif
