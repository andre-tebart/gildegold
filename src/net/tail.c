
extern int net_Seq;
extern NETSTAT net_Stats[NET_TYPES];
int net_WSAStarted = 0;  /* 0x633160 */
unsigned int net_RecvBytes = 0;  /* 0x633174 */
unsigned int net_RecvPackets = 0;  /* 0x633178 */
unsigned int net_SentBytes = 0;  /* 0x63317c */
unsigned int net_SentPackets = 0;  /* 0x633180 */
int net_Seq = 0;  /* 0x1460ac4, last received sequence number */
/* 0x1460ae0, the network connection (net.h's NETCONN, 0x14 bytes). It is one object: ai_Unk1460ae0 is
   only the alias of its first field (the socket), so the full structure must be reserved here - when the
   4-byte ai_Unk1460ae0 was the definition, net_Conn.closed (+0x10) landed on the next .bss globals. */
NETCONN net_Conn = {0};  /* 0x1460ae0, the network connection of a running network game */
NETSTAT net_Stats[NET_TYPES] = {0};  /* 0x1460b00 */
