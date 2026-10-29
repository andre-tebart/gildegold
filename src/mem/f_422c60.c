/* 0x422c60, free every block of the pool chain and clear *pool */

void pool_Destroy(MPOOL **pool)
{
    MPOOL *p, *n;

    if (pool) {
        for (p = *pool; p; p = n) {
            n = p->next;
            mem_Free(p);
        }
        *pool = NULL;
    }
}
