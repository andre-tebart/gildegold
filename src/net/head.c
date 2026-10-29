/* net
   unit of rea 8cef7d5b (src/game/net.c); boundaries in decl/text.dcl, notes in src/net/open.md */
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

/* 0x1460ae0, the network connection (net.h's NETCONN, 0x14 bytes). It is one object: ai_Unk1460ae0 is
   only the alias of its first field (the socket), so the full structure must be reserved here - when the
   4-byte ai_Unk1460ae0 was the definition, net_Conn.closed (+0x10) landed on the next .bss globals. */
NETCONN net_Conn;  /* 0x1460ae0, the network connection of a running network game */

