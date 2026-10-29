/* 0x44ed50, number of polygons of an object; allocates the polygon buffer once */

void agi_NumPolys(void *unused, AGICTX *c)
{
    int n = atoi(AGI_TOK(" "));

    c->npoly = n;
    if (n > 0 && !c->polys) {
        c->polyidx = 0;
        c->polybase = 0;
        c->polys = (AGIPOLY *)mem_Alloc(0xe0000, "d3_io_a:AGIPolyBuffer");
    }
}
