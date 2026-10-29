/* 0x446180, renders the cache textures of all tiles at their current detail level and precaches them;
   returns the number of textures drawn
   @deep B: read the whole 123-line asmdiff against 0x446180..0x4463b0 and diffclass: one slot correction at entry (0xc vs 0x10), then the same expressions with the locals permuted over slots 0x10..0x28 and other registers (the tile base edi+edx*4+0xe0 vs eax+0x5e is the same address), the three loop counters in another nesting order and the fpu stores scheduled differently; no different constant, callee, condition or field. Register/slot allocation only, needs vargen/try.py. Readability still draft: D3FLTEX.unk58 has no evidence (all uses are zero stores, no reader) */

#undef T

int d3fl_Prerender(D3FLMAP *map)
{
    int count = 0;
    unsigned int n, s, nx, ny, i, j, x, y, tx, ty, mx, my;
    int size;
    unsigned char *types;
    D3FLTEX *tex;
    D3FLCACHE *c;
    D3FLTILE *t;
    void *p;

    if (d3_Hardware && d3_Camera) {
    n = map->n;
    size = map->size;
    types = map->types;
    d3fl_UpdateTiles(map);
    for (ty = 0; ty < 8; ty++)
        for (tx = 0; tx < 8; tx++) {
            my = ty * n;
            mx = tx * n;
            t = &map->tiles[ty][tx];
            if (!t->lodLevel)
                s = map->lod ? map->lod : 1;
            else
                s = t->lodLevel;
            if (s) {
                ny = n / s + 1;
                nx = ny;
                if (tx == 7)
                    nx -= 4 / s;
                if (ty == 7)
                    ny -= 4 / s;
                for (j = 0; j < ny - 1; j++)
                    for (i = 0; i < nx - 1; i++) {
                        tex = d3fl_GetTexture(types, size, mx + i * s, my + j * s, s, map->src);
                        if (tex && tex->name[0] == '*') {
                            c = tex->cache;
                            if (!c->flag) {
                                tex->unk58 = 0;
                                if (!tex->surf)
                                    tex_Prepare(tex);
                                if (p = dd_LockSurfaceFlags(tex->surf, 0x20)) {
                                    d3fl_Render(p, &c->t);
                                    dd_UnlockSurface(tex->surf);
                                    if (d3fl_Mips)
                                        d3d_BuildMipmaps(tex->surf, 0);
                                    c->flag = 1;
                                    count++;
                                }
                            }
                            if (!(tex->state & 0x80)) {
                                d3d_PrecacheTexture(tex->d3dtex);
                                tex->state |= 0x80;
                            }
                        }
                    }
            }
        }
        return count;
    }
    return 0;
}
