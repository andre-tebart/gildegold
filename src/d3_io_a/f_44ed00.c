/* 0x44ed00, number of points of an object; allocates the point buffer once */

void agi_NumPoints(void *unused, AGICTX *c)
{
    int n = atoi(AGI_TOK(" "));

    c->npts = n;
    if (n > 0 && !c->pts) {
        c->ptidx = 0;
        c->ptbase = 0;
        c->pts = (AGIPOINT *)mem_Alloc(0x60000, "d3_io_a:AGIPointBuffer");
    }
}
