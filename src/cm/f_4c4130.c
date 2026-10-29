/* 0x4c4130, takes m out of the receive list and frees it */

void ai_Unk4c4130(G17MSG *m)
{
    if (m->prev != 0)
        m->prev->next = m->next;
    else
        ai_Unka68d24 = m->next;
    if (m->next != 0)
        m->next->prev = m->prev;
    m->prev = 0;
    m->next = ai_Unka68c00;
    ai_Unka68c00 = m;
}
