/* 0x4c39c0, queues a copy of src in the send ring; returns its slot, -1: closed or the slot is still in use */

int ai_Unk4c39c0(G17MSG *src)
{
    G17MSG *m, *p;
    unsigned int i;

    if (net_Conn.closed != 0)
        return -1;
    i = (ai_Unkf81168U + 1) & 0x7fff;
    m = &ai_Unka69140[i];
    if (net_Conn.s != INVALID_SOCKET && (G17MSG *)net_Conn.tx == m)
        return -1;
    for (p = cm_SendQueue; p != 0; p = p->next)
        if (p == m)
            return -1;
    ai_Unkf81168U++;
    *m = *src;
    m->size = ai_Unk4c3180(m);
    m->unk03 = 0;
    m->idx = i;
    m->seq = ai_Unkf81168U;
    m->next7 = 0;
    if (src->type == 0x20 && src->data[0] == 0x0e)
        cm_LastSyncCount = m->seq;
    ai_Unkf31160[i].state = 0;
    ai_Unkf31160[i].idx = i;
    if (cm_SendQueue == 0) {
        cm_SendQueue = m;
        m->prev = 0;
    } else {
        for (p = cm_SendQueue; p->next != 0; p = p->next)
            ;
        m->prev = p;
        p->next = m;
    }
    m->next = 0;
    if (ai_Unka68d30 != 0)
        m->next7 = ai_Unk4c3720();
    return i;
}
