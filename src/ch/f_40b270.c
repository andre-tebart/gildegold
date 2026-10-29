/* 0x40b270, a cleared queue entry appended to the character's queue */

CHACTION *ch_NewAction(CHARACTER *ch)
{
    CHACTION *a, *p;

    a = ch_GetFreeQueueEntry();
    if (!a) {
        err_Message("ch_InsertAction(): Could not ch_GetFreeQueueEntry()");
        return 0;
    }
    memset(a, 0, sizeof(*a));
    if (!CH_ACTIONS(ch))
        CH_ACTIONS(ch) = a;
    else {
        for (p = CH_ACTIONS(ch); p->next; p = p->next)
            ;
        p->next = a;
        a->prev = p;
    }
    a->ch = ch;
    a->count = 0;
    a->func = 0;
    a->next = 0;
    a->unk10 = 0;
    ch_CheckQueue(ch, a);
    return a;
}
