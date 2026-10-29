/* 0x4047a0, unlinks the action from its character's queue and frees it */

int ch_RemoveAction(CHACTION *a)
{
    CHACTION *prev, *next;

    if (!a)
        return 0;
    if (!a->next && a->ch && a == CH_ACTIONS(a->ch)) {
        CH_ACTIONS(a->ch) = 0;
        memset(a, 0, sizeof(*a));
        return 1;
    }
    prev = a->prev;
    if (prev)
        prev->next = a->next;
    next = a->next;
    if (next)
        next->prev = prev;
    if (!prev && a->ch)
        CH_ACTIONS(a->ch) = next;
    memset(a, 0, sizeof(*a));
    return 1;
}
