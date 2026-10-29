/* 0x44f6b0, number of polygons; allocates the polygon buffer once */

void bgi_NumPolys(void *f, BGIRD *c)
{
    bio_rd_i(f, &c->npoly);
    if (c->npoly > 0 && !c->polys) {
        c->polyidx = 0;
        c->polybase = 0;
        c->polys = (D3BGFPOLY *)mem_Alloc(0xe0000, "d3_io_b:BGIPolyBuffer");
    }
}
