/* 0x45abc0, gives the cache entry c to slot, (re)creating its texture if size or flag differ */

D3TEXHANDLE *d3s_CacheBind(D3SCENT *c, D3SSLOT *slot, char flag, int size, unsigned int time)
{
    D3TEXFLAGS f;

    c->time = time;
    slot->cache = c;
    if (c->size > 0 && c->owner)
        *c->owner = 0;
    c->owner = (void **)slot;
    if (c->size != size || c->flag != flag) {
        if (c->tex)
            tex_Release(c->tex);
        c->size = size;
        c->flag = flag;
        memset(&f, 0, sizeof(f));
        f.b[0] = 0x7f;
        f.b[2] |= 2;
        c->tex = (D3TEXHANDLE *)tex_Create("*shadow", size, 1, f, 0, 0, 2, d3s_Palette, flag);
    }
    return c->tex;
}
