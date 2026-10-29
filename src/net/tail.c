
int net_Seq;  /* 0x1460ac4, last received sequence number */
NETSTAT net_Stats[NET_TYPES];  /* 0x1460b00 */
int net_WSAStarted = 0;  /* 0x633160 */
unsigned int net_RecvBytes = 0;  /* 0x633174 */
unsigned int net_RecvPackets = 0;  /* 0x633178 */
unsigned int net_SentBytes = 0;  /* 0x63317c */
unsigned int net_SentPackets = 0;  /* 0x633180 */
