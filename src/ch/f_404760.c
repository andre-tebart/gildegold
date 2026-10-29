/* 0x404760 */

CHACTION *ch_GetFreeQueueEntry(void)
{
    int i;

    for (i = 0; i < CH_QUEUE; )
        if (!ch_Queue[i++].func)
            break;
    if (i >= CH_QUEUE)
        return 0;
    return &ch_Queue[i - 1];
}
