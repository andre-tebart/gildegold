/* 0x405610, stops the current action and drops the queued ones */

int ch_StopActions(CHARACTER *ch)
{
    CHACTION *a;

    if (!ch)
        return 0;
    a = CH_ACTIONS(ch);
    if (!a)
        return 0;
    a->stop = 1;
    while (a->next)
        ch_RemoveAction(a->next);
    if (ch->flags.all & 0x10)
        ch_StandUp(ch, "sitzend/aufstehen");
    return 1;
}
