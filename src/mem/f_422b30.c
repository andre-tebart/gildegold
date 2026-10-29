/* 0x422b30 */

void pool_Free(MPOOL **pool, void *ptr)
{
    int *slot = (int *)ptr - 1;
    MPOOL *p, *q;

    if (slot && (p = (MPOOL *)*slot) && (char *)ptr >= (char *)(p + 1)
        && (char *)ptr < (char *)(p + 1) + p->size * p->count) {
        *slot = 0;
        if (--p->used == 0) {
            if (p == *pool) {
                *pool = p->next;
                mem_Free(p);
                return;
            }
            for (q = *pool; q->next != p; q = q->next)
                ;
            q->next = p->next;
            mem_Free(p);
        }
    }
}
