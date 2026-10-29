/* 0x404860, consistency checks of the queue */

void ch_CheckQueue(CHARACTER *ch, CHACTION *a)
{
    CHACTION *p;

    if (a != CH_ACTIONS(ch)) {
        if (!a->prev)
            __asm int 3
        if (a->next && a->next->prev != a)
            __asm int 3
    }
    for (p = CH_ACTIONS(ch); p; p = p->next)
        if (p->next && p->next->ch != ch)
            __asm int 3
}
