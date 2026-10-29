/* 0x44f700, reads the points (x, y, z) */

void bgi_Points(void *f, BGIRD *c)
{
    int i;

    for (i = 0; i < c->npts; i++) {
        bio_rd_i(f, (int *)&c->pts[c->ptidx][0]);
        bio_rd_i(f, (int *)&c->pts[c->ptidx][1]);
        bio_rd_i(f, (int *)&c->pts[c->ptidx][2]);
        c->ptidx++;
    }
}
