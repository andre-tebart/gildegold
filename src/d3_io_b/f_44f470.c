/* 0x44f470, the three point indices of a polygon */

void bgi_PolyVerts(void *f, BGIRD *c)
{
    int a;

    bio_rd_i(f, &a);
    BGI_P(c).v[0] = c->ptbase + a;
    bio_rd_i(f, &a);
    BGI_P(c).v[1] = c->ptbase + a;
    bio_rd_i(f, &a);
    BGI_P(c).v[2] = c->ptbase + a;
}
