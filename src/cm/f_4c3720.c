/* 0x4c3720, queues ai_Unka68800 as parts of type 7 (0x80 bytes each); returns the seq of the first part */

int ai_Unk4c3720(void)
{
    G17MSG *m, *p, *last;
    unsigned int i;
    int first, off;

    if (ai_Unka68d30 == 0)
        return 0;
    if (net_Conn.closed != 0)
        return 0;
    last = 0;
    first = ai_Unkf81168U + 1;
    for (off = 0; off < ai_Unka68d30; off += 0x80) {
        ai_Unkf81168U++;
        i = ai_Unkf81168U & 0x7fff;
        m = &ai_Unka69140[i];
        m->type = 7;
        m->size = ai_Unk4c3180(m);
        memcpy(m->data, ai_Unka68800 + off, 0x80);
        m->unk03 = 0;
        m->idx = i;
        m->seq = ai_Unkf81168U;
        m->next7 = 0;
        if (last != 0)
            last->next7 = ai_Unkf81168U;
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
        last = m;
    }
    ai_Unka68d30 = 0;
    return first;
}
