/* 0x4c4080, appends a copy of src to the receive list; 1: no free record (closes the connection) */

int ai_Unk4c4080(G17MSG *src)
{
    G17MSG *m, *p;

    m = ai_Unka68c00;
    if (m == 0) {
        if (net_Conn.s != INVALID_SOCKET)
            net_Close();
        return 1;
    }
    ai_Unka68c00 = m->next;
    *m = *src;
    m->size = ai_Unk4c3180(m);
    if (ai_Unka68d24 == 0) {
        m->prev = 0;
        m->next = 0;
        ai_Unka68d24 = m;
        return 0;
    }
    for (p = ai_Unka68d24; p->next != 0; p = p->next)
        ;
    m->prev = p;
    m->next = 0;
    p->next = m;
    return 0;
}
