/* 0x4bd950, appends a new point to list p (0 if it has 8 already) */
RTPOINT *rt_AddPoint(RTPOINT *p)
{
    RTPOINT *q;
    RTPOINT *n;
    int i;

    i = 0;
    q = 0;
    if (p != 0) {
        do {
            q = p;
            i++;
            p = p->next;
        } while (p != 0);
        if (i >= 8)
            return 0;
    }
    n = mem_Alloc(0x40, "rt_point");
    memset(n, 0, 0x40);
    if (q != 0)
        q->next = n;
    n->prev = q;
    n->buildingId = -1;
    return n;
}
