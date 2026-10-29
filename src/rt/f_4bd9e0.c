/* 0x4bd9e0, frees list p */

void rt_FreePoints(RTPOINT *p)
{
    RTPOINT *n;

    while (p != 0) {
        n = p;
        p = p->next;
        mem_Free(n);
    }
}
