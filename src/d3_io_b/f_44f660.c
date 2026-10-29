/* 0x44f660, number of points; allocates the point buffer once */

void bgi_NumPoints(void *f, BGIRD *c)
{
    bio_rd_i(f, &c->npts);
    if (c->npts > 0 && !c->pts) {
        c->ptidx = 0;
        c->ptbase = 0;
        c->pts = (float (*)[6])mem_Alloc(0x60000, "d3_io_b:BGIPointBuffer");
    }
}
