/* 0x4c3fd0, receives the waiting messages into the receive list; 1: no free record */

int cm_ReceiveMessages(void)
{
    if (net_Conn.s != INVALID_SOCKET) {
        if (net_Conn.rx != 0) {
            net_Recv();
            if (net_Conn.rx != 0)
                return 0;
        }
        do {
            ai_Unk6f0cd0++;
            if (ai_Unk4c4080(&ai_Unkf81180) != 0)
                return 1;
            net_Conn.rx = (NETPKT *)&ai_Unkf81180;
            net_Recv();
        } while (net_Conn.rx == 0);
    }
    return 0;
}
