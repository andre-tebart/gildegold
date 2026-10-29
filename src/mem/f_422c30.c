/* 0x422c30, initialise it for pool and return the first used slot (pool_Next), NULL without pool or it */

void *pool_First(MPOOL *pool, MPOOLITER *it)
{
    if (pool && it) {
        it->pool = pool;
        it->index = 0;
        it->left = 0;
        return pool_Next(it);
    }
    return NULL;
}
