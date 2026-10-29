/* 0x4c4030, waits up to timeout ms for a message of type type */

G17MSG *ai_Unk4c4030(char type, int timeout)
{
    G17MSG *m;
    unsigned int end;

    end = d3_Time + timeout;
    while (d3_Time < end) {
        cm_FlushSendQueue();
        cm_ReceiveMessages();
        for (m = ai_Unka68d24; m != 0; m = m->next)
            if (m->type == type)
                return m;
    }
    return 0;
}
