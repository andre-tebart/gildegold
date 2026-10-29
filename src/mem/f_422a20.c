/* 0x422a20 */

void *pool_Alloc(MPOOL **pool, unsigned int size, int count)
{
    MPOOL *p;
    int *slot;
    int i, sz, rem;

    if (pool && size > 0 && count > 0) {
        sz = size + 4;
        rem = size & 3;
        if (rem > 0)
            sz += 4 - rem;
        p = *pool;
        if (!p) {
            p = mem_Alloc(sz * count + sizeof(MPOOL), "m:pool_get_free(tp_m_pool)");
            *pool = p;
            p->used = 0;
            p->count = count;
            p->size = sz;
            p->next = NULL;
        } else {
            do {
                if (p->used < p->count && p->size == sz)
                    goto found;
                p = p->next;
            } while (p);
            p = mem_Alloc(sz * count + sizeof(MPOOL), "m:pool_alloc(tp_m_pool)");
            p->used = 0;
            p->count = count;
            p->size = sz;
            p->next = *pool;
            *pool = p;
        }
found:
        for (i = 0, slot = (int *)(p + 1); i < p->count; i++, slot = (int *)((char *)slot + p->size)) {
            if (*slot == 0) {
                p->used++;
                *slot++ = (int)p;
                MEM_ZERO(slot, sz - 4);
                return slot;
            }
        }
    }
    return NULL;
}
