/* 0x4bd9b0, unlinks and frees point p */

int rt_RemovePoint(RTPOINT *p)
{
    if (p->prev != 0)
        p->prev->next = p->next;
    if (p->next != 0)
        p->next->prev = p->prev;
    mem_Free(p);
    return 1;
}
