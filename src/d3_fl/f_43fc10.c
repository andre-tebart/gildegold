/* 0x43fc10, the cache texture for the tiles at x, y (created if needed) @deep Q2 only after the Q4 fix (byte already, the *(int *)&f = 0 on D3TEXFLAGS replaced by a local int/D3TEXFLAGS union view): D3FLTEX.unk58 (slot 0x58) has no role evidence - only zero stores in the sources (here, d3fl_Alloc eviction, both d3fl_Draw re-render sites, ts_texture.c tex_Load/tex_Create/tex_CreateHi init, d3_tools DC_ unload), no reader in the drafted source, 0x54 next to it is the last-use stamp; callers and data read - a name would be invented */

D3FLTEX *d3fl_GetTexture(unsigned char *map, int mapsize, int x, int y, unsigned int size, D3FLSRC *src)
{
    D3FLCACHE *c;
    int hash;
    unsigned int i, j;
    union {
        int i;                      /* the flags as the dword the original zeroes at once */
        D3TEXFLAGS f;
    } u;

    if (c = d3fl_Find(map, mapsize, x, y, size, &hash))
        return c->tex;
    if (c = d3fl_Alloc()) {
        char names[5][7] = { "*", "*fltx1", "*fltx2", "*", "*fltx4" };

        int mask;

        c->size = size;
        mask = mapsize - 1;
        for (j = 0; j <= size; j++)
            for (i = 0; i <= size; i++)
                c->t.tiles[j][i] = map[((x + i) & mask) + ((j + y) & mask) * mapsize];
        c->hash = hash;
        c->t.n = size;
        c->t.x = x;
        c->t.y = y;
        c->t.src = src;
        if (!c->tex) {
            c->t.texsize = min((0x40 >> d3fl_DetailShift) * size, d3fl_DetailScale << d3fl_DetailSizeShift);
            u.i = 0;
            u.f.b[0] = 0xff;
            c->tex = tex_CreateHi(names[size], c->t.texsize, u.f, 0, 0, d3fl_Mips);
            c->tex->unk58 = 0;
        } else
            strcpy(c->tex->name, names[size]);
        c->tex->cache = c;
        c->flag = 0;
        return c->tex;
    }
    return 0;
}
