/* 0x45ac60, a texture of size for slot: a free entry, the oldest of the same kind, or any other of the flag */

D3TEXHANDLE *d3s_CacheTexture(D3SSLOT *slot, char flag, int size, unsigned int time, D3SCENT **unused)
{
    D3SCENT *c, *best;
    unsigned int i, oldest;

    best = 0;
    oldest = time;
    for (i = 0, c = d3s_Cache; i < d3s_CacheCount; i++, c++) {
        if (c->size == size && c->flag == flag && c->time < oldest) {
            oldest = c->time;
            best = c;
        }
        if (c->size == 0)
            return d3s_CacheBind(c, slot, flag, size, time);
    }
    if (best && time - oldest > d3s_CacheAge)
        return d3s_CacheBind(best, slot, flag, size, time);
    best = 0;
    oldest = time;
    for (i = 0, c = d3s_Cache; i < d3s_CacheCount; i++, c++) {
        if (c->size > 0 && c->flag == flag && c->time < oldest) {
            oldest = c->time;
            best = c;
        }
    }
    if (best && time - oldest > 1)
        return d3s_CacheBind(best, slot, flag, size, time);
    return 0;
}
