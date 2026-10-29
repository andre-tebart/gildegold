/* 0x441c50, allocates and resets the point and polygon buffers of a tile */

void d3fl_InitTile(D3FLTILE *t, unsigned int n, unsigned char shift)
{
    unsigned int i;

    n >>= shift;
    t->lodLevel = 0xff;
    t->lodDelay = 0;
    t->nextLod = 0;
    t->npolys = 0;
    t->npoints = 0;
    if (!t->points)
        t->points = mem_Alloc((n + 2) * (n + 2) * sizeof(D3FLPOINT), "d3_fl:TilePoints");
    if (!t->polys)
        t->polys = mem_Alloc((n + 1) * (n + 1) * 2 * sizeof(D3FLPOLY), "d3_fl:TilePolys");
    if (!t->split)
        t->split = mem_Alloc((n * 3 + 6) * 0x10, "d3_fl:TileSplitUpdate");
    if (!t->bp) {
        t->nbp = n * 8 + 0x10;
        t->bp = mem_Alloc((t->nbp + (n + 1) * (n + 1) * 2) * 0x18, "d3_fl:TileBPBuffer");
    }
    if (!t->wregions) {
        t->water = 0;
        t->wpoints = 0;
        t->wpolys = 0;
        t->nwpolys = 0;
        t->nwpoints = 0;
    }
    for (i = 0; i < (n + 2) * (n + 2); i++) {
        t->points[i].src = 0;
        t->points[i].flags = 0;
        t->points[i].specular = D3FL_WHITE;
        t->points[i].color = D3FL_WHITE;
    }
    for (i = 0; i < (n + 1) * (n + 1) * 2; i++) {
        t->polys[i].zOfs = 16.0f;
        t->polys[i].vOfs = 0;
        t->polys[i].uOfs = 0;
        t->polys[i].flags = 0;
        t->polys[i].v[0] = 0;
        t->polys[i].v[1] = 0;
        t->polys[i].v[2] = 0;
        t->polys[i].uv = 0;
        t->polys[i].tex = 0;
    }
}
