/* 0x45cc90, frees the texture of slot and clears the cache entry that owned it */

void d3s_FreeSlot(D3SSLOT *slot)
{
    D3SCENT *c;

    if (slot->tex) {
        if (c = slot->cache) {
            c->owner = 0;
            c->tex = 0;
            c->size = 0;
        }
        tex_Release(slot->tex);
        slot->tex = 0;
        slot->cache = 0;
    }
}
