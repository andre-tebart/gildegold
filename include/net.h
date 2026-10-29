/* net.c (D:\develop\units\net\net.c): TCP connection to the game server over WinSock.
   Extent in GildeGold.exe: 0x424510..0x424f30. Names are ours. */
#ifndef NET_H
#define NET_H

#pragma pack(push, 1)
typedef struct NETPKT {             /* packet header; len is unaligned */
    unsigned char type;             /* 0x00, < NET_TYPES */
    unsigned short len;             /* 0x01, whole packet in bytes */
    unsigned char pad03;
    int idx;                        /* 0x04, its slot in the send ring (cm.h's G17MSG.idx), -1: not sequenced */
    int seq;                        /* 0x08 */
    int pad0c;
    unsigned char cmd;              /* 0x10 */
} NETPKT;
#pragma pack(pop)

typedef struct NETCONN {            /* 0x1460ae0 */
    SOCKET s;
    NETPKT *rx;                     /* 0x04, packet being received */
    NETPKT *tx;                     /* 0x08, packet being sent */
    unsigned short rxlen;           /* 0x0c, bytes received so far */
    unsigned short txlen;           /* 0x0e, bytes sent so far */
    int closed;                     /* 0x10 */
} NETCONN;

#define NET_TYPES 0x6a

typedef struct NETSTAT {            /* per packet type */
    unsigned int recvpk, recvbytes, sentpk, sentbytes;
} NETSTAT;

extern NETCONN net_Conn;                    /* 0x1460ae0 */
extern int net_Seq;                         /* 0x1460ac4, last received sequence number */
extern int cm_LastSyncCount;                   /* 0xa687e0 */
extern int net_WSAStarted;                  /* 0x633160 */
extern unsigned int net_RecvPackets;        /* 0x633178 */
extern unsigned int net_RecvBytes;          /* 0x633174 */
extern unsigned int net_SentPackets;        /* 0x633180 */
extern unsigned int net_SentBytes;          /* 0x63317c */
extern NETSTAT net_Stats[NET_TYPES];        /* 0x1460b00 */

void net_Error(int err);                                                                /* 0x424510 */
void net_ClearStats(void);                                                              /* 0x4247e0 */
void net_Count(NETPKT *pkt, unsigned int size, char recv);                              /* 0x424810 */
void net_PrintStats(void);                                                              /* 0x424860 */
SOCKET net_Connect(char *host, unsigned short port);                                    /* 0x424990 */
void net_Close(void);                                                                   /* 0x424c30 */
int net_Recv(void);                                                                     /* 0x424c80, 1 = connection lost */
int net_Send(void);                                                                     /* 0x424e70, 1 = connection lost */

#endif
