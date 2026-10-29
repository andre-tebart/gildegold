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

extern NETCONN net_Conn;

