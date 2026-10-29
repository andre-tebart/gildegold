/* 0x4c3680, sends the queued messages (offline: straight into the receive list); 1: no free record */

int cm_FlushSendQueue(void)
{
    G17MSG *n;

    if (cm_SendQueue != 0) {
        if (net_Conn.s == INVALID_SOCKET) {
            while (1) {
                if (ai_Unk4c4080(cm_SendQueue) != 0)
                    return 1;
                cm_SendQueue = cm_SendQueue->next;
                if (cm_SendQueue == 0)
                    break;
                cm_SendQueue->prev = 0;
            }
        } else {
            do {
                if (net_Conn.tx != 0) {
                    net_Send();
                    if (net_Conn.tx != 0)
                        return 0;
                }
                n = cm_SendQueue->next;
                net_Conn.tx = (NETPKT *)cm_SendQueue;
                if (n != 0)
                    n->prev = 0;
                cm_SendQueue = n;
                net_Send();
            } while (cm_SendQueue != 0);
        }
    }
    return 0;
}
